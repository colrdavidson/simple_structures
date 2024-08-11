# Simple Reference Datastructures in C

This is a pile of little datastructures for reference and learning
[main.c](main.c) runs all the little functions they expose, so it's a decent place to start

## In the Beginning There was a Little Scratch Buffer

I'm putting a little house rule in place for these, so we can really use what we're learning.
We're only going to confine malloc to our allocator dungeon as soon as possible. Scary? Yes. I know.
No more easy memory. It's time to get our hands dirty, and get those darn bytes ourselves.

- [scratch.h](allocators/scratch.h) is a scratch buffer, with a fixed capacity

What if I want to deduplicate a list of things?
What about a map from an id to a value?
### Basic Sets and Maps
- [fixed_set.h](maps/fixed_set.h) is a set for strings that can't grow
- [fixed_map.h](maps/fixed_map.h) is a basic hashmap that can't grow


## Breaking out of the Box
Ok, we can't stay in a tiny little box forever. How do we get *more* memory?
How do we handle so much data we can fill all of our RAM?
We have to start with a little detour into the humble linked list.

### The Linked List
This is a clunky little datastructure, but it'll get us off the ground
- [simple_linked_list.h](lists/simple_linked_list.h)

### The Growing Arena
And now it's time for a simple allocator that'll give us some legroom
[arena.h](allocators/arena.h) is an arena that uses a linked list of blocks to grow


## Infinite Memory, Infinite New Problems
Now that we have growing memory, we can start into the real meat and potatoes of our day-to-day datastructures.

### The Dynamic Array
(or vector if you're a weird c++ person)

[simple_dynarray.h](lists/simple_dynarray.h) makes a decent first pass.
It's easy to read, but not super useful unless we want to write a lot of them by hand

And with a little macro magic, we've got something we can use on a real project, [dynarray.h](lists/dynarray.h)

### Map and Sets
Don't forget, we've got a hashmap to get back to.
With our newfound memory growing potential, we can finally make ourselves a map with legs
Time to check out [growing_map.h](maps/growing_map.h)
