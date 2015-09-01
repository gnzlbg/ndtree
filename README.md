# ND-Tree <a href="https://travis-ci.org/gnzlbg/ndtree" target="_blank">![Build Status][badge.Travis]</a> <a href="https://coveralls.io/r/gnzlbg/ndtree" target="_blank">![Coverage Status][badge.Coveralls]</a>

> Data-oriented pointer-free edge based n-dimensional region octree and algorithms

**DISCLAIMER**: this library is a work in progress and woefully incomplete!

This library provides a minimal `nd`-dimensional octree implementation
(`tree<nd>`) with a relatively low memory usage (`1 + 1/2^nd` words per node),
_ok_ complexity guarantees (`log(N)` root-to-node and node-to-root traversals),
and configurable node data layout (as long as you can swap two elements), which
allows using a Struct of Arrays data layout for your node data if you want.

Geohashes are implemented externally (but used by some algorithms). You can
implement your own for different purposes (e.g. fast neighbor queries,
low-memory foot-print, ...).


### Quick start

For a quick start there is an n-dimensional nearest neighbor implementation in
the example directory. That shows the main features of the library.

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

- Thread safety: 

  - insertion/removal of nodes requires external synchronization (is not thread-safe!).
  - anything else is thread safe.
  - TODO: provide a wrapper with internal synchronization.
  
- Traversal complexities:

  - node-to-root and root-to-node traversals are `O(log(N))` where `N` is the
    number of nodes in the tree.

  - traversal to neighbor nodes are also `O(log(N))` and require one
    root-to-neighbor traversal if the location hash is known (otherwise an extra
    node-to-root traversal to compute the location hash)

- Sorting:

  - siblings are always sorted after a Morton Z-Curve
  - groups of siblings are freely sortable
    - DFS sorting is implemented.
    - TODO: implement BFS/Hilbert/... 

- Location hashes:

  - The storage of the location hash is configurable:
  - For fast neighbor searches: a location hash with `1 + nd` words of memory to
    find nodes at a particular level
  - For low memory storage:
    - TODO: a location hash with `2` words of memory for nodes at a particular level
    - TODO: a location hash with `1` word of memory for leaf nodes

- Algorithms:

  - See the algorithm subdirectory.

<!-- Links -->
[badge.Travis]: https://travis-ci.org/gnzlbg/ndtree.svg?branch=master
[badge.Coveralls]: https://coveralls.io/repos/gnzlbg/ndtree/badge.svg
