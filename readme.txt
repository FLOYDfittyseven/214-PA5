Jarrett Mead
Anush Pogosyan

Our implementation uses a doubly-linked list of memEntry structs to manage the heap. We added an unsigned int called "sanity_check" to the struct and populate the field with the value 0xAAAAAAAA when a new memEntry is created. This allows us to account for attempts to free pointers to memory that is within the heap that were not returned by mymalloc.

Our mymalloc function uses the best fit method to minimize memory fragmentation. When looking for a chunk of memory to return, mymalloc keeps a pointer to the memEntry with the smallest amount of memory that satisfies the user's request. This increases runtime of mymalloc, but as previously mentioned it also reduces memory fragmentation.
