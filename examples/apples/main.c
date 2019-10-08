#include <bound_lifetimes/runtime.h>
#include <stdlib.h>
#include <stdio.h>

struct Apple {
  size_t worms;
  struct Ref(Apple)* friend;
};
typedef struct Apple Apple;

DefRef(Apple);

Ref(Apple)* get_circular_apple();

int main() {
  Ref(Apple)* apple = get_circular_apple();

  printf("The main apple has (%d) worms\n", apple->value->worms);
  printf("The apple A has (%d) worms\n", apple->value->friend->value->friend->value->worms);
  printf("The apple B has (%d) worms\n", apple->value->friend->value->worms);

  Ref_drop(apple->value->friend, apple);
  _ref_free((Ref(void)*)apple);
}

Ref(Apple)* get_circular_apple() {
  Box* box = Box_new();

  Ref(Apple)* result = Ref_new(Apple, malloc(sizeof(Apple)));
  result->value->worms = 7;

  Ref(Apple)* apple_b;

  {
    Ref(Apple)* apple_a = Ref_new(Apple, malloc(sizeof(Apple)));
    apple_a->value->worms = 3;
    Box_bind(box, apple_a); // we tell the box that we have a reference (as a variable) that we want to bind

    apple_b = Ref_new(Apple, malloc(sizeof(Apple)));
    apple_b->value->worms = 1;
    Box_bind(box, apple_b);

    apple_a->value->friend = apple_b;
    Box_ref(box, apple_a, apple_b);

    apple_b->value->friend = apple_a;
    Ref_ref(apple_b, apple_a);

    // These two calls to Box_ref should change nothing, as it all happens between the box's bound elements

    Box_drop(box, apple_a, NULL); // we drop the reference to apple_a, yet we stil have one to apple_b, and indirectly apple_a
  }

  printf("Box has apple_a: %s\n", _box_has(box, (Ref(void)*)apple_b->value->friend) ? "true" : "false");
  printf("Box has apple_b: %s\n", _box_has(box, (Ref(void)*)apple_b) ? "true" : "false");
  printf("Box's ref count: %d\n", box->ref_count);

  result->value->friend = apple_b;
  Ref_ref(apple_b, result);

  Box_drop(box, apple_b, NULL);
  Box_drop_self(box);

  return result;
}
