#include <stdio.h>
#include <assert.h>
extern "C" {
#include "ccontrol.h"
// void do_stuff(char *t, size_t s);
}

void print(char *t, size_t s) {
  for (int i = 0; i < s; ++i) {
    printf("%c", t[i]);
  }
  puts("");
}
int main(void) {
  char *t = NULL;
  struct ccontrol_zone *z = NULL;
  color_set c;

  COLOR_ZERO(&c);
  for (int i = 0; i < 32; i++) COLOR_SET(i, &c);
  puts("colors are set");
  z = ccontrol_new();
  printf("the address of z is %d\n", z);
  // assert(z!= NULL);

  int i = ccontrol_create_zone(z, &c, 40);
  // assert(i==0);
  puts("have created zone");
  t = (char *)ccontrol_malloc(z, 10);
  printf("the address of memory is %d \n", t);
  // do_stuff(t,100);
  // print(t,10);
  ccontrol_free(z, t);

  ccontrol_destroy_zone(z);
  ccontrol_delete(z);
  return 0;
}
