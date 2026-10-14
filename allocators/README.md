The allocators package is meant as a place to put different kinds of
C/C++ memory allocators. It currently only contains functions for
aligned allocation, and an STL-compatible aligned allocator.

## Aligned Allocation

General aligned allocation is done through the `alignedMalloc()` and
`alignedFree()` functions. For example:



```cplusplus
#include <allocators/aligned.h>

...

// Allocate 1000 bytes aligned to 128 bytes
void* mem = allocators::alignedMalloc(1000, 128);
...
// You must free using alignedFree
allocators::alignedFree(mem);
```



There is also an stl-compatible aligned allocator:


```cplusplus
// All allocations done by the vector will be aligned to 128 bytes
std::vector<int, allocators::AlignedAllocator<int, 128> > vec;
```


