#include "runtime.h"

void test() {
  printf("It works!\n");
}

Box* Box_new() {
  Box* new_box = malloc(sizeof(Box));
  new_box->ref = NULL;
  new_box->last_ref = NULL;
  new_box->ref_count = 0;
  new_box->ref_count_self = 1;
  return new_box;
}

void _box_bind(Box* parent, Ref(void)* child) {
  RefList* new_ref = malloc(sizeof(RefList));
  new_ref->ptr = child;
  new_ref->next_ref = NULL;
  if (parent->ref == NULL) {
    parent->ref = new_ref;
  } else {
    parent->last_ref->next_ref = new_ref;
  }
  parent->last_ref = new_ref;
  parent->ref_count += child->ref_count;
  child->parent = parent;
}

void _box_drop(Box* parent, Ref(void)* child, Ref(void)* from) {
  printf("Dropping a reference...\n");
  if ((!_box_has(parent, from) || from == NULL) && _box_has(parent, child)) {
    parent->ref_count--;
    printf(" ^-- reference dropped, now only having (%d) refs\n", parent->ref_count);
  }
  if (parent->ref_count <= 0) {
    _box_free(parent);
  }
}

void _box_ref(Box* parent, Ref(void)* child, Ref(void)* from) {
  printf("Registering a reference...\n");
  if (!_box_has(parent, from) && _box_has(parent, child)) {
    printf(" ^-- reference registered\n");
    parent->ref_count++;
  }
}

bool _box_has(Box* parent, Ref(void)* child) {
  RefList* ref = parent->ref;
  while (ref != NULL) {
    if (ref->ptr == child) return true;
    ref = ref->next_ref;
  }
  return false;
}

void _box_free(Box* parent) {
  RefList* ref = parent->ref;
  size_t count = 0;
  while (ref != NULL) {
    RefList* next_ref = ref->next_ref;
    count++;
    free(ref->ptr->value); // Free the actual value
    free(ref->ptr); // Free its reference object
    free(ref); // Free the RefList object
    ref = next_ref;
  }
  printf(" ^ Freed %d references\n", count);
  parent->last_ref = NULL;
  parent->ref = NULL;
  if (parent->ref_count_self <= 0) {
    printf(" ^ Freed the box itself\n");
    free(parent);
  }
}

void Box_drop_self(Box* parent) {
  parent->ref_count_self--;
  if (parent->ref_count <= 0 && parent->ref_count_self <= 0) {
    free(parent);
    printf(" ^ Freed the box itself\n");
  }
}

Ref(void)* _ref_new(void* value) {
  Ref(void)* new_ref = malloc(sizeof(Ref(void)));

  new_ref->value = value;
  new_ref->parent = NULL;
  new_ref->ref_count = 1; // the variable holding it

  return new_ref;
}

bool _ref_ref(Ref(void)* child, Ref(void)* from) {
  if (child->parent == NULL) {
    printf("Huh\n");
    child->ref_count++;
    return false;
  } else {
    _box_ref(child->parent, child, from);
  }
}

bool _ref_drop(Ref(void)* child, Ref(void)* from) {
  if (child->parent == NULL) {
    child->ref_count--;
    return false;
  } else {
    _box_drop(child->parent, child, from);
  }
}

void _ref_free(Ref(void)* ref) {
  free(ref);
  
}
