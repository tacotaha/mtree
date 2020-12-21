# mtree
basic merkle tree implementation for block-wise integrity verification

### Basic Idea

* partition file into 256MB blocks
* init leaf nodes with the sha256 hash of each block
* build tree from the bottom up
* dump root hash + individual block digests

### TODO

* error detection given external root hash (rebuild tree and compare?)
* fix mem leaks (maybe? runtime minimal anyway)
