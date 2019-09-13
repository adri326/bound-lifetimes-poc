#include "runtime.h"

void test() {
  printf("It works!\n");
}

void _box_bind(Box_void* parent, void* child) {
  Reference* new_ref = malloc(sizeof(Reference));
  new_ref->ptr = child;
  new_ref->next_ref = NULL;
  if (parent->ref == NULL) {
    parent->ref = new_ref;
  } else {
    parent->last_ref->next_ref = new_ref;
  }
  parent->last_ref = new_ref;
  parent->ref_count++;
}

Box(void)* _box_new(void* value) {
  Box(void)* new_box = malloc(sizeof(Box(void)));
  new_box->obj = value;
  new_box->ref = NULL;
  new_box->last_ref = NULL;
  new_box->ref_count = 0;
  return new_box;
}

void _box_drop(Box_void* parent, void* child, void* from) {
  printf("Dropping a reference...\n");
  if ((!_box_has(parent, from) || from == NULL) && _box_has(parent, child)) {
    parent->ref_count--;
    printf(" ^-- reference dropped, now only having (%d) refs\n", parent->ref_count);
  }
  if (parent->ref_count <= 0) {
    _box_free(parent);
  }
}

void _box_ref(Box_void* parent, void* child, void* from) {
  printf("Registering a reference...\n");
  if (!_box_has(parent, from) && _box_has(parent, child)) {
    printf(" ^-- reference registered\n");
    parent->ref_count++;
  }
}

bool _box_has(Box_void* parent, void* child) {
  Reference* ref = parent->ref;
  while (ref != NULL) {
    if (ref->ptr == child) return true;
    ref = ref->next_ref;
  }
  return false;
}

void _box_free(Box_void* parent) {
  Reference* ref = parent->ref;
  size_t count = 0;
  while (ref != NULL) {
    Reference* next_ref = ref->next_ref;
    count++;
    free(ref->ptr);
    free(ref);
    ref = next_ref;
  }
  printf("Freed %d references\n", count);
  parent->last_ref = NULL;
  parent->ref = NULL;
}
