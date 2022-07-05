mov [base + 4], accw
mov accw, [base + 4]
mov accw, [label]
mov accw, [base + label]
mov accw, [fun_global + base]
mov accw, fun_string
mov [base + 4], accw
mov [label], accw
mov [base + label], accw
mov [fun_string], base
.byte(0x10)
label:

fun_global: .word 0x1234
fun_string: .string "big blah"