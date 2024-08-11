# Simple Reference Datastructures in C

This is a pile of useful little datastructures I've written and collected over the years.

# Table Of Contents
- [main.c](main.c) home of the tests

### Allocators
- [Scratch Buffer](allocators/scratch.h)
- [Arena](allocators/arena.h)

### Lists
- [Simple Dynamic Array](lists/simple_dynarray.h)
- [Fancy Dynamic Array](lists/dynarray.h)
- [Simple Linked List](lists/simple_linked_list.h)
- [Ring Buffer / Queue](lists/ring_buffer.h)
- [Stack](lists/stack.h)

### Maps and Sets
- [Fixed HashSet](maps/fixed_set.h)
- [Fixed HashMap](maps/fixed_map.h)
- [Growing HashMap](maps/growing_map.h)
- [String Interner](maps/intern.h)

# A Random Narrative Walk

I figure it helps if you don't know where to start, to just dig in. This is about the
best order I can guess for wading through some of this stuff, but know you can always
skip around if you're not quite ready to get through the whole gauntlet today.

First up, I'm sticking a house rule in place for these, just to keep it interesting.
No libraries, and we're calling malloc as little as possible.
Gonna stick it in the allocator dungeon, lock it up and throw away the key. Scary? Yes. I know.  
No more easy memory, with a fluffy little malloc of an int here, or a float there.  
It's time to get our hands dirty and source those darn bytes ourselves.


## In the Beginning There was a Little Scratch Buffer

We'll start nice and simple with the [Scratch Buffer](allocators/scratch.h).
It's all you need to get started, even if we'll outgrow it fast.

### Basic Sets and Maps
What if we want to deduplicate a list of things, with very few variations?
Try a [Fixed HashSet](maps/fixed_set.h)

What about a map from a small set of ids to values?
[Fixed HashMap](maps/fixed_map.h) Time


## Breaking out of the Box
Ok, we can't stay in a tiny little box forever. How do we get more memory?  
How do we handle so much data we can fill all of our RAM?  
We have to start with a little detour into the humble linked list.  

### Chasing Pointers
The [Linked List](lists/simple_linked_list.h) is a clunky little guy, but we're going to need it for our next project!

### Ok, Ok, I Know You Wanted Arenas
With linked lists out of the way, it's time for allocators part 2.  
The [Arena](allocators/arena.h) uses that spiffy new linked-list idea to append new chunks of memory to itself,
and when it's time to clean up it can just walk the nodes to clean up.

## Infinite Memory, Infinite New Problems
Now that we have space we can grow, we can start into the real meat and potatoes of our day-to-day datastructures.

### The Dynamic Array
(or vector if you're a weird c++ person)

We'll start with something I use *constantly*, in as simple a form as we can.
The [DynArray](lists/simple_dynarray.h) is just an array that grows as you push things into it.
Of course, this implementation is a little limited, but it's fairly straightforward.

Can't stick with it for long though, unless we want to write a million of them.
With a little macro magic, we've got something we can use for real: [DynArray V2](lists/dynarray.h)

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

### Neat Little Tricks

What if you want to take messages from another thread or an external device, and they don't operate exactly in sync?
This is where a [Ring Buffer](lists/ring_buffer.h) comes to the rescue.
These are used a ton when sending messages to and from real hardware, like buffering keyboard input for the computer
to pick up when it gets a chance. These can grow, like ours, or be set on a fixed chunk of memory, and have the writer
block waiting for the reader to catch up.

You'll notice that the Ring Buffer is FIFO (First-In, First-Out).
If you push in `0, 1, 2`, you'll get `0, 1, 2` out.

What if you wanted the opposite? [Stacks](lists/stack.h) do that.
Stacks are LIFO (Last-In, First-Out), when you pop from a stack,
you get the most recent thing you just put in.
If you push in `0, 1, 2`, you'll get `2, 1, 0` out.
