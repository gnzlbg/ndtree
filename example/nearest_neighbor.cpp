#include "simple_nearest_neighbor.hpp"
#include "faster_nearest_neighbor.hpp"
#include <chrono>
#include <iostream>
// Define this to validate the octree solution against a brute-force O(N^2)
// algorithm. It can take a while...
//#define NEAREST_NEIGHBOR_VALIDATE

using namespace nearest_neighbor;

/// This shows how to compute nearest neighbors using the ndtree library

static constexpr int nd = 2;                   ///< Number of spatial dimensions
static constexpr std::size_t no_points = 1e6;  ///< Number of points

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
    using namespace std::chrono;
    time_point<high_resolution_clock> b, e;
    b = high_resolution_clock::now();
    f();
    e = high_resolution_clock::now();
    return duration_cast<seconds>(e - b);
  };

  auto print = [](auto&& s, auto&& tp) {
    using namespace std::chrono;
    std::cout << s << " total: " << tp.count() << "s ("
              << duration_cast<microseconds>(tp).count() / no_points
              << "mus/point)" << std::endl;
  };

  {  // Simple

    /// Create a collection of points with capacity to hold no_points
    simple::points<nd> points(no_points);

    /// Insert all points in the collection
    std::cout << "Insertion test..." << std::endl;
    auto insertion_time = time([&]() {
      for (auto&& p : point_array) { points.push(p); }
    });
    std::cout << "(Simple) Inserted " << no_points << " points in "
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
      for (auto&& p : point_array) {
        auto fnn = points.nearest_neighbor(p);
        auto bnn = nn_brute_force(point_array, p);
        if (fnn != bnn) { std::cerr << "FNN failed!" << std::endl; }
      }
    }
#endif
    /// First one round of warm up
    auto warmup = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Simple) Nearest neighbor search warmup (unsorted)", warmup);

    /// Then we perform a nearest neighbor search for each node in an unsorted
    /// tree
    auto find_nn_time = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Simple) Nearest neighbor search (unsorted)", find_nn_time);

    /// We sort the tree in DFS/Z-Morton order
    points.sort();

    /// And perform a nearest neighbor search for each node again
    ///
    /// On my computer it takes 14mus/node on an unsorted tree, and 4mus/node on
    /// a
    /// sorted tree, which is way faster.
    auto find_nn_time_s = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Simple) Nearest neighbor search (sorted)", find_nn_time_s);

  }  // Simple

  {  // Faster

    faster::points<nd> points(no_points);

    std::cout << "Insertion test..." << std::endl;
    auto insertion_time = time([&]() {
      for (auto&& p : point_array) { points.push(p); }
    });
    std::cout << "(Faster) Inserted " << no_points << " points in "
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
      for (auto&& p : point_array) {
        auto fnn = points.nearest_neighbor(p);
        auto bnn = nn_brute_force(point_array, p);
        if (fnn != bnn) { std::cerr << "FNN failed!" << std::endl; }
      }
    }
#endif

    /// First one round of warm up
    auto warmup = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Sorted) Nearest neighbor search warmup (unsorted)", warmup);

    /// Then we perform a nearest neighbor search for each node in an unsorted
    /// tree
    auto find_nn_time = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Sorted) Nearest neighbor search (unsorted)", find_nn_time);

    /// We sort the tree in DFS/Z-Morton order
    points.sort();

    /// And perform a nearest neighbor search for each node again
    ///
    /// On my computer it takes 14mus/node on an unsorted tree, and 4mus/node on
    /// a
    /// sorted tree, which is way faster.
    auto find_nn_time_s = time([&]() {
      for (auto&& p : point_array) { points.nearest_neighbor(p); }
    });
    print("(Sorted) Nearest neighbor search (sorted)", find_nn_time_s);

  }  // Faster

  return 0;
}
