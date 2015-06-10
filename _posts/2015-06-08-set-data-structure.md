---
layout: post
title: Considerations of a Bag Data Structure
---

I wanted to write a post about the details considered when implementing a simple data structure in C.  Here goes...

A Bag data structure stores a collection of unordered, non-unique items.  It differs from an Array (which stores ordered, non-unique items) and a Set (which stores unordered, unique items).  A Bag can be used in place of an Array where the order of the collection is not important.  For instance, a graph node's children have no specific order and could be stored in a Bag.

A Bag performs much the same as an Array except that the remove operation is constant and may change item order.  Removing the 0th element of a 1,000,000 item Array causes 999,999 items be moved.  A Bag, on the other hand, simply removes the 0th item by moving the 999,999th item into the 0th index.

Here is a simple unstructured Bag implementation using an array.

{% highlight c %}
// bag implementation over an array
int count = 0;
int items[10];

// add items
items[count++] = 1;
items[count++] = 3;
items[count++] = 5;
items[count++] = 7;
for(int i = 0; i < count; i++)
    printf("%d ", items[i]);
// 1 3 5 7

// remove item 1
items[1] = items[--count];
for(int i = 0; i < count; i++)
    printf("%d ", items[i]);
// 1 7 5

// remove item 0
items[0] = items[--count];
for(int i = 0; i < count; i++)
    printf("%d ", items[i]);
// 5 7
{% endhighlight %}

The logic for the Bag is very simple but prone to error when used manuaully.  It is prudent to create a struct and set of functions to simplify using a Bag.

{% highlight c %}
struct Bag {
    int count;
    int items[10];
};

void Bag_Init(Bag *s)
{
    s->count = 0;
}

bool Bag_Add(Bag *s, int value)
{
    if (s->count + 1 > sizeof(s->items) / sizeof(s->items[0])) {
        return false;
    }

    s->items[s->count++] = value;
    return true;
}

void Bag_Remove(Bag *s, int index)
{
    if (index >= 0 && index < s->count) {
        s->items[index] = s->items[--s->count];
    }
}


Bag s;

Bag_Init(&s);

// add items
Bag_Add(&s, 1);
Bag_Add(&s, 3);
Bag_Add(&s, 5);
Bag_Add(&s, 7);
for(int i = 0; i < s.count; i++)
    printf("%d ", s.items[i]);
// 1 3 5 7

// remove item 1
Bag_Remove(&s, 1);
for(int i = 0; i < s.count; i++)
    printf("%d ", s.items[i]);
// 1 7 5

// remove item 0
Bag_Remove(&s, 0);
for(int i = 0; i < s.count; i++)
    printf("%d ", s.items[i]);
// 5 7
{% endhighlight %}


The size of the bag is fixed at 10, if we add an 11th item it is discarded and lost.  Obviously we want to be able to specify how many items to store.

One option is have the Bag dynamically allocate the item memory for us.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int *items;
};

void Bag_Init(Bag *s, int capacity)
{
    s->capacity = capacity;
    s->count = 0;
    s->items = (int *)malloc(sizeof(int) * capacity);
}

void Bag_Free(Bag *s)
{
    free(s->items);
}

Bag s;
Bag_Init(&s, 10);
...
Bag_Free(&s);
{% endhighlight %}

This is convenient, but the user has no control over the allocation.  The Bag could take custom alloc/free hooks in Bag_Init.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int *items;
    void *(*malloc)(size_t);
    void (*free)(void *);
    void *userdata;
};


void Bag_Init(Bag *s, int capacity, void *(*customMalloc)(size_t, void *userdata), void (*customFree)(void *, void *userdata), void *userdata)
{
    s->malloc = malloc;
    s->free = free;
    if (customMalloc) {
        s->malloc = customMalloc;
    }
    if (customFree) {
        s->free = customFree;
    }
    s->userdata = userdata;

    s->capacity = capacity;
    s->count = 0;
    s->items = (int *)s->malloc(sizeof(int) * capacity, s->userdata);
}

void Bag_Free(Bag *s)
{
    s->free(s->items, s->userdata);
}


struct Buffer {
    void *data;
    int used;
    int capacity;
};

void *myAlloc(size_t size, void *userdata)
{
    Buffer *b = (Buffer *)userdata;
    void *mem = (byte *)b->data + b->used;
    b->used += size;
    return mem;
}

void myFree(void *p)
{
    // don't need to free stack memory
}

byte memory[1024];
Buffer b = {memory, 0, sizeof(memory)};

Bag s;
Bag_Init(&s, 10, myAlloc, myFree, &b);
...
Bag_Free(&s);
{% endhighlight %}

So this is better, right?  The Bag is encapsulated and can manage itself, great!  But it takes a lot of work just to tell the Bag where to allocate memory, which makes using the Bag very inconvenient.  This is supposed to be a lightweight data structure!

A better alternative is to _give_ Bag the memory to work in.  This allows the user to control the memory and the Bag only needs to handle the logic.  The Bag can use stack, heap or custom memory trivially.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int *items;
};

void Bag_Init(Bag *s, int *items, int capacity)
{
    s->capacity = capacity;
    s->count = 0;
    s->items = items;
}

int items[10];
Bag s;
Bag_Init(&s, items, 10);
...

int *items = CustomAlloc(sizeof(int) * 10);
Bag s;
Bag_Init(&s, items, 10);
...
{% endhighlight %}

But now the Bag is two discontiguous chunks of memory: the Bag structure and the items array.  Moving, copying or reallocating the Bag is a multistep process: 1) copy the Bag struct  2) alloc a new items array  3) copy the old items into the new array  4) assign the new items to the new Bag's items.

Another option is to allocate the Bag and items as a contiguous block of memory.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int items[];
};

void Bag_Init(Bag *s, int bytes)
{
    s->capacity = (bytes - sizeof(Bag)) / sizeof(int);
    s->count = 0;
}

byte memory[1024];
Bag *s = (Bag *)memory;
Bag_Init(&s, 1024);
...

Bag *s = (Bag *)malloc(1024);
Bag_Init(&s, 1024);
...
// out of space!
if(!Bag_Add(&s, i)) {
    s = (Bag *)realloc(s, 2048);
    Bag_Add(&s, i);
}
free(s);
{% endhighlight %}

Now the Bag can be trivially memcpy'd and realloc'd as a single block of data and is always internally consistent.

Finally, the Bag isn't very useful if it only works with ints.  A template can be used to parameterize the type of the items array.

{% highlight c %}
template<typename T>
struct Bag {
    int capacity;
    int count;
    T items[];
};

void Bag_Init(Bag<T> *s, int bytes)
{
    s->capacity = (bytes - sizeof(Bag<T>)) / sizeof(T);
    s->count = 0;
}

bool Bag_Add(Bag<T> *s, T value)
{
    if (s->count + 1 > s->capacity) {
        return false;
    }

    s->items[s->count++] = value;
    return true;
}

void Bag_Remove(Bag<T> *s, int index)
{
    if (index >= 0 && index < s->count) {
        s->count--;
        s->items[index] = s->items[s->count];
    }
}
{% endhighlight %}

That is the whole Bag implementation: one struct and three simple functions.

One last note, I prefer not to obscure data strucutres behind accessor functions.  In this case, I am not creating Bag_Count, Bag_Get, Bag_Set accessors because they can be accessed directly on the structure itself.

The Bag 
-------
- Does not manage it's own memory, can easily be used with any user memory
- Can be relocated, copied or reallocated as a single memory block
- Adding and removing items is one value copy and one addition
