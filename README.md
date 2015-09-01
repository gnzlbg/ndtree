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

  auto find_nn_time = time([&]() {
    for (auto&& p : points.idx()) { auto c = points.nearest_neighbor(p); }
  });


<!---
### Quick start

The data-structure itself is in `<ndtree/tree.hpp>`, the algorithms are in
`<ndtree/algorithm/...>`. In the following minimal example it is shown how to
create a custom quad-tree for 2D nearest-neighbor searches that uses a SOA
layout for the points (very WIP).

```c++
#include <ndtree/tree.hpp>
#include <ndtree/algorithm/...>

using namespace ndtree;

struct my_tree : tree<2> {
  // data: for each point we store x, y, and a value, as a
  // struct of arrays (simplified here with 3 different std::vector for exposition) 
  std::vector<double> x;
  std::vector<double> y;
  std::vector<int> value;

  // For each tree node we store here the index of the data at that node,
  // which will be used bellow in data_location(node_idx) -> data_idx
  // (for a different application we might want to store multiple data-sets
  //  within the same tree structure, want to store the data-idx only for
  // leaf nodes, or might not need this at all)
  std::vector<data_idx> data_idx;
  
  my_tree(uint_t capacity)
  : tree(capacity) // the capacity is fixed up-front
  , data_idx(capacity) // 
  {}

  // computes the geohash of a node
  location<2> location(data_idx i) {
    return i? node_location(*this, std::array<num_t, 2>(x[*i], y[*i]))
            : location<2>{};
  }

  // returns the index of the data at a given tree node
  data_idx data_location(node_idx n) {
    return data_idx[*n];
  }

  void push(double x, double y, int value) {
    auto loc = node_location(*this, x, y); // compute a geohash
    auto n_idx = insert(*this, loc, factor = 9); // insert geohash in the tree
    x.push(x);
    y.push(y);
    value.push(value);
    data_idx[node_idx] = x.size() - 1;
  }

  // this lets the tree reorder your data
  void swap(node_idx a, node_idx b) {
    using std::swap;
    swap(x[*a], x[*b]);
    swap(y[*a], y[*b]);
    swap(value[*a], value[*b]);
    tree::swap(a, b);
  }
};

int main() {
  int capacity = 10e6;
  my_tree t(capacity);

  // rng gen
  for (auto i : view::iota(0, capacity)) {
    auto value = gen();
    auto x = gen();
    auto z = gen();
    insert(t, x, y, value);
  }

  node_idx closest_neighbor;
  for (auto node : t) {
    auto tmp = find_closest_neighbor_that(t, [&](node_idx a, node_idx b) {
  
    });
    closest_neighbor = min(tmp, closest_neighbor);
  }

  return closest_neighbor;
}
```
-->

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

  - computing location hashes
  - DFS sorting
  - find node neighbors
  - TODO: find nodes in a region
  - TODO: point insertion/removal
  - TODO: closest point

<!-- Links -->
[badge.Travis]: https://travis-ci.org/gnzlbg/ndtree.svg?branch=master
[badge.Coveralls]: https://coveralls.io/repos/gnzlbg/ndtree/badge.svg
