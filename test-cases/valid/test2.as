.entry DATA
.extern X
START:  mov r2, DATA
LOOP:   cmp r2, #10
        bne NEXT
        mcro test_macro
        add r1, X
        add r2, X
        add r3, X
        inc LABEL2
        mcroend
        prn r2
        jmp &END
NEXT:   add r1, X
        lea LABEL, r5
        inc r5
        dec COUNT
        test_macro
        bne LOOP
        jmp LABEL2
END:    stop

LABEL:  .string "hello-world!"
DATA:   .data 5, -3, 8
        .data 20
COUNT:  .data 4
.extern LABEL2
