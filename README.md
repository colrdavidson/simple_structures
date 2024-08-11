# Simple Reference Datastructures in C

This is a pile of little datastructures for reference and learning
[main.c](main.c) runs all the little functions they expose, so it's an decent place to start

## Datastructures
- [fixed_map.h](fixed_map.h) is a simpler hashmap that can't grow
- [growing_map.h](growing_map.h) is a slightly more complex hashmap that can, and uses a more useful hash function
- [dynarr.h](dynarr.h) is a very basic dynamic array
- [macro_dynarr.h](macro_dynarr.h) is a more useful dynamic array that you can use for arbitrary types
- [arena.h](arena.h) is a very very simple allocator, with a fixed capacity
