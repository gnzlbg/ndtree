#include "nearest_neighbor.hpp"
#include <chrono>
//#define NEAREST_NEIGHBOR_VALIDATE

using namespace nearest_neighbor;

static constexpr int nd = 2;
static constexpr std::size_t no_points = 1e7;

int main() {
  /// Generate random points
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

  // Time function f
  auto time = [](auto&& f) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::system_clock::now();
    f();
    end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end - start);
  };

  points<nd> points(no_points);

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

#ifdef NEAREST_NEIGHBOR_VALIDATE
  {  // validate against O(N^2) brute force, can take a while...
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

  auto warmup = time([&]() {
    for (auto&& p : points.idx()) { auto c = points.nearest_neighbor(p); }
  });

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

  points.sort();

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

  return 0;
}
