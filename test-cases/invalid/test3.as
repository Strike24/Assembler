; Invalid test case #3, pre assembler and macros
START:      add r3, r5
            mcro dec
            dec HEAD
            dec r3
            dec r5
            mcroend
            mcro m_acro
            stop
            mcroend
HELLOWORLD: dec
            mcro
            mcroend
            cmp r2, r7
mcro THISMCRONAMEISTOOLONGANDEXCEEDSMAX
bne START
mcroend
mcro m_acro
prn r3
mcroend
mcro !cool_name!
bne &HELLOWORLD
mcroend
mcro best_name-ever!
mcroend