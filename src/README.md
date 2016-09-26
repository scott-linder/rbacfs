src
===

Working source directory for the RBAC project. 

Authors
-------

* Scott Linder
* Ryan DePrekel
* Justin Lanyon

Modules
-------

### list

A generic linked list.

### hashmap

A generic hashmap.

### parse

A lexer and parser for an RBAC definition language using Flex and Bison.

### policy

Converts the parse tree from the parse module into a structure for efficient
lookup of RBAC queries.

### fuse

Interface to the Filesystem in Userspace kernel feature for implementing the
policy for arbitrary filesystem operations.
