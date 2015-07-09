#include "test.hpp"

// I know what I'm doing TM:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#define private public
#include <htree/htree.hpp>
#pragma clang diagnostic pop

using namespace htree;

HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<1>>().nodes(siblings_idx{}));

int main() {
  {
    using tree_t = tree<1>;

    static_assert(tree_t::sibling_group(0_n) == 0_sg, "");
    static_assert(tree_t::sibling_group(1_n) == 1_sg, "");
    static_assert(tree_t::sibling_group(2_n) == 1_sg, "");
    static_assert(tree_t::sibling_group(3_n) == 2_sg, "");
    static_assert(tree_t::sibling_group(4_n) == 2_sg, "");
    static_assert(tree_t::sibling_group(5_n) == 3_sg, "");
    static_assert(tree_t::sibling_group(6_n) == 3_sg, "");
  }
  return test::result();
};
