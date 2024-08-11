# Simple Reference Datastructures in C

This is a pile of little datastructures for reference and learning.  
[main.c](main.c) runs all the little functions they expose, so it's a decent place to start

## In the Beginning There was a Little Scratch Buffer

I'm putting a little house rule in place for these, so we can really use what we're learning.  
We're only going to confine malloc to our allocator dungeon as soon as possible. Scary? Yes. I know.  
No more easy memory. It's time to get our hands dirty, and get those darn bytes ourselves.  

The [Scratch Buffer](allocators/scratch.h) is baby's first allocator. It's all you need to get started, even if you'll outgrow it fast.

### Basic Sets and Maps
What if I want to deduplicate a list of things?
Try a [Fixed HashSet](maps/fixed_set.h)

What about a map from an id to a value?
[Fixed HashMap](maps/fixed_map.h) Time


## Breaking out of the Box
Ok, we can't stay in a tiny little box forever. How do we get *more* memory?  
How do we handle so much data we can fill all of our RAM?  
We have to start with a little detour into the humble linked list.  

### Chasing Pointers
The [Linked List](lists/simple_linked_list.h) is a clunky little datastructure,  
but we're going to need it for our next project!

### Ok, Ok, I Know You Wanted Arenas
With linked lists out of the way, it's time for a more interesting allocator with some legroom.  
Our [Arena](allocators/arena.h) uses a linked list to keep track of chunks of memory as it grows,  
so it can clean itself up when you're ready to collapse it.


## Infinite Memory, Infinite New Problems
Now that we have growing memory, we can start into the real meat and potatoes of our day-to-day datastructures.

### The Dynamic Array
(or vector if you're a weird c++ person)

[Simple DynArray](lists/simple_dynarray.h) makes an ok first pass.  
It's easy to read, but not super useful unless we want to write a lot of them by hand

With a little macro magic, we've got something we can use for later, the real [DynArray](lists/dynarray.h)

### Map and Sets Part 2

Remember when we made our first maps? They were pretty limited, right?  

Now that we can grow, we can finally make ourselves a real [HashMap](maps/growing_map.h).  
HashMaps are useful for all sorts of things. I've used them to write little crappy assemblers,  
making a map full of labels-to-addresses, so I can go back and patch in jump targets later.  
I've also used them to handle keywords for compilers, using them to see if the token I just  
parsed was a keyword, and if it was, what the right response should be.  

Another great tool in the toolbox is [String Interning](maps/intern.h).  
String interning is great for deduplicating key strings as you parse them in from user data.  
I've used it to squash repetitive log entries, and to do function name deduplication in profilers and linkers.
