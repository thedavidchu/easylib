/** Test file */

#include "easylib.h"

void test_integers(void) {
  /* Test basic addition: a + b = c */
  struct EasyInteger a = EasyInteger__from_cstr("+1000000");
  struct EasyInteger b = EasyInteger__from_cstr("+100");
  struct EasyInteger c = EasyInteger__add(&a, &b);

  EasyInteger__print(&a);
  printf(" + ");
  EasyInteger__print(&b);
  printf(" = ");
  EasyInteger__print(&c);
  printf("\n");

  /* Test addition carrying */
  struct EasyInteger d = EasyInteger__from_cstr("+9999");
  struct EasyInteger e = EasyInteger__from_cstr("+999");
  struct EasyInteger f = EasyInteger__add(&d, &e);

  EasyInteger__print(&d);
  printf(" + ");
  EasyInteger__print(&e);
  printf(" = ");
  EasyInteger__print(&f);
  printf("\n");

  /* Test subtraction borrowing */
  struct EasyInteger g = EasyInteger__from_cstr("+1000");
  struct EasyInteger h = EasyInteger__from_cstr("-999");
  struct EasyInteger i = EasyInteger__add(&g, &h);

  EasyInteger__print(&g);
  printf(" + ");
  EasyInteger__print(&h);
  printf(" = ");
  EasyInteger__print(&i);
  printf("\n");

  /* Destroy all integers */
  EasyInteger__destroy(&a);
  EasyInteger__destroy(&b);
  EasyInteger__destroy(&c);
  EasyInteger__destroy(&d);
  EasyInteger__destroy(&e);
  EasyInteger__destroy(&f);
  EasyInteger__destroy(&g);
  EasyInteger__destroy(&h);
  EasyInteger__destroy(&i);
}

int main(void) {
  test_integers();
  return 0;
}
