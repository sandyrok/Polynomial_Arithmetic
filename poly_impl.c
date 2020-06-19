#include "poly_header.h"

poly_node *create_poly_node(float coeff, int expo, poly_node *prev,
                            poly_node *next) {
  poly_node *tmp = (poly_node *)malloc(sizeof(poly_node));
  tmp->coeff = coeff;
  tmp->expo = expo;
  tmp->next = next;
  tmp->prev = prev;
  return tmp;
}

void dealloc_poly(poly_node **head) {
  poly_node *tmp;
  while (*head != NULL) {
    tmp = (*head)->next;
    free(*head);
    *head = tmp;
  }
}

void append_poly_node(poly_node **last, int expo, float coeff) {

  if (*last == NULL) {
    (*last) = create_poly_node(coeff, expo, NULL, NULL);
    return;
  }
  (*last)->next = create_poly_node(coeff, expo, (*last), NULL);
  (*last) = (*last)->next;
  return;
}

void flush_blanks() {
  char c;
  if ((c = getc(stdin)) == ' ' || c == '\n' || c == '\t')
    while ((c = getc(stdin)) == ' ' || c == '\n' || c == '\t')
      ;
  ungetc(c, stdin);
}

void print_polynomial(poly_node *poly) {
  if (poly == NULL) {
    printf("0\n");
    return;
  }
  while (poly != NULL) {
    printf("(%d, %f) ", poly->expo, poly->coeff);
    poly = poly->next;
  }
  printf("\n");
  return;
}

bool read_polynomial(poly_node **poly) {
  flush_blanks();
  char c;
  bool error = false;
  int prev_expo = INT_MAX;
  flush_blanks();
  int expo;
  float coeff;
  *poly = NULL;
  poly_node *last = NULL;
  while (!error && (c = getc(stdin)) != '\n') {
    if (c == '(') {
      scanf("%d", &expo);
      if (expo >= prev_expo || expo < 0)
        error = true;
      flush_blanks();
      if ((c = getc(stdin)) != ',') {
        error = true;
        continue;
      }
      flush_blanks();
      scanf("%f", &coeff);
      if (coeff != 0)
        append_poly_node(&last, expo, coeff);
      if (*poly == NULL)
        *poly = last;
      flush_blanks();
      if ((c = getc(stdin)) != ')')
        error = true;
    } else if (c == ',' || c == ' ' || c == '\t')
      continue;

    else
      error = true;
    prev_expo = expo;
  }
  if (error)
    *poly = NULL;
  return !error;
}

poly_node *insert_zero_terms(poly_node *dividend) {
  int degree = dividend->expo, i, j, k;
  poly_node *full_dividend = NULL;

  while (degree >= 0) {
    if (dividend != NULL && dividend->expo == degree) {
      append_poly_node(&full_dividend, degree, dividend->coeff);
      dividend = dividend->next;
    } else
      append_poly_node(&full_dividend, degree, 0);
    degree--;
  }
  while (full_dividend != NULL && full_dividend->prev != NULL) {
    full_dividend = full_dividend->prev;
  }

  return full_dividend;
}

poly_node *remove_zero_terms(poly_node *poly) {
  poly_node *tmp = NULL;
  while (poly != NULL) {
    if (poly->coeff != 0)
      append_poly_node(&tmp, poly->expo, poly->coeff);
    poly = poly->next;
  }
  while (tmp != NULL && tmp->prev != NULL) {
    tmp = tmp->prev;
  }
  return tmp;
}

bool divide_polynomial(poly_node *dividend, poly_node *divisor,
                       poly_node **quotient, poly_node **remainder) {
  if (dividend == NULL || divisor == NULL)
    return false;
  int deg_a = dividend->expo, deg_b = divisor->expo;
  poly_node *full_dividend = insert_zero_terms(dividend),
            *full_divisor = insert_zero_terms(divisor);
  int i, j, k, a;
  float q, b;
  poly_node *ptr_q;
  b = divisor->coeff;
  poly_node *ptr_a = NULL, *ptr_b = NULL;
  for (k = deg_a - deg_b; k >= 0; k--) {
    q = (full_dividend->coeff) / b;
    if (q != 0) {
      ptr_a = full_dividend->next;
      ptr_b = full_divisor->next;
      for (j = deg_b + k - 1; j >= k; j--) {
        ptr_a->coeff = ptr_a->coeff - q * ptr_b->coeff;
        ptr_a = ptr_a->next;
        ptr_b = ptr_b->next;
      }
      append_poly_node(quotient, k, q);
    }
    full_dividend = full_dividend->next;
  }
  while ((*quotient) != NULL && (*quotient)->prev != NULL) {
    (*quotient) = (*quotient)->prev;
  }
  *remainder = remove_zero_terms(full_dividend);
  dealloc_poly(&full_dividend);
  dealloc_poly(&full_divisor);
}
