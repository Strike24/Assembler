      Thisssssss lliiiinnneeeee iss wayyyyyyyy tooooo longgggggggggg      TTTTThissssss lliiiinnneeeeeeee isssss wayyyyyyyy tooooo lllllongggggggggggg!!!!!

;Invalid mcroo declaration, no name was defined
mcro
macroend

;Invalid mcroo declaration, "mcro" command with extraneous text
e mcro mcroo_name
macroend

;Invalid mcroo declaration, an opcode name cannot be used as a mcroo name
mcro mov
macroend

;Invalid mcroo declaration, a register name cannot be used as a mcroo name
mcro r0
macroend

;Invalid mcroo declaration, an instruction name cannot be used as a mcroo name
mcro .data
macroend

;Invalid mcroo declaration, mcroo should be assigned with one name
mcro mcroo1 mcroo2
macroend

;Invalid mcroo declaration, this mcroo name is already in use
mcro mymcroo
    mov r1, r2
macroend

mcro mymcroo
    add r1, r3
macroend

;Invalid mcroo declaration, "macroend" command with extraneous text
mcro anothermcroo
    sub r2, r4
macroend extra_text_here

;Invalid mcroo declaration, mcroo name is too long, 31 characters max
mcro thismcrooNameIsWayTooLongAndInvalid
    clr r5
macroend

;Invalid mcroo declaration, empty mcroo content
mcro emptymcroo

macroend

;Invalid mcroo declaration, "mcro" is a reserved word
mcro mcro
    inc r1
macroend

;Invalid mcroo declaration, "macroend" is a reserved word
mcro macroend
    dec r2
macroend