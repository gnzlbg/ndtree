/// \file slim.cpp Slim location tests
#include <ndtree/location/slim.hpp>
#include "test.hpp"

using namespace ndtree;

template struct ndtree::location::slim<1, uint32_t>;
template struct ndtree::location::slim<2, uint32_t>;
template struct ndtree::location::slim<3, uint32_t>;
template struct ndtree::location::slim<1, uint64_t>;
template struct ndtree::location::slim<2, uint64_t>;
template struct ndtree::location::slim<3, uint64_t>;

int main() {
  {  // 1D (32 bit)
    test_location<1, 31>(location::slim<1, uint32_t>{});
  }

  {  // 2D (32 bit)
    test_location<2, 15>(location::slim<2, uint32_t>{});
  }

  {  // 3D (32_bit)
    test_location<3, 9>(location::slim<3, uint32_t>{});
  }
  {  // 1D (64 bit)
    test_location<1, 63>(location::slim<1, uint64_t>{});
  }

  {  // 2D (64 bit)
    test_location<2, 31>(location::slim<2, uint64_t>{});
  }

  {  // 3D (64_bit)
    test_location<3, 20>(location::slim<3, uint64_t>{});
  }

  { test_location_2<location::slim>(); }

  return test::result();
}
