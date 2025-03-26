; Invalid test case #2, label names and data
HEAD: .string "coded-in-24-bit-memory"
      bne SKIP
      lea HEAD, #32
      .string "this-string-is-too-long-for-the-assembler-and-is-bigger-than-81-chars"
      mcro HEAD
      .data 7,25
      dec NUMBER
      mcroend
NUMBER: .data 115
EXIT: stop r5
    HEAD
lea: cmp r3, r5
THISLABELNAMEISTOOLONGANDEXCEEDSMAX: cmp r5, r7
