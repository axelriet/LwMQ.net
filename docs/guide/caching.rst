#################################
LRU Caching and Key-Value Storage
#################################

LRU caching and key-value storage play supporting roles in LwMQ's
offerings, as they are often needed to implement applications
that leverage LwMQ's messaging capabilities.

Those features are designed to be used in conjunction with
the messaging subsystem. For example the in-memory LRU cache
can be used to store partial results in AI inference scenarios,
or to store often-served data with a precise expiration time.

The segmented LRU cache supports massive caching scenarios with
millions of entries under *heavy* multithreaded concurrent access,
and both caches support compression and encryption with per-entry
entropy.

Both the LRU cache and the segmented LRU cache support multi-million
retrievals per second, only subject to CPU speed and memory bandwidth
limitations, with the segmented cache taking the lead in heavily
contended scenarios, delivering lookup and retrieval rates in the tens
of millions per second on suitable hardware.

The Key-Value storage enable persistency with optional compression
at reasonably high speeds, the database being backed by virtual memory,
and offers a decent tradeoff between speed and durability.
