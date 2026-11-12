The lockfree package contains
[lock-free data structures](http://en.wikipedia.org/wiki/Non-blocking_synchronization)
generally meant to be used in realtime or high-performance multithreaded
systems.

Currently lockfree contains 2 data structures,
[FreeList](http://www.ros.org/doc/api/lockfree/html/classlockfree_1_1FreeList.html)
and
[ObjectPool](http://www.ros.org/doc/api/lockfree/html/classlockfree_1_1ObjectPool.html).

## FreeList

The
[FreeList](http://www.ros.org/doc/api/lockfree/html/classlockfree_1_1FreeList.html)
class provides a fixed number of fixed-size blocks that you can allocate
and free from multiple threads. `FreeList` is lock-free but not
wait-free.

### Example



```cplusplus
// Create a free list with 100 128-byte blocks
FreeList free_list(128, 100);
void* mem = free_list.allocate();

if (mem)
{
  ...

  free_list.free(mem);
}
```



## ObjectPool<T>

While the `FreeList` class simply provides allocation of
specific-sized blocks of memory, the
[ObjectPool](http://www.ros.org/doc/api/lockfree/html/classlockfree_1_1ObjectPool.html)
class builds on top of that to provide allocation of specific object
types, initialized using a template object. It also allows you to
allocate either `boost::shared_ptr`s or bare pointers, depending on
your needs. `ObjectPool` is lock-free but not wait-free.

### Example



```cplusplus
struct Foo
{
  std::vector<uint32_t> bar;
};

// Create a template object.  All objects in the pool will start initialized to this value.
// In realtime systems, for example, this lets you preallocate any vectors/strings/etc.
Foo template_object;
template_object.bar.resize(100);

// Construct a pool of 5 objects
ObjectPool<Foo> pool(5, template_object);

boost::shared_ptr<Foo> inst1 = pool.allocate();
if (inst1)
{
  ...
}

Foo* inst2 = pool.allocateBare();
if (inst2)
{
  ...
  pool.freeBare(inst2);
}
```
