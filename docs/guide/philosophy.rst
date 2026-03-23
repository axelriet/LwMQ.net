##########
Philosophy
##########

LwMQ is a low-level library. The user has control over the
type of queues that are used to post messages, between
single-producer unbounded queues, to multi-producer queues
of various types. The multi-producer queues implement
synchronization internally and therefore can be accessed
concurrently from multiple threads without special precautions
at some performance cost.

The single-producer queues are meant to be access strictly
from one and only one thread (as they don't perform internal
synchronization) but they offer unprecedented performances
where queuing a new message only requires a few machine
instructions without taking any lock.

LwMQ is therefore flexible and offers more control than
typical libraries and subsystems, and does not force unwarranted
cost to applications that don't need a particular guarantee.

Another example of the low-level philosophy that is
pervasive through the entire design is the in-memory LRU cache,
which does not assume any particular key type.

Most existing LRU caches accept keys in form of a text string, which
in turn makes assumptions about the character type and encoding,
and the cache designers settled for a particular hash function
that they think should be suitable for most uses.

LwMQ makes no such assumptions and standardizes all keys as
128-bit (16-bytes) entities.

It is up to the application to supply the keys and decide how
to create them: random or non-random MAC-based GUIDs, hashed
ANSI, UTF-8, or Unicode strings or byte buffers hashed with whatever
hashing algorithm satisfying the application's requirements.

LwMQ provide fast functions to transform any string or arbitrary
byte buffer into a key, as well as functions that create ideal
keys the application can also use as record identity, but LwMQ 
does not make any assumption about the nature of the
data ending up serving as key. It directly exposes the
underlying native key type instead and give the application
full control over how to create those keys. Some applications
may find it advantageous to compute or create the keys in
advance and store them as part of their data. This avoids
the runtime cost of hashing the data each time a key is
needed. The drawback is it is the application's responsibility
to ensure the uniqueness of the keys and make them fit into 16 bytes.

The general philosophy is to give maximum control to the application
and focus on the low-level aspects, with a strong emphasis on
bomb-proof robustness while providing best-in-industry
performances on every aspects.

Whatever your scenario, LwMQ aims to provide multiple ways
to efficiently address your needs.

As an example, instead of opting for a turnkey "distributed cache" solution,
consider that LwMQ provides the individual building blocks that can be used
to assemble your own, tailored to your specific needs and requirements,
and optimized for your particular scenario which, with a low-contention highly
concurrent LRU backend and a fast RDMA front-end will likely
outperform any existing solution by a wide margin, augmented with the
inherent elasticity provided by queuing.

Need a *secure* cache? Flip a switch to enable fast memory encryption for tiny
entries and AES-GCM encryption for large ones. Need a *compressed* cache?
Flip another switch to enable LZ4 compression. Need a *compressed and
encrypted* cache? Flip both switches.

The cache also optionally support hardware deflate through `Intel QAT`_ for
very large payloads, if your server is equipped with the required accelerator
and drivers.

.. _Intel QAT: https://www.intel.com/content/www/us/en/architecture-and-technology/intel-quick-assist-technology-overview.html

Ease of use, best in class components that can be composed to
build your winning solution. That, in a nutshell, is the philosophy
behind LwMQ.
