#include "nearest_neighbor.hpp"
#include <chrono>
// Define this to validate the octree solution against a brute-force O(N^2)
// algorithm. It can take a while...
// #define NEAREST_NEIGHBOR_VALIDATE

using namespace nearest_neighbor;

/// This shows how to compute nearest neighbors using the ndtree library

static constexpr int nd = 2;                   //< Number of spatial dimensions
static constexpr std::size_t no_points = 1e7;  //< Number of points

int main() {
  /// First generate random points once and use them for every benchmark
  std::vector<vec<nd>> point_array(no_points);
  {
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_real_distribution<> point_distribution(0., 1.);
    RANGES_FOR(auto&& i, ranges::view::iota(0, no_points)) {
      RANGES_FOR(auto&& d, ranges::view::iota(0, nd)) {
        point_array[i][d] = point_distribution(engine);
      }
    }
  }

  /// This function is used to time the benchmarks using <chrono>
  auto time = [](auto&& f) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::system_clock::now();
    f();
    end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end - start);
  };

  /// Create a collection of points with capacity to hold no_points
  points<nd> points(no_points);

  /// Insert all points in the collection
  auto insertion_time = time([&]() {
    for (auto&& p : point_array) { points.push(p); }
  });
  std::cout << "Inserted " << no_points << " points in "
            << insertion_time.count() << "s ("
            << std::chrono::duration_cast<std::chrono::microseconds>(
                insertion_time)
                 .count()
                / no_points
            << "mus/point)" << std::endl;

// This can be used to validate the octree solution against a O(N^2) brute force
// algorithm but this can take a while to run...
#ifdef NEAREST_NEIGHBOR_VALIDATE
  {
    for (auto&& p : points.idx()) {
      auto fnn = points.nearest_neighbor(p);
      auto bnn = points.nn_brute_force(p);
      if (fnn != bnn) {
        std::cerr << "FNN failed for " << p << "(fnn: " << fnn
                  << " != bnn: " << bnn << ")" << std::endl;
      }
    }
  }
#endif

  /// First one round of warm up
  auto warmup = time([&]() {
    for (auto&& p : points.idx()) { auto c = points.nearest_neighbor(p); }
  });

  /// Then we perform a nearest neighbor search for each node in an unsorted
  /// tree
  auto find_nn_time = time([&]() {
    for (auto&& p : points.idx()) { auto c = points.nearest_neighbor(p); }
  });

  std::cout << "Nearest neighbor search total: " << find_nn_time.count()
            << "s ("
            << std::chrono::duration_cast<std::chrono::microseconds>(
                find_nn_time)
                 .count()
                / no_points
            << "mus/point)" << std::endl;

  /// We sort the tree in DFS/Z-Morton order
  points.sort();

  /// And perform a nearest neighbor search for each node again
  ///
  /// On my computer it takes 14mus/node on an unsorted tree, and 4mus/node on a
  /// sorted tree, which is way faster.
  auto find_nn_time_s = time([&]() {
    for (auto&& p : points.idx()) { auto c = points.nearest_neighbor(p); }
  });

  std::cout << "Nearest neighbor search SORTED total: "
            << find_nn_time_s.count() << "s ("
            << std::chrono::duration_cast<std::chrono::microseconds>(
                find_nn_time_s)
                 .count()
                / no_points
            << "mus/point)" << std::endl;

  /// Done :)
  return 0;
}
