#include "filefunctions.h"

FILE *open_file(char *filename, char *mode, char *ext)
{
    ErrorObject error = {0};
    char *ext_filename = NULL;
    FILE *file = NULL;

    if (filename == NULL || mode == NULL || ext == NULL)
    {
        /*If any of the parameters are null return null*/
        return NULL;
    }
    /*Allocates memory for a new file name that includes the extension*/
    ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        /*If memory allocation failed, handle the error and return NULL*/
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return NULL;
    }

    /*Initializes all chars to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);

    /*Copies the original file name to the new file name*/
    strcpy(ext_filename, filename);
    /*Adds the extension to the new file name*/
    strcat(ext_filename, ext);

    /*Opens the file with the new file name and the mode*/
    file = fopen(ext_filename, mode);

    free(ext_filename); /*ext_file no longer needed, free allocation*/

    if (file == NULL)
    {
        if (strcmp(mode, "r") == 0)
        {
            /*Failed to open file for reading, handle error and return null*/
            fill_error_object(ERROR_FILE_NOT_FOUND, 0, filename, &error);
        }
        else
        {
            /*Failed to open file for writing, handle error and return null*/
            fill_error_object(ERROR_FILE_OPEN_FAILED, 0, filename, &error);
        }

        handle_error(&error);
        return NULL;
    }

    return file;
}

void remove_file(char *filename, char *ext)
{
    /*Allocates memory for a new file name that includes the extension*/
    char *ext_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);

    if (ext_filename == NULL)
    {
        /*If memory allocation failed, handle the error and return*/
        handle_system_error(ERROR_MEMORY_ALLOCATION_FAILED);
        return;
    }

    /*Initializes all chars to avoid garbage values*/
    memset(ext_filename, 0, strlen(filename) + strlen(ext) + 1);
    /*Copies the original file name to the new file name*/
    strcpy(ext_filename, filename);
    /*Adds the extension to the new file name*/
    strcat(ext_filename, ext);

    /*Removes the file with the new file name*/
    remove(ext_filename);

    free(ext_filename);
}

int build_output_files(char *filename, BinaryNode *code_image, BinaryNode *data_image, Label *label_list, ExternLabel *extern_list, int ICF, int DCF)
{
    FILE *ob_file;
    FILE *ent_file;
    FILE *ext_file;
    BinaryNode *current_node = code_image;
    Label *current_label = label_list;
    ExternLabel *current_extern_label = extern_list;
    int is_ent_file = FALSE;
    int is_ext_file = FALSE;

    /*CREATE FILES*/
    ob_file = create_ob_file(code_image, data_image, filename);
    ent_file = create_ent_file(label_list, filename, &is_ent_file);
    ext_file = create_ext_file(extern_list, filename, &is_ext_file);

    if (!ob_file || (!ent_file && is_ent_file) || (!ext_file && is_ext_file)) /*If one of the files is supposed to be created but not, return*/
    {
        /*If any of the files failed to open, return error and stop building files*/
        if (ob_file)
            fclose(ob_file);
        if (ent_file)
            fclose(ent_file);
        if (ext_file)
            fclose(ext_file);
        return ERROR;
    }

    /*WRITE TO OBJECT FILE*/

    fprintf(ob_file, "%7d %-6d\n", ICF - 100, DCF); /*Write the header to the object file*/
    /*Write the code image to the object file,
    looping in reverse for order to be from first to last*/
    current_node = code_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != code_image)
    {
        /*Print the line in hex*/
        print_line_ob(ob_file, current_node->line, 0);
        current_node = current_node->prev;
    }

    /*Write the data image to the object file,
    looping in reverse for order to be from first to last*/
    current_node = data_image;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }
    while (current_node != data_image)
    {
        /*Print the line in hex*/
        print_line_ob(ob_file, current_node->line, ICF);
        current_node = current_node->prev;
    }

    /*Done writing the object file*/
    fclose(ob_file);

    /*WRITE TO ENTRY FILE*/
    current_label = label_list;
    if (is_ent_file && current_label != NULL)
    {
        while (current_label->next != NULL)
        {
            current_label = current_label->next;
        }
        while (current_label != label_list)
        {

            if (current_label->type == ENTRY)
            {
                /*If label is entry, write it to the entry file with its address*/
                fprintf(ent_file, "%s %07d\n", current_label->name, current_label->address);
            }
            current_label = current_label->prev;
        }
        fclose(ent_file);
    }

    current_extern_label = extern_list;
    if (is_ext_file)
    {
        while (current_extern_label->next != NULL)
        {
            current_extern_label = current_extern_label->next;
        }
        while (current_extern_label != extern_list)
        {
            if (current_extern_label->usage_address != 0 && current_extern_label->label->name != NULL)
            {
                /*For every external label usage, write it to the file with the usage address and name*/
                fprintf(ext_file, "%s %07d\n", current_extern_label->label->name, current_extern_label->usage_address);
            }
            current_extern_label = current_extern_label->prev;
        }
        fclose(ext_file);
    }

    /*Files built, exiting file building*/
    printf("* No errors found in the assembly process, output files built. *\n");
    return 0;
}

FILE *create_ob_file(BinaryNode *code_image, BinaryNode *data_image, char *filename)
{
    FILE *ob_file = NULL;
    /*If theres at least one code or data line in the memory images, create an object file*/
    if (code_image->next != NULL || data_image->next != NULL)
    {
        ob_file = open_file(filename, "w", OB_EXT);
        if (ob_file == NULL)
        {
            return NULL;
        }
        return ob_file;
    }
    return NULL; /*If no code or data lines, return null*/
}

FILE *create_ent_file(Label *label_list, char *filename, int *is_ent_file)
{
    FILE *ent_file = NULL;
    Label *current_label = label_list;
    while (current_label != NULL)
    {
        if (current_label->type == ENTRY)
        {
            /*If there is at least one entry label, create an entry file*/
            ent_file = open_file(filename, "w", ENT_EXT);
            if (ent_file == NULL)
            {
                return NULL;
            }
            *is_ent_file = TRUE;
            return ent_file;
        }
        current_label = current_label->next;
    }

    return NULL; /*If no entry labels, return null*/
}

FILE *create_ext_file(ExternLabel *extern_list, char *filename, int *is_ext_file)
{
    FILE *ext_file = NULL;
    ExternLabel *current_extern_label = extern_list;
    while (current_extern_label != NULL)
    {
        if (current_extern_label->label != NULL)
        {
            /*If there is at least one external label usage, create an extern file*/
            ext_file = open_file(filename, "w", EXT_EXT);
            if (ext_file == NULL)
            {
                return NULL;
            }
            *is_ext_file = TRUE;
            return ext_file;
        }
        current_extern_label = current_extern_label->next;
    }
    return NULL; /*If no external labels usage, return null*/
}

int print_line_ob(FILE *ob_file, BinaryLine *current_node, int starting_index)
{
    int i = 0;
    if (current_node == NULL)
    {
        return ERROR;
    }
    for (i = 0; i < current_node->num_of_lines; i++)
    {
        /*Print the line in hex, address in acending order*/
        fprintf(ob_file, "%07d %06x\n", current_node->address + i + starting_index, current_node->code[i]);
    }
    return 0;
}