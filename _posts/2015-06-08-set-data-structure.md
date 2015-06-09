---
title: A Bag Data Structure and Separable Memory Management
---

A bag data structure stores a collection of unordered, non-unique items.  It differs from an array (which stores ordered, non-unique items) and a set (which stores unordered, unique items).  A bag is useful when storing items where the order doesn't matter.  For instance, a graph node could use a bag to store it's child nodes.

Adding items to a bag is constat just like an array: insert the item at the end and increment the count.  However, removing an item is simpler than an array because there is no order constraint: move the last item to the index being removed and decrement the count.  For an array this would require "copying down" the remainder of the array to fill the gap left by the removed item.  Removing an item from an array is therefore linear in the number of items, while removing an item from a bag is constant.

A simple bag implementation using an array.

{% highlight c %}
// bag implementation over an array
int capacity = 10;
int count = 0;
int items[capacity];

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

The exemplifies the basic idea, but this should get wrapped up in a struct...and here is where the complications lie.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int items[10];
};

void Bag_Init(Bag *s)
{
    s->capacity = sizeof(items) / sizeof(items[0]);
    s->count = 0;
}

bool Bag_Add(Bag *s, T value)
{
    if (s->count + 1 > s->capacity) {
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


Now, let's talk about memory.  The size of the bag is fixed at 10, if we add an 11th item it is discarded and lost.  Obviously we will need to be able to specify how many items we want to store.

The first option is to have the bag dynamically allocate the item memory for us.

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
    s->items = malloc(sizeof(int) * capacity);
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

Another option is to _give_ bag the memory to work in.  This obviates Bag from managing or freeing the memory and allows us to allocate the memory however we want, even handing it local stack memory.

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
Bag_Init(&s, items, sizeof(items) / sizeof(items[0]));
...
{% endhighlight %}

Both of implementations have a downside: their memory can't be copied or moved without breaking.  For example, if we memcpy a Bag it still points at it's original items.  If the original Bag's memory is freed the new Bag items is invalidated.  To copy the Bag a to Bag b we'd need to add an extra function that "does the right thing" by copying the values of Bag a into Bag b, allocating a new buffer of items for Bag b and copying that data too.

A hybrid option is to allocate the Bag and it's items together as a contiguous block of memory outside of the structure and give it to the Bag.

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

Bag *s = (Bag *)malloc(1024);
Bag_Init(&s, bytes);
...
if(!Bag_Add(&s, i)) {
    s = (Bag *)realloc(s, 2048);
    Bag_Add(&s, i);
}
{% endhighlight %}

Because the Bag is allocated in a contiguous piece of memory we can simply realloc it when we've filled the Bag, no fixups required.

If we want to get crazier we can hide the Bag struct header so we can index the items directly.

{% highlight c %}
struct Bag {
    int capacity;
    int count;
    int items[];
};

int *Bag_Init(Bag *s, int bytes)
{
    s->capacity = (bytes - sizeof(Bag)) / sizeof(int);
    s->count = 0;
    return s->items;
}

bool Bag_Add(int *items, int value)
{
    Bag *s = (Bag *)items - 1;

    if (s->count + 1 > s->capacity) {
        return false;
    }

    s->items[s->count++] = value;
    return true;
}

void Bag_Remove(int *items, int index)
{
    Bag *s = (Bag *)items - 1;

    if (index >= 0 && index < s->count) {
        s->items[index] = s->items[--s->count];
    }
}


byte memory[sizeof(Bag) + sizeof(int) * 10];
int *s = Bag_Init(memory, bytes);

// add items
Bag_Add(s, 1);
Bag_Add(s, 3);
Bag_Add(s, 5);
Bag_Add(s, 7);
for(int i = 0; i < Bag_Count(s); i++)
    printf("%d ", s[i]);
// 1 3 5 7

// remove item 1
Bag_Remove(s, 1);
for(int i = 0; i < s.count; i++)
    printf("%d ", s[i]);
// 1 7 5

// remove item 0
Bag_Remove(s, 0);
for(int i = 0; i < Bag_Count(s); i++)
    printf("%d ", s[i]);
// 5 7
{% endhighlight %}

Finally, the Bag would be more useful if we could use it to store items of any type.  Using a simple template allows this.

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

void Bag_Resize(Bag<T> *s, int bytes)
{
    s->capacity = (bytes - sizeof(Bag<T>)) / sizeof(T);
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


- Works in fixed memory
- Does not manage it's own memory
- Can be relocated, copied or reallocated
- Removing items is a swap and decrement
- Note: the remove operation is why item order cannot be garunteed
