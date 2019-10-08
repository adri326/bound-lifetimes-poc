# Bound lifetimes

## Introduction

There has been many answers to the question of memory management in programming languages.
These languages can be split in two main categories: those with and those without garbage collection.

Languages with garbage collection include Java, Python, JavaScript or LUA.
They have the advantage of leveraging the complexity of memory management, which lets the programmer focus on the functionality of their code.

Languages without garbage collection include C, C++, Rust or Assembly.
C/C++/Assembly require that the programmer manages themselves the memory in some cases, which is considered by most to be complex and moreover prone to memory leaks and segmentation faults.
Rust uses the concept of lifetimes to prevent such issues, while being as fast as a well-written C/C++ code would be.

## Garbage collection

The idea of garbage collection is to store which variables are currently referenced.
Once in a while, the garbace collector would then look at these references, and would decide whether or not it should be freed from memory.

Its main disadvantage is that it sometimes has to tackle circular references, and has to find out wether or not these cycles are still used somewhere.
This adds a lot of computational overhead to the garbage collector, which ends up taking up a lot of resources.

Because of such cycles, simple reference counting (counting how many times a variable is referenced, and freeing it only when this number reaches 0) cannot be used in every cases.
As you can see in the following diagram, both variables `A` and `B` have 1 references, while they cannot be accessed from the outside:

```js
|----------------|       |----------------|
|   references --*-----> +                |
| A: 1 reference |       | B: 1 reference |
|                + <-----*-- references   |
|----------------|       |----------------|
```

## Lifetimes

Lifetimes are a way of managing memory. It isolates ownership from referencing, and adds requirement to references.
As such, a reference stored in a structure needs to have a lifetime, which is or exceeds that of the structure itself.

While this prevents memory issues, it introduces a lot of complexity and a steep learning curve to master such lifetimes.

## Our method

We propose here what we call "bound lifetimes". The idea is to be able to use reference counting, and to get rid of the impossibility of its application with reference cycles.

Looking back at the above circular reference example, we could have solved its issue by encapsulating it in another structure, and said "as soon as nothing outside of this structure references any of these variables, then free them".

```js
                  outside references

            |             |           |
|-----------*-------------*-----------*-----------|
|           |             v           |           |
|           v                         v           |
|   |----------------|       |----------------|   |
|   |   references --*-----> +                |   |
|   |       A        |       |       B        |   |
|   |                + <-----*-- references   |   |
|   |----------------|       |----------------|   |
|       ^^^^^^^^^                 ^^^^^^^^^       |
|       reference                 reference       |
|                                                 |
|               C: n references                   |
|-------------------------------------------------|
```

This can be achieved by "binding" `A` and `B` to `C`.
Any future reference to `A` or `B` will be stored in `C`'s counter (except references from `A` to `B`), and as soon as `C`'s counter falls down to 0, we free all of this structure.

These are the simple rules that govern this memory management method:

* All variables are referenced
* When a new reference is created, tell the variable to increment its reference counter
  * If the variable is bound, then its bound variable should be told to increment its reference counter
* When a reference is dropped, tell the variable to decrement their reference counter
  * If its reference counter falls to 0, drop all of its member's references and free the memory it occupies
* A reference cycle must be bound to another variable (that is, all of the variables constituting the cycle must be bound to a variable outside of the cycle, which is not referenced by any of them)
  * The parent variable holds a reference to the child variables
  * The child variables hold a reference to the parent variable
  * The child's reference count is added to the parent's reference count

## Example

This repository is a small C runtime library demonstrating how this idea pulls itself together.

It includes a few examples using it; to run them, do:

```sh
mkdir build
cd build

cmake ..
make
```
