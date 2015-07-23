# ND-Tree <a href="https://travis-ci.org/gnzlbg/htree" target="_blank">![Build Status][badge.Travis]</a> <a href="https://coveralls.io/r/gnzlbg/htree" target="_blank">![Coverage Status][badge.Coveralls]</a>

> An algorithms library for n-dimensional octree-like data-structures

`nd-tree` is a library that provides a minimal implementation of an
n-dimensional pointer-free region octree and a collection of algorithms that
operate on these n-dimensional data-structures.

It is partially extensible (configurable layout of the node data, user-defined
location hashes, ...) and the aim is for the algorithms provided to work on any
similar data structure that provides the same minimal interface.

### Nomenclature

- `nd`: spatial dimension of the tree.
- `root node`: largest node in the tree.
- `level(n) -> l`: `l` is the minimum number of edges between node `n` and the `root node`.
- `parent(n) -> p`: `p` is the smallest node enclosing `n`, that is, with `level(p) == level(n) - 1`.
- `child(n) -> c`: `c` is the smallest node enclosed by `n`, that is, with `level(c) == level(n) + 1`.
- a `sibling_group` is the group of all nodes sharing the same parent node.
- a `children_group` is the group of all children of a given node.

### Properties

- Memory requirements:

  - `1 + 1 / 2^nd` words of memory per node

- Internal node data layout:

  - pointer-free optimized for frequent modifications and high anisotropy
  - cache-oblivious

- Node data layout:

  - fully configurable: SOA/AOS/user-defined

- Thread safety: thread unsafe

  - insertion/removal of nodes requires external synchronization.
  - read-only operations (that is, anything else) is thread safe.
  - TODO: provide an internally synchronized wrapper over the data structure

- Traversal complexities:

  - node-to-root and root-to-node traversals are `O(log(N))` where `N` is the
    number of nodes in the tree.

  - traversal to neighbor nodes are also `O(log(N))` but if the location hash of
  the node:
    - is not know, they require two traversals,
    - is known, they require one traversal (but this requires extra storage)

- Sorting:

  - siblings are always sorted after a Morton Z-Curve
  - sibling groups (that is the block) are freely sortable
    - DFS sorting is implemented.
    - TODO: implement BFS/Hilbert/... 

- Location hashes:

  - The storage of the location hash is configurable:
  - For fast neighbor searches:
    - WIP: a location hash with `1 + nd` words of memory to find nodes at a particular level
    - TODO: a location hash with `nd` words of memory to find leaf nodes
  - For low memory storage
    - TODO: a location hash with `2` words of memory for nodes at a particular level
    - TODO: a location hash with `1` word of memory for leaf nodes

  - User-defined location hashes are supported, so if for example the number of
    levels of the tree is very small (or very large) users can provide their own
    location hashes optimized for their particular application and still use all
    the algorithms provided by the library

Algorithms:

  - computing location hashes
  - DFS sorting
  - TODO: find node neighbors
  - TODO: find nodes in a region
  - TODO: point insertion/removal
  - TODO: closest point



<!-- Links -->
[badge.Travis]: https://travis-ci.org/gnzlbg/ndtree.svg?branch=master
[badge.Coveralls]: https://coveralls.io/repos/gnzlbg/htree/badge.svg
