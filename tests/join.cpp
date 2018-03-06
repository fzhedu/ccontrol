#include <stdio.h>
#include <algorithm>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
extern "C" {
#include "ccontrol.h"
}
int color_num = 192;
int color_num_build = 100;
int extra_byte = 64;
typedef unsigned long long lld;
#define MALLOC 0
struct ColorAddr {
  struct ccontrol_zone* zone;
  void* pointer;
};
void destroy(ColorAddr* addr) {
  ccontrol_free(addr->zone, addr->pointer);
  ccontrol_destroy_zone(addr->zone);
  ccontrol_delete(addr->zone);
}
ColorAddr* bulid_ht(lld size) {
  void* ht_addr = NULL;
#if MALLOC
  ht_addr = malloc(size * 8);
#else
  struct ccontrol_zone* zone = NULL;
  color_set c_set;

  COLOR_ZERO(&c_set);
  for (int i = 0; i < color_num_build; ++i) {
    COLOR_SET(i, &c_set);
  }
  zone = ccontrol_new();
  assert(zone != NULL);

  int i = ccontrol_create_zone(zone, &c_set, size * 8 + extra_byte);
  assert(i == 0);
  ht_addr = ccontrol_malloc(zone, size * 8);
#endif
  for (int i = 0; i < size; ++i) {
    *((lld*)(ht_addr + i * 8)) = i;
  }
  ColorAddr* addr = new ColorAddr;
  addr->pointer = ht_addr;
  addr->zone = zone;
  return addr;
}
ColorAddr* gen_probe_data(lld size) {
  void* probe_addr = NULL;
#if MALLOC
  probe_addr = malloc(size * 8);
#else
  struct ccontrol_zone* zone = NULL;
  color_set c_set;

  COLOR_ZERO(&c_set);
  for (int i = color_num_build; i < color_num; ++i) {
    COLOR_SET(i, &c_set);
  }
  zone = ccontrol_new();
  assert(zone != NULL);

  int i = ccontrol_create_zone(zone, &c_set, size * 8 + extra_byte);
  assert(i == 0);
  probe_addr = ccontrol_malloc(zone, size * 8);
#endif
  for (int i = 0; i < size; ++i) {
    *((lld*)(probe_addr + i * 8)) = i;
  }
  ColorAddr* addr = new ColorAddr;
  addr->pointer = probe_addr;
  addr->zone = zone;
  return addr;
}
void probe(void* ht_addr, void* probe_addr, lld probe_size, lld build_size) {
  lld probe_key, equal = 0;
  for (lld index = 0; index < probe_size; ++index) {
    probe_key = (*((lld*)(probe_addr + index * 8)));
    if (probe_key == *((lld*)(ht_addr + (probe_key % build_size) * 8))) {
      ++equal;
    }
  }
  printf("equal num = %lld\n", equal);
}
int main() {
  lld probe_size = 10240, build_size = 1024;
  ColorAddr* ht_addr = bulid_ht(build_size);
  ColorAddr* probe_addr = gen_probe_data(probe_size);
  probe(ht_addr->pointer, probe_addr->pointer, probe_size, build_size);
  destroy(ht_addr);
  destroy(probe_addr);
  return 0;
}
