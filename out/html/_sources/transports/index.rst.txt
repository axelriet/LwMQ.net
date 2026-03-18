###############
LwMQ Transports
###############

LwMQ supports multiple transports, which are the underlying communication mechanisms used to send messages between processes. The transport layer is designed to be modular and extensible, allowing for different transport implementations to be used based on the needs of the application and the environment in which it is running.

.. toctree::
   :maxdepth: 1

   ipc
   rdma
   tli
   wire-format
   