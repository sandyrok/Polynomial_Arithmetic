#include "poly_header.h"

int main() {
  poly_node *dividend = NULL, *divisor = NULL, *quotient = NULL,
            *remainder = NULL;
  char c;
  do {
    if (!read_polynomial(&dividend))
      printf("Input is not in the expected format\n");
    if (!read_polynomial(&divisor))
      printf("Input is not in the expected format\n");
    divide_polynomial(dividend, divisor, &quotient, &remainder);

    printf("Quotient:  ");
    print_polynomial(quotient);
    printf("Remainder:  ");
    print_polynomial(remainder);
    dealloc_poly(&dividend);
    dealloc_poly(&divisor);
    dealloc_poly(&quotient);
    dealloc_poly(&remainder);
    do {
      scanf("%c", &c);
    } while (c == '\n' || c == '\t' || c == ' ');
  } while (c == 'y' || c == 'Y');
}
