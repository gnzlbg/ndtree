#include "test.hpp"
#include <ndtree/relations/tree.hpp>
#include <ndtree/relations/neighbor.hpp>

using namespace ndtree;

int main() {
  // Test no children:
  static_assert(no_children(1) == 2, "");
  static_assert(no_children(2) == 4, "");
  static_assert(no_children(3) == 8, "");

  // Test no siblings
  static_assert(no_siblings(1) == 2, "");
  static_assert(no_siblings(2) == 4, "");
  static_assert(no_siblings(3) == 8, "");

  // Test no nodes sharing face: 0D
  static_assert(no_nodes_sharing_face(0, 0) == 1, "");
  static_assert(no_nodes_sharing_face(0, 1) == 0, "");
  static_assert(no_nodes_sharing_face(0, 2) == 0, "");
  static_assert(no_nodes_sharing_face(0, 3) == 0, "");
  static_assert(no_nodes_sharing_face(0, 4) == 0, "");

  // Test no nodes sharing face: 1D
  static_assert(no_nodes_sharing_face(1, 0) == 1, "");
  static_assert(no_nodes_sharing_face(1, 1) == 2, "");
  static_assert(no_nodes_sharing_face(1, 2) == 0, "");
  static_assert(no_nodes_sharing_face(1, 3) == 0, "");
  static_assert(no_nodes_sharing_face(1, 4) == 0, "");

  // Test no nodes sharing face: 2D
  static_assert(no_nodes_sharing_face(2, 0) == 1, "");
  static_assert(no_nodes_sharing_face(2, 1) == 2, "");
  static_assert(no_nodes_sharing_face(2, 2) == 4, "");
  static_assert(no_nodes_sharing_face(2, 3) == 0, "");
  static_assert(no_nodes_sharing_face(2, 4) == 0, "");

  // Test no nodes sharing face: 3D
  static_assert(no_nodes_sharing_face(3, 0) == 1, "");
  static_assert(no_nodes_sharing_face(3, 1) == 2, "");
  static_assert(no_nodes_sharing_face(3, 2) == 4, "");
  static_assert(no_nodes_sharing_face(3, 3) == 8, "");
  static_assert(no_nodes_sharing_face(3, 4) == 0, "");

  // Test no nodes sharing face at level: 0D
  static_assert(no_nodes_sharing_face_at_level(0, 0, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(0, 0, 1) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(0, 0, 2) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(0, 1, 0) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(0, 1, 1) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(0, 1, 2) == 0, "");

  // Test no nodes sharing face at level: 1D
  static_assert(no_nodes_sharing_face_at_level(1, 0, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(1, 0, 1) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(1, 0, 2) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(1, 1, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(1, 1, 1) == 2, "");
  static_assert(no_nodes_sharing_face_at_level(1, 1, 2) == 4, "");
  static_assert(no_nodes_sharing_face_at_level(1, 2, 0) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(1, 2, 1) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(1, 2, 2) == 0, "");

  // Test no nodes sharing face at level: 2D
  static_assert(no_nodes_sharing_face_at_level(2, 0, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(2, 0, 1) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(2, 0, 2) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(2, 1, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(2, 1, 1) == 2, "");
  static_assert(no_nodes_sharing_face_at_level(2, 1, 2) == 4, "");
  static_assert(no_nodes_sharing_face_at_level(2, 1, 3) == 8, "");
  static_assert(no_nodes_sharing_face_at_level(2, 2, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(2, 2, 1) == 4, "");
  static_assert(no_nodes_sharing_face_at_level(2, 2, 2) == 16, "");
  static_assert(no_nodes_sharing_face_at_level(2, 3, 0) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(2, 3, 1) == 0, "");
  static_assert(no_nodes_sharing_face_at_level(2, 3, 2) == 0, "");

  // Test no nodes sharing face at level: 3D
  static_assert(no_nodes_sharing_face_at_level(3, 0, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 0, 1) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 0, 2) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 1, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 1, 1) == 2, "");
  static_assert(no_nodes_sharing_face_at_level(3, 1, 2) == 4, "");
  static_assert(no_nodes_sharing_face_at_level(3, 1, 3) == 8, "");
  static_assert(no_nodes_sharing_face_at_level(3, 2, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 2, 1) == 4, "");
  static_assert(no_nodes_sharing_face_at_level(3, 2, 2) == 16, "");
  static_assert(no_nodes_sharing_face_at_level(3, 3, 0) == 1, "");
  static_assert(no_nodes_sharing_face_at_level(3, 3, 1) == 8, "");
  static_assert(no_nodes_sharing_face_at_level(3, 3, 2) == 64, "");

  /// Test number of node faces: 1D
  static_assert(no_faces(1, 0) == 2, "");
  static_assert(no_faces(1, 1) == 1, "");
  static_assert(no_faces(1, 2) == 0, "");

  /// Test number of node faces: 2D
  static_assert(no_faces(2, 0) == 4, "");
  static_assert(no_faces(2, 1) == 4, "");
  static_assert(no_faces(2, 2) == 1, "");

  /// Test number of node faces: 3D
  static_assert(no_faces(3, 0) == 8, "");
  static_assert(no_faces(3, 1) == 12, "");
  static_assert(no_faces(3, 2) == 6, "");
  static_assert(no_faces(3, 3) == 1, "");

  /// Check face neighbors: 1D
  {
    constexpr auto fn = face_neighbors<1>{};
    static_assert(fn.size() == 2, "");
    CHECK(size(fn()) == 2_u);
    test::check_equal(fn[0_ni], neighbor_offset<1>{{-1}});
    test::check_equal(fn[1_ni], neighbor_offset<1>{{1}});
  }

  /// Check face neighbors: 2D
  {
    constexpr auto fn = face_neighbors<2>{};
    static_assert(fn.size() == 4, "");
    CHECK(size(fn()) == 4_u);
    test::check_equal(fn[0_ni], neighbor_offset<2>{{-1, 0}});
    test::check_equal(fn[1_ni], neighbor_offset<2>{{1, 0}});
    test::check_equal(fn[2_ni], neighbor_offset<2>{{0, -1}});
    test::check_equal(fn[3_ni], neighbor_offset<2>{{0, 1}});
  }

  /// Check face neighbors: 3D
  {
    constexpr auto fn = face_neighbors<3>{};
    static_assert(fn.size() == 6, "");
    CHECK(size(fn()) == 6_u);
    test::check_equal(fn[0_ni], neighbor_offset<3>{{-1, 0, 0}});
    test::check_equal(fn[1_ni], neighbor_offset<3>{{1, 0, 0}});
    test::check_equal(fn[2_ni], neighbor_offset<3>{{0, -1, 0}});
    test::check_equal(fn[3_ni], neighbor_offset<3>{{0, 1, 0}});
    test::check_equal(fn[4_ni], neighbor_offset<3>{{0, 0, -1}});
    test::check_equal(fn[5_ni], neighbor_offset<3>{{0, 0, 1}});
  }

  /// Check edge neighbors: 1D
  {
    constexpr auto fn = edge_neighbors<1>{};
    CHECK(size(fn()) == 0_u);
    static_assert(fn.size() == 0, "");
  }

  /// Check edge neighbors: 2D
  {
    constexpr auto fn = edge_neighbors<2>{};
    static_assert(fn.size() == 4, "");
    CHECK(size(fn()) == 4_u);
    test::check_equal(fn[0_ni], neighbor_offset<2>{{-1, -1}});
    test::check_equal(fn[1_ni], neighbor_offset<2>{{1, -1}});
    test::check_equal(fn[2_ni], neighbor_offset<2>{{-1, 1}});
    test::check_equal(fn[3_ni], neighbor_offset<2>{{1, 1}});
  }

  /// Check edge neighbors: 3D
  {
    constexpr auto fn = edge_neighbors<3>{};
    static_assert(fn.size() == 12, "");
    CHECK(size(fn()) == 12_u);
    test::check_equal(fn[0_ni], neighbor_offset<3>{{-1, -1, 0}});
    test::check_equal(fn[1_ni], neighbor_offset<3>{{1, -1, 0}});
    test::check_equal(fn[2_ni], neighbor_offset<3>{{-1, 1, 0}});
    test::check_equal(fn[3_ni], neighbor_offset<3>{{1, 1, 0}});

    test::check_equal(fn[4_ni], neighbor_offset<3>{{-1, 0, -1}});
    test::check_equal(fn[5_ni], neighbor_offset<3>{{1, 0, -1}});
    test::check_equal(fn[6_ni], neighbor_offset<3>{{0, -1, -1}});
    test::check_equal(fn[7_ni], neighbor_offset<3>{{0, 1, -1}});

    test::check_equal(fn[8_ni], neighbor_offset<3>{{-1, 0, 1}});
    test::check_equal(fn[9_ni], neighbor_offset<3>{{1, 0, 1}});
    test::check_equal(fn[10_ni], neighbor_offset<3>{{0, -1, 1}});
    test::check_equal(fn[11_ni], neighbor_offset<3>{{0, 1, 1}});
  }

  /// Check corner neighbors: 1D
  {
    constexpr auto fn = corner_neighbors<1>{};
    static_assert(fn.size() == 0, "");
    CHECK(size(fn()) == 0_u);
  }

  /// Check corner neighbors: 2D
  {
    constexpr auto fn = corner_neighbors<2>{};
    static_assert(fn.size() == 0, "");
    CHECK(size(fn()) == 0_u);
  }

  /// Check corner neighbors: 3D
  {
    constexpr auto fn = corner_neighbors<3>{};
    static_assert(fn.size() == 8, "");
    CHECK(size(fn()) == 8_u);
    test::check_equal(fn[0_ni], neighbor_offset<3>{{-1, -1, -1}});
    test::check_equal(fn[1_ni], neighbor_offset<3>{{1, -1, -1}});
    test::check_equal(fn[2_ni], neighbor_offset<3>{{-1, 1, -1}});
    test::check_equal(fn[3_ni], neighbor_offset<3>{{1, 1, -1}});
    test::check_equal(fn[4_ni], neighbor_offset<3>{{-1, -1, 1}});
    test::check_equal(fn[5_ni], neighbor_offset<3>{{1, -1, 1}});
    test::check_equal(fn[6_ni], neighbor_offset<3>{{-1, 1, 1}});
    test::check_equal(fn[7_ni], neighbor_offset<3>{{1, 1, 1}});
  }

  /// Test max no neighbors
  {
    static_assert(max_no_neighbors(1) == 2, "");
    static_assert(max_no_neighbors(2) == 12, "");
    static_assert(max_no_neighbors(3) == 56, "");
  }

  return test::result();
}
