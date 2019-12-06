.. _bridges:grpc:

gRPC Remote Procedure Calls​
############################

Another use case is to run a service that accepts HTTP requests and contains the virtual model of a memory. Then, multiple simulations can be executed in several boards/workstations, all of them sharing the virtual memory by using read_* and write_* function calls to handle HTTPS requests. dbhi/gRPC is an example of such a service. It is a prototype, and virtual streams/FIFOs are supported only. However, since it is written in golang, it is an interesting example to show that extfnc is easier to use when interfacing different languages.