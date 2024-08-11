# Simple Reference Datastructures in C

This is a pile of little datastructures for reference and learning
[main.c](main.c) runs all the little functions they expose, so it's a decent place to start

## Datastructures

So, how do I learn all these things?
Let's start at the very beginning. A very good place to start.
I like to work from basic allocation up, because things we build later will leverage
some of the really simple datastructures

Have you heard of arenas? They've got a lot of hype, but they're really not all that complicated.
We'll need them for things later, so why don't we start there?

### Basic Arenas
- [arena.h](allocators/arena.h) is a very very simple allocator, with a fixed capacity

Ok, what if I want to make a list of things that grows?
There are two common ways to do it.
### The Dynamic Array
(or vector if you're a weird c++ person)

- [simple_dynarray.h](lists/simple_dynarray.h) is a very straightforward dynamic array
- [dynarray.h](lists/dynarray.h) is a more useful dynamic array that you can use for arbitrary types

### The Linked List
This is a little more clunky than our friend the dynamic array, but they have their uses

- [simple_linked_list.h](lists/simple_linked_list.h) as simple a linked list as we can get

And now that we've got the basics, what can we do with those?
### Map and Sets
- [fixed_set.h](maps/fixed_set.h) is a set for strings that can't grow
- [fixed_map.h](maps/fixed_map.h) is a basic hashmap that can't grow
- [growing_map.h](maps/growing_map.h) is a hashmap that can grow
