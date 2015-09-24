---
layout: post
title: C API Internal Access Protection
---

I like C style apis, they're simple and clean.  When I create an api I use an underscored prefix to group functions in the same api.

### C Time API
{% highlight c %}
uint64_t Time_GetTicks();
uint64_t Time_GetTickFrequency();
{% endhighlight %}

In the source file I group api data in a static struct instance named with the same prefix.

{% highlight c %}
static struct Time_ {
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeFrequency;
} Time_;
{% endhighlight %}

The struct type is declared with the same name as the instance so statics and type declarations can be accessed through the same Time\_:: namespace.  In fact, any use of the Time api starts with the same prefix:

 * Time\_::type
 * Time\_.member
 * Time_Function()

This allows trivial api renaming with a simple find and replace of "Time\_".  The underscored name also avoids conflicting with any user facing typename that might be returned by the api.  A File\_ api might want to return a File pointer for example.

Inside the api functions the struct is always used explicitly.

{% highlight c %}
static void Time_Initialize()
{
	QueryPerformanceCounter(&Time_.timeStart);
	QueryPerformanceFrequency(&Time_.timeFrequency);
}

uint64_t Time_GetTicks()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return now.QuadPart - Time_.timeStart.QuadPart;
}

uint64_t Time_GetTickFrequency()
{
	return Time_.timeFrequency.QuadPart;
}
{% endhighlight %}

This organization is clean and simple and the data is hidden inside the translation unit.  Pretty good, but it would be nice to protect the api data from being used outside the api functions inside the same translation unit.  This is especially handy for unity builds where everything is in the same translation unit.

The adjustment is simple: 1) make members private 2) make api functions friends

I prefer to use a class instead of a struct so members default to private without the noise of an extra private: statement.  Friend declarations simply follow the data declarations in the class.  This has a nice effect of grouping everything in the api but with the downside of needing to redeclare the functions an extra time as friends.

### Access Protection
{% highlight c %}
static class Time_ {
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeFrequency;

	friend static void Time_Initialize();
	friend uint64_t Time_GetTicks();
	friend uint64_t Time_GetTickFrequency();
} Time_;

static void Time_Initialize()
{
	QueryPerformanceCounter(&Time_.timeStart);
	QueryPerformanceFrequency(&Time_.timeFrequency);
}

uint64_t Time_GetTicks()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return now.QuadPart - Time_.timeStart.QuadPart;
}

uint64_t Time_GetTickFrequency()
{
	return Time_.timeFrequency.QuadPart;
}
{% endhighlight %}

The best part is that this has no effect on the organization of the rest of the C code.  You can add or remove data protection simply by changing the struct declaration.
