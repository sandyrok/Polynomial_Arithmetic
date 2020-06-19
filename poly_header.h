#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct poly_node {
  float coeff;
  int expo;
  struct poly_node *next, *prev;
} poly_node;

poly_node *create_poly_node(float coeff, int expo, poly_node *prev,
                            poly_node *next);
void dealloc_poly(poly_node **head);
void append_poly_node(poly_node **last, int expo, float coeff);
void flush_blanks();
void print_polynomial(poly_node *poly);
bool read_polynomial(poly_node **poly);
poly_node *insert_zero_terms(poly_node *dividend);
poly_node *remove_zero_terms(poly_node *poly);
bool divide_polynomial(poly_node *dividend, poly_node *divisor,
                       poly_node **quotient, poly_node **remainder);
