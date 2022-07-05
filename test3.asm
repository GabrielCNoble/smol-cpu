mov accw, [base + 4]
add accw, 1
mov [base + 4], accw
mov base, [base + 2]