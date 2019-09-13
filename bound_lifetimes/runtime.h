#ifndef BOUND_LIFETIMESR_H
#define BOUND_LIFETIMESR_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct Reference Reference;

#define Box(name) Box_##name

#define DefBox(name) struct Box(name) { \
  name* obj; \
  Reference* ref; \
  Reference* last_ref; \
  size_t ref_count; \
}; \
typedef struct Box(name) Box(name);

DefBox(void);

/*?
  * Box_bind(Box<*> parent, void* child)
  *
  * Binds a child (a pointer) to a parent box
  */
#define Box_bind(parent, child) _box_bind((Box_void*)parent, child)
void _box_bind(Box_void* parent, void* child);

/*?
  * Box_new(type name, <name> value)
  *
  * Creates a new box instance with the given value
  */
#define Box_new(name, value) ((Box(name)*) _box_new((void*) value))
Box(void)* _box_new(void* value);

/*?
  * Box_ref(Box<*> parent, void* child, void* from)
  *
  * Tells the box to increase its reference counter, if `from` isn't already bound to `parent`
  */
#define Box_ref(parent, child, from) _box_ref((Box_void*)parent, child, from)
void _box_ref(Box_void* parent, void* child, void* from);

/*?
  * Box_drop(Box<*> parent, void* child, void* from)
  *
  * Tells the box to decrease its reference counter, unless `from` is already bound to `parent`
  */
#define Box_drop(parent, child, from) _box_drop((Box_void*)parent, child, from)
void _box_drop(Box_void* parent, void* child, void* from);

bool _box_has(Box_void* parent, void* child);
void _box_free(Box_void* parent);

struct Reference {
  Reference* next_ref;
  void* ptr;
  Box(void)* parent;
};

void test();


#endif // BOUND_LIFETIMESR_H
