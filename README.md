# C Assembler for Custom Architecture
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)<br>
### Project Overview
This project is a complete Assembler in ANSI C for a hypothetical 12-bit CPU architecture. The program takes a raw assembly source file (.as), processes it, and outputs the binary machine code (.ob) along with memory maps for external and entry symbols.

[Project Writeup in a PDF Format](https://s3.us-east-1.amazonaws.com/pub-cdn-us.apitemplate.io/2025/12/bd970fa3-4ba8-4435-8cff-9a928ae9c628.pdf?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIA6ENCBKJYLWJUD36X%2F20251206%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20251206T153116Z&X-Amz-Expires=7200&X-Amz-Signature=7f5fa2ce63b5ee69a447db8f05ef5eaa9c3f41622f670d9972a3d241c27182c6&X-Amz-SignedHeaders=host&x-amz-checksum-mode=ENABLED&x-id=GetObject) (Or you can just read it below in markdown)

## How it works - System Architecture 🛠️
The assembler works with a Two-Pass Algorithem to handle translation process.
1. **Pre-Processor** 🟠
   * Scans source code to identify macro definitions
   * Expands these macros inline and generates an intermediate file (.am), stripping out the definitions so the main assembler can process linear code.
2. **First Pass** 🔴
   * Syntax Analysis - Parses every line to validate opcodes, operands, and addressing modes (Immediate, Direct, Struct, Register).
   * Memory Counting - Calculates the memory size of every instruction (IC) and data definition (DC). This is critical because size varies.
   * Symbol Table - Builds a Linked List of all labels (e.g., LOOP:, MAIN:) and assigns them memory addresses.
3. **Second Pass** 🟢
   * Forward Referencing - Uses the symbol table from Pass One to fill in the addresses of labels that were called before they were defined.
   * Bitwise Packing - Uses bitwise operations (<<, &, |) to physically pack the Opcode, Source Register, Destination Register, and ARE (Attributes) into the 12-bit output words.

## Error Management ⚠️
The assembler was designed to emulate real-world compilers (like gcc), prioritizing robustness and informative feedback.
* **Non-Blocking Error Detection:** Instead of exiting on the first syntax error, the assembler sets an internal error_flag and continues parsing the rest of the file. This allows the user to see all syntax errors in the source code in a single run, rather than fixing them one by one.
  
* **Memory Safety in Failure States:** A critical design challenge was ensuring memory is freed even when the program fails. I implemented a cleanup routine. Whether the program finishes successfully or encounters syntax errors, the exit path always traverses the Linked Lists and Symbol Tables to free() every allocated block, ensuring zero memory leaks (verified with Valgrind).

* **Context Reporting:** I wrote a dedicated error-reporting module that takes the current file name, line number, and error code. It prints formatted messages to stderr, making debugging the assembly code intuitive for the user. <br>Example: `file.as:14: Error: Undefined label 'LOOP_START'`

* **Input Validation:** Every string parsing function (like strtok or custom pointer arithmetic) includes bounds checking to prevent buffer overflows if the input file contains malformed lines (e.g., a line longer than the buffer or missing commas).

## Technical Highlights 💻
* **Low-Level Memory Management** <br>
Fully dynamic memory allocation using malloc and free. The system utilizes Linked Lists to manage an unknown number of symbols and macros, ensuring no memory leaks occur even if the program errors out. The program frees every allocated memory in every exit.
* **Bitwise Manipulation** <br>
Direct manipulation of bits to construct the binary output according to the specific Instruction Set Architecture (ISA).
* **Parsing** <br>
wrote a custom text parser to handle whitespace, commas, and string validation without relying on heavy external libraries.

## Skills Learned 💡
* Low-Level C: pointers, structs, and dynamic memory allocation.
* Data Structures: Implementation of Linked Lists and Hash Tables for symbol management.
* System Programming: File I/O manipulation and binary file generation.
* Computer Architecture: Understanding of instruction cycles, memory addressing modes (Immediate, Direct, Register), and the translation of mnemonics to binary.
* Error handeling & Memory Mangement
