#ifndef BOUND_LIFETIMESR_H
#define BOUND_LIFETIMESR_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct RefList RefList;

struct Box {
  RefList* ref;
  RefList* last_ref;
  size_t ref_count;
  size_t ref_count_self;
};
typedef struct Box Box;


#define Ref(type) Ref_##type

#define DefRef(type) struct Ref(type) { \
  type* value; \
  Box* parent; \
  size_t ref_count; \
}; \
typedef struct Ref(type) Ref(type);
// ^ DefRef

DefRef(void);

/*?
  * Box_bind(Box<*> parent, void* child)
  *
  * Binds a child (a pointer) to a parent box
  */
#define Box_bind(parent, child) _box_bind(parent, (Ref(void)*)child)
void _box_bind(Box* parent, Ref(void)* child);

/*?
  * Box_new()
  *
  * Creates a new box instance with the given value
  */
Box* Box_new();

/*?
  * Box_ref(Box<*> parent, void* child, void* from)
  *
  * Tells the box to increase its reference counter, if `from` isn't already bound to `parent`
  */
#define Box_ref(parent, child, from) _box_ref(parent, (Ref(void)*)child, (Ref(void)*)from)
void _box_ref(Box* parent, Ref(void)* child, Ref(void)* from);

/*?
  * Box_drop(Box<*> parent, void* child, void* from)
  *
  * Tells the box to decrease its reference counter, unless `from` is already bound to `parent`
  */
#define Box_drop(parent, child, from) _box_drop(parent, (Ref(void)*)child, (Ref(void)*)from)
void _box_drop(Box* parent, Ref(void)* child, Ref(void)* from);

//? Increments the self-reference counter of the parent
#define Box_ref_self(parent) parent->ref_count_self++;
void Box_drop_self(Box* parent);

//? Returns whether or not a box holds a reference
bool _box_has(Box* parent, Ref(void)* child);

//? Frees whatever was in the box, do avoid using it
void _box_free(Box* parent);

struct RefList {
  RefList* next_ref;
  Ref_void* ptr;
  Box* parent;
};

void test();

//? Creates a new Ref<type> instance
#define Ref_new(type, value) (Ref(type)*)_ref_new((void*)value)
Ref(void)* _ref_new(void* value);

//? Tells the reference that is being referenced; if it is bound, then it will pass the reference to the box, otherwise it will increase its own counter
#define Ref_ref(child, from) _ref_ref((Ref(void)*)child, (Ref(void)*)from);
bool _ref_ref(Ref(void)* child, Ref(void)* from);

//? Tells the reference that is being dereferenced; if it is bound, then it will pass the drop to the box, otherwise it will decrease its own counter
#define Ref_drop(child, from) _ref_drop((Ref(void)*)child, (Ref(void)*)from);
bool _ref_drop(Ref(void)* child, Ref(void)* from);

void _ref_free(Ref(void)* ref);

#endif // BOUND_LIFETIMESR_H
