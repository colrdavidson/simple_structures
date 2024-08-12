# Simple Datastructures in C

This is a pile of datastructures I've used for one project or another. They're all useful and worth knowing.
If you've heard of these mythical things, and assumed they were beyond the reach of mere mortals, don't be.
I learned them the hard way, one project at a time, and each of them gave me a new way of looking at code and data.
All of these should be in your go-to toolbox.


## Table Of Contents
- [main.c](main.c) home of the tests

### Allocators
- [Scratch Buffer](allocators/scratch.h)
- [Arena](allocators/arena.h)
- [Pool](allocators/pool.h)

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
- [Bitmap](allocators/pool.h)

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
There's many ways to write a hashmap, hashmaps are like hammers. You've got mallets, ball peens,  
claw hammers, sledgehammers, etc, etc, and all of them have their uses. Step one is knowing which end  
you hit with though. We're going to start with very simplified ones, and then work up from there.

[HashSets](maps/fixed_set.h) are one of the simplest "probabalistic" datastructures, and look a lot like  
a hashmap without the "associativity" (the "map" between a key and a value).  
They can be used for classification, for questions like "Is this token a keyword in my language?",  
or "Have I seen this person before?".

[HashMap](maps/fixed_map.h) allow you to map keys to values, which can be handy for counting the kinds of strings in text,  
"how many unique words are there in this book?", or to map between ids and data, `map["dogs"] = {"pugs", "labs", "boxers"}`.  
Hashmaps are reasonably fast for storage, and very fast for lookup, as long as your map doesn't get too full.

## Growing Pains
So, we can't stay in a tiny little box forever. How do we get more memory? What if we don't want our hashmaps  
to get full, or and don't want to have to make guesses ahead of time about how much space we'll need?  

Quick detour into the humble linked list, and then we'll find out.

### Linked Lists
The [Linked List](lists/simple_linked_list.h) is a clunky little guy, but it shows up in some very odd places.  
It is used heavily in the linux kernel, it can be made lock-free, and it even sees use as a general purpose dynamic
array in Objective-C's NSArray, although it's a bit of a memory hog for that application.
One of the big places it shines is in our next project, a growing allocator called an Arena.

### Arenas
The [Arena](allocators/arena.h) looks a bit like our Scratch Buffer, but it uses that spiffy new linked-list to append
new chunks of memory to itself, and when it's time to clean up, it has a lovely list of all the chunks it needs to free.


## A New Era of Memory Growth
Now that we have space we can grow, we can start into the real meat and potatoes of the day-to-day datastructures.

### Dynamic Arrays
(or vector if you're a weird c++ person)

We'll start with something I use *constantly*.
The [Dynamic Array](lists/simple_dynarray.h) is an array that expands as you push things into it.  
This one is a little limited, what if we need one that does more than ints?  
With a little C macro magic, we've got something we can use for real: [Dynamic Array V2](lists/dynarray.h)

### Map and Sets Part 2

Dynamic Arrays open up all sorts of doors for us.  
We can finally make ourselves a [HashMap](maps/growing_map.h) that can grow!  
Now we can handle big datasets, as many IDs and tokens as our memory can fit.

But, what if we're starting to see a lot of repeat tokens, and they're eating up all of our space?
Remember the HashSet? Time to use them for something interesting, [String Interning](maps/intern.h).
Interning is great for squashing down big repetitive lists of function names in profilers and linkers, and
for gathering all your unique strings in one place so you can compress them later.

### Neat Little Tricks

What if you want to take messages from another thread or an external device, and they don't operate exactly in sync?  
This is where a [Ring Buffer](lists/ring_buffer.h) comes to the rescue.  
These are used a ton when sending messages to and from real hardware, like buffering keyboard input for the computer  
to pick up when it gets a chance. These can grow, like ours, or be set on a fixed chunk of memory, and have the writer  
block waiting for the reader to catch up.

Interesting note, a ring buffer is actually a special form of a Queue,
and Queues are FIFO (First-In, First-Out).
> If you push in `0, 1, 2`, you'll get `0, 1, 2` out.

What if you wanted the opposite ordering? [Stacks](lists/stack.h) do that.  
Stacks are LIFO (Last-In, First-Out). 
> If you push in `0, 1, 2`, you'll get `2, 1, 0` out.

## Fragmentation Is Hell
So, if you've been following along so far, you may have noticed something important.
When you allocate a big space in our arena, any space leftover in the block before it becomes unaccessible until we clear the whole arena.

Not so great. You'll also have noticed another important tidbit...
We don't have a real free for any of the allocators we've written yet!

All that stale memory that we can't get to, is what we call "internal fragmentation".
I'm a little sick of arenas, so we'll start with an allocator that's close to ideal for internal fragmentation.  

### Solution #1 - Pools
The [Pool Allocator](allocators/pool.h)!

To manage our allocs and frees, our pool allocator converts the pointer it passes out to an index into an internal bitmap,
which uses a bit per slot to track whether the slot has been filled.

Bitmaps give you the ability to track state in a very efficient compressed form, in our case, we're using 1 bit per 64-bit element,
so we can track a 4096-byte page worth of slots in only 64 bytes. The larger the unit in our bitmap, the more efficient the tracking gets.

Our little pool allocator is solid for managing small allocations that need to be created and destroyed frequently,
which makes it a great allocator for small linked list or tree nodes.
