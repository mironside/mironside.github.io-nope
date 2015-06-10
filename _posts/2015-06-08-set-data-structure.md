---
title: Considerations of a Bag Data Structure
---

I wanted to write a post about the details to consider when implementing a simple data structure in C.

A Bag data structure stores a collection of unordered, non-unique items.  It differs from an Array (which stores ordered, non-unique items) and a Set (which stores unordered, unique items).  A Bag can be used in place of an Array since the order of a collection is rarely important.  For instance, a graph node's children have no specific order and could be stored in a Bag.

The implementation of a Bag is essentially the same as an Array except the remove operation is constant and can change the item order.  In an Array removing an element requires items to be "copied down" to fill the gap left by the removed item and maintain the item order.  Removing the 0th element in an Array of 1,000,000 items causes 999,999 items be moved.  A Bag, on the other hand, has no order constraint so the 999,999th item can simply be moved into the 0th index.

A simple bag implementation using an array.

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

This exemplifies the basic idea, but this should get wrapped up in a struct for convenience...and here is where the complications lie.

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


The size of the bag is fixed at 10, if we add an 11th item it is discarded and lost.  Obviously we will need to be able to specify how many items we want to store.

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

This is convenient, but the user has no control over the allocation.  The Bag could take custom alloc/free hooks to Bag_Init.

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

So this is better, right?  The Bag is encapsulated and can manage itself, great!  But it takes a lot of work to tell the Bag where to get memory which makes using the Bag very inconvenient.  This is supposed to be a lightweight data structure!

A better alternative is to _give_ Bag the memory to work in.  This obviates Bag from allocating or freeing memory, giving the user full control.  The Bag can use stack, heap or custom memory trivially.

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

However, the Bag is now two discontiguous chunks of memory: the Bag structure and the items array.  The Bag can't be moved, copied or reallocated in a single operation.  For example, if we memcpy a Bag it's items pointer still points to the original items array.  If the original Bag memory is freed the new Bag items is invalidated.  The Bag needs to be duplicated then the items array needs to be duplicated and reassigned.

A solution is to allocate the Bag and items as a contiguous block of memory.

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
{% endhighlight %}

Now the Bag can be trivially memcpy'd and realloc'd as a single block of data and will always remain internally consistent.

Finally, using a template to parameterize the type of the items array lets us use it with any type we want.

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

One last note, I prefer not to obscure a data strucutre by hiding it entirely behind trivial accessor functions.  In this case, I am not creating Bag_Count, Bag_Get, Bag_Set functions because they can be accessed directly on the structure itself.

- Works in fixed memory
- Does not manage it's own memory
- Can be relocated, copied or reallocated
- Adding and removing items is a value copy and addition 
- The remove operation is why item order cannot be garunteed in a Bag
