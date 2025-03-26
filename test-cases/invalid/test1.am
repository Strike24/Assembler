; invalid test case #1, command operands
!label: mov r3, #300
        add r2, STR
        cmp r4, DATA, r5
        jmp &UNKNOWN
        stop

STR:    .string "test"
DATA:   .data 5, -12, 30

LOOP: add r7, r8
         mov LIST, r3
         sub r5
         prn UNKNOWN
         stop

LIST:  .data 1, 2, "Hello"