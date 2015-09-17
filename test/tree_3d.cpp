/// \file tree_3d.cpp
#include <fstream>
#include "test.hpp"
#include "tree.hpp"
#include <ndtree/algorithm/dfs_sort.hpp>
#include <ndtree/algorithm/node_location.hpp>
#include <ndtree/location/slim.hpp>

using namespace test;

/// Explicit instantiate it
template struct ndtree::tree<3>;

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<3>>().children(node_idx{}));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(std::declval<tree<3>>()());

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(tree<3>::child_positions());

struct uniform_tree {
  std::vector<node> nodes{
   {idx{0}, lvl{0}, pn{i}, cs{1, 2, 3, 4, 5, 6, 7, 8}, pip{}, fn{is(6)},
    en{is(12)}, cn{is(8)}, an{}},
   // level 1:
   {idx{1}, lvl{1}, pn{0}, cs{9, 10, 11, 12, 13, 14, 15, 16}, pip{0},
    fn{i, 2, i, 3, i, 5}, en{i, i, i, 4, i, i, i, i, i, 6, i, 7},
    cn{i, i, i, i, i, i, i, 8}},
   {idx{2}, lvl{1}, pn{0}, cs{17, 18, 19, 20, 21, 22, 23, 24}, pip{1},
    fn{1, i, i, 4, i, 6}, en{i, i, 3, i, i, i, i, i, 5, i, i, 8},
    cn{i, i, i, i, i, i, 7, i}},
   {idx{3}, lvl{1}, pn{0}, cs{25, 26, 27, 28, 29, 30, 31, 32}, pip{2},
    fn{i, 4, 1, i, i, 7}, en{i, 2, i, i, i, i, i, i, i, 8, 5, i},
    cn{i, i, i, i, i, 6, i, i}},
   {idx{4}, lvl{1}, pn{0}, cs{33, 34, 35, 36, 37, 38, 39, 40}, pip{3},
    fn{3, i, 2, i, i, 8}, en{1, i, i, i, i, i, i, i, 7, i, 6, i},
    cn{i, i, i, i, 5, i, i, i}},
   //
   {idx{5}, lvl{1}, pn{0}, cs{41, 42, 43, 44, 45, 46, 47, 48}, pip{4},
    fn{i, 6, i, 7, 1, i}, en{i, i, i, 8, i, 2, i, 3, i, i, i, i},
    cn{i, i, i, 4, i, i, i, i}},
   {idx{6}, lvl{1}, pn{0}, cs{49, 50, 51, 52, 53, 54, 55, 56}, pip{5},
    fn{5, i, i, 8, 2, i}, en{i, i, 7, i, 1, i, i, 4, i, i, i, i},
    cn{i, i, 3, i, i, i, i, i}},
   {idx{7}, lvl{1}, pn{0}, cs{57, 58, 59, 60, 61, 62, 63, 64}, pip{6},
    fn{i, 8, 5, i, 3, i}, en{i, 6, i, i, i, 4, 1, i, i, i, i, i},
    cn{i, 2, i, i, i, i, i, i}},
   {idx{8}, lvl{1}, pn{0}, cs{65, 66, 67, 68, 69, 70, 71, 72}, pip{7},
    fn{7, i, 6, i, 4, i}, en{5, i, i, i, 3, i, 2, i, i, i, i, i},
    cn{1, i, i, i, i, i, i, i}},
   // level 2:
   {idx{9}, lvl{2}, pn{1}, cs{}, pip{0, 0}, fn{i, 10, i, 11, i, 13},
    en{i, i, i, 12, i, i, i, i, i, 14, i, 15}, cn{i, i, i, i, i, i, i, 16}},
   {idx{10}, lvl{2}, pn{1}, cs{}, pip{0, 1}, fn{9, 17, i, 12, i, 14},
    en{i, i, 11, 19, i, i, i, i, 13, 21, i, 16}, cn{i, i, i, i, i, i, 15, 23}},
   {idx{11}, lvl{2}, pn{1}, cs{}, pip{0, 2}, fn{i, 12, 9, 25, i, 15},
    en{i, 10, i, 26, i, i, i, i, i, 16, 13, 29}, cn{i, i, i, i, i, 14, i, 30}},
   {idx{12}, lvl{2}, pn{1}, cs{}, pip{0, 3}, fn{11, 19, 10, 26, i, 16},
    en{9, 17, 25, 33, i, i, i, i, 15, 23, 14, 30},
    cn{i, i, i, i, 13, 21, 29, 37}},
   {idx{13}, lvl{2}, pn{1}, cs{}, pip{0, 4}, fn{i, 14, i, 15, 9, 41},
    en{i, i, i, 16, i, 10, i, 11, i, 42, i, 43}, cn{i, i, i, 12, i, i, i, 44}},
   {idx{14}, lvl{2}, pn{1}, cs{}, pip{0, 5}, fn{13, 21, i, 16, 10, 42},
    en{i, i, 15, 23, 9, 17, i, 12, 41, 49, i, 44},
    cn{i, i, 11, 19, i, i, 43, 51}},
   {idx{15}, lvl{2}, pn{1}, cs{}, pip{0, 6}, fn{i, 16, 13, 29, 11, 43},
    en{i, 14, i, 30, i, 12, 9, 25, i, 44, 41, 57},
    cn{i, 10, i, 26, i, 42, i, 58}},
   {idx{16}, lvl{2}, pn{1}, cs{}, pip{0, 7}, fn{15, 23, 14, 30, 12, 44},
    en{13, 21, 29, 37, 11, 19, 10, 26, 43, 51, 42, 58},
    cn{9, 17, 25, 33, 41, 49, 57, 65}},
   //
   {idx{17}, lvl{2}, pn{2}, cs{}, pip{1, 0}, fn{10, 18, i, 19, i, 21},
    en{i, i, 12, 20, i, i, i, i, 14, 22, i, 23}, cn{i, i, i, i, i, i, 16, 24}},
   {idx{18}, lvl{2}, pn{2}, cs{}, pip{1, 1}, fn{17, i, i, 20, i, 22},
    en{i, i, 19, i, i, i, i, i, 21, i, i, 24}, cn{i, i, i, i, i, i, 23, i}},
   {idx{19}, lvl{2}, pn{2}, cs{}, pip{1, 2}, fn{12, 20, 17, 33, i, 23},
    en{10, 18, 26, 34, i, i, i, i, 16, 24, 21, 37},
    cn{i, i, i, i, 14, 22, 30, 38}},
   {idx{20}, lvl{2}, pn{2}, cs{}, pip{1, 3}, fn{19, i, 18, 34, i, 24},
    en{17, i, 33, i, i, i, i, i, 23, i, 22, 38}, cn{i, i, i, i, 21, i, 37, i}},
   {idx{21}, lvl{2}, pn{2}, cs{}, pip{1, 4}, fn{14, 22, i, 23, 17, 49},
    en{i, i, 16, 24, 10, 18, i, 19, 42, 50, i, 51},
    cn{i, i, 12, 20, i, i, 44, 52}},
   {idx{22}, lvl{2}, pn{2}, cs{}, pip{1, 5}, fn{21, i, i, 24, 18, 50},
    en{i, i, 23, i, 17, i, i, 20, 49, i, i, 52}, cn{i, i, 19, i, i, i, 51, i}},
   {idx{23}, lvl{2}, pn{2}, cs{}, pip{1, 6}, fn{16, 24, 21, 37, 19, 51},
    en{14, 22, 30, 38, 12, 20, 17, 33, 44, 52, 49, 65},
    cn{10, 18, 26, 34, 42, 50, 58, 66}},
   {idx{24}, lvl{2}, pn{2}, cs{}, pip{1, 7}, fn{23, i, 22, 38, 20, 52},
    en{21, i, 37, i, 19, i, 18, 34, 51, i, 50, 66},
    cn{17, i, 33, i, 49, i, 65, i}},
   //
   {idx{25}, lvl{2}, pn{3}, cs{}, pip{2, 0}},
   {idx{26}, lvl{2}, pn{3}, cs{}, pip{2, 1}},
   {idx{27}, lvl{2}, pn{3}, cs{}, pip{2, 2}},
   {idx{28}, lvl{2}, pn{3}, cs{}, pip{2, 3}},
   {idx{29}, lvl{2}, pn{3}, cs{}, pip{2, 4}},
   {idx{30}, lvl{2}, pn{3}, cs{}, pip{2, 5}, fn{29, 37, 16, 32, 26, 58},
    en{15, 23, 31, 39, 25, 33, 12, 28, 57, 65, 44, 60},
    cn{11, 19, 27, 35, 43, 51, 59, 67}},
   {idx{31}, lvl{2}, pn{3}, cs{}, pip{2, 6}},
   {idx{32}, lvl{2}, pn{3}, cs{}, pip{2, 7}},
   //
   {idx{33}, lvl{2}, pn{4}, cs{}, pip{3, 0}},
   {idx{34}, lvl{2}, pn{4}, cs{}, pip{3, 1}},
   {idx{35}, lvl{2}, pn{4}, cs{}, pip{3, 2}},
   {idx{36}, lvl{2}, pn{4}, cs{}, pip{3, 3}},
   {idx{37}, lvl{2}, pn{4}, cs{}, pip{3, 4}, fn{30, 38, 23, 39, 33, 65},
    en{16, 24, 32, 40, 26, 34, 19, 35, 58, 66, 51, 67},
    cn{12, 20, 28, 36, 44, 52, 60, 68}},
   {idx{38}, lvl{2}, pn{4}, cs{}, pip{3, 5}},
   {idx{39}, lvl{2}, pn{4}, cs{}, pip{3, 6}},
   {idx{40}, lvl{2}, pn{4}, cs{}, pip{3, 7}},
   //
   {idx{41}, lvl{2}, pn{5}, cs{}, pip{4, 0}, fn{i, 42, i, 43, 13, 45},
    en{i, i, i, 44, i, 14, i, 15, i, 46, i, 47}, cn{i, i, i, 16, i, i, i, 48}},
   {idx{42}, lvl{2}, pn{5}, cs{}, pip{4, 1}},
   {idx{43}, lvl{2}, pn{5}, cs{}, pip{4, 2}},
   {idx{44}, lvl{2}, pn{5}, cs{}, pip{4, 3}, fn{43, 51, 42, 58, 16, 48},
    en{41, 49, 57, 65, 15, 23, 14, 30, 47, 55, 46, 62},
    cn{13, 21, 29, 37, 45, 53, 61, 69}},
   {idx{45}, lvl{2}, pn{5}, cs{}, pip{4, 4}, fn{i, 46, i, 47, 41, i},
    en{i, i, i, 48, i, 42, i, 43, i, i, i, i}, cn{i, i, i, 44, i, i, i, i}},
   {idx{46}, lvl{2}, pn{5}, cs{}, pip{4, 5}},
   {idx{47}, lvl{2}, pn{5}, cs{}, pip{4, 6}},
   {idx{48}, lvl{2}, pn{5}, cs{}, pip{4, 7}},
   //
   {idx{49}, lvl{2}, pn{6}, cs{}, pip{5, 0}},
   {idx{50}, lvl{2}, pn{6}, cs{}, pip{5, 1}, fn{49, i, i, 52, 22, 54},
    en{i, i, 51, i, 21, i, i, 24, 53, i, i, 56}, cn{i, i, 23, i, i, i, 55, i}},
   {idx{51}, lvl{2}, pn{6}, cs{}, pip{5, 2}, fn{44, 52, 49, 65, 23, 55},
    en{42, 50, 58, 66, 16, 24, 21, 37, 48, 56, 53, 69},
    cn{14, 22, 30, 38, 46, 54, 62, 70}},
   {idx{52}, lvl{2}, pn{6}, cs{}, pip{5, 3}},
   {idx{53}, lvl{2}, pn{6}, cs{}, pip{5, 4}},
   {idx{54}, lvl{2}, pn{6}, cs{}, pip{5, 5}, fn{53, i, i, 56, 50, i},
    en{i, i, 55, i, 49, i, i, 52, i, i, i, i}, cn{i, i, 51, i, i, i, i, i}},
   {idx{55}, lvl{2}, pn{6}, cs{}, pip{5, 6}},
   {idx{56}, lvl{2}, pn{6}, cs{}, pip{5, 7}},
   //
   {idx{57}, lvl{2}, pn{7}, cs{}, pip{6, 0}},
   {idx{58}, lvl{2}, pn{7}, cs{}, pip{6, 1}, fn{57, 65, 44, 60, 30, 62},
    en{43, 51, 59, 67, 29, 37, 16, 32, 61, 69, 48, 64},
    cn{15, 23, 31, 39, 47, 55, 63, 71}},
   {idx{59}, lvl{2}, pn{7}, cs{}, pip{6, 2}, fn{i, 60, 57, i, 31, 63},
    en{i, 58, i, i, i, 32, 29, i, i, 64, 61, i}, cn{i, 30, i, i, i, 62, i, i}},
   {idx{60}, lvl{2}, pn{7}, cs{}, pip{6, 3}},
   {idx{61}, lvl{2}, pn{7}, cs{}, pip{6, 4}},
   {idx{62}, lvl{2}, pn{7}, cs{}, pip{6, 5}},
   {idx{63}, lvl{2}, pn{7}, cs{}, pip{6, 6}, fn{i, 64, 61, i, 59, i},
    en{i, 62, i, i, i, 60, 57, i, i, i, i, i}, cn{i, 58, i, i, i, i, i, i}},
   {idx{64}, lvl{2}, pn{7}, cs{}, pip{6, 7}},
   //
   {idx{65}, lvl{2}, pn{8}, cs{}, pip{7, 0}, fn{58, 66, 51, 67, 37, 69},
    en{44, 52, 60, 68, 30, 38, 23, 39, 62, 70, 55, 71},
    cn{16, 24, 32, 40, 48, 56, 64, 72}},
   {idx{66}, lvl{2}, pn{8}, cs{}, pip{7, 1}},
   {idx{67}, lvl{2}, pn{8}, cs{}, pip{7, 2}},
   {idx{68}, lvl{2}, pn{8}, cs{}, pip{7, 3}, fn{67, i, 66, i, 40, 72},
    en{65, i, i, i, 39, i, 38, i, 71, i, 70, i}, cn{37, i, i, i, 69, i, i, i}},
   {idx{69}, lvl{2}, pn{8}, cs{}, pip{7, 4}},
   {idx{70}, lvl{2}, pn{8}, cs{}, pip{7, 5}},
   {idx{71}, lvl{2}, pn{8}, cs{}, pip{7, 6}},
   {idx{72}, lvl{2}, pn{8}, cs{}, pip{7, 7}, fn{71, i, 70, i, 68, i},
    en{69, i, i, i, 67, i, 66, i, i, i, i, i}, cn{65, i, i, i, i, i, i, i}},
   //
  };
};

struct tree_after_refine {
  std::vector<node> nodes{
   {idx{0}, lvl{0}, pn{i}, cs{1, 2, 3, 4, 5, 6, 7, 8}, pip{}, fn{is(6)},
    en{is(12)}, cn{is(8)}, an{}},
   // level 1:
   {idx{1}, lvl{1}, pn{0}, cs{9, 10, 11, 12, 13, 14, 15, 16}, pip{0},
    fn{i, 2, i, 3, i, 5}, en{i, i, i, 4, i, i, i, i, i, 6, i, 7},
    cn{i, i, i, i, i, i, i, 8}},
   {idx{2}, lvl{1}, pn{0}, cs{17, 18, 19, 20, 21, 22, 23, 24}, pip{1},
    fn{1, i, i, 4, i, 6}, en{i, i, 3, i, i, i, i, i, 5, i, i, 8},
    cn{i, i, i, i, i, i, 7, i}},
   {idx{3}, lvl{1}, pn{0}, cs{25, 26, 27, 28, 29, 30, 31, 32}, pip{2},
    fn{i, 4, 1, i, i, 7}, en{i, 2, i, i, i, i, i, i, i, 8, 5, i},
    cn{i, i, i, i, i, 6, i, i}},
   {idx{4}, lvl{1}, pn{0}, cs{33, 34, 35, 36, 37, 38, 39, 40}, pip{3},
    fn{3, i, 2, i, i, 8}, en{1, i, i, i, i, i, i, i, 7, i, 6, i},
    cn{i, i, i, i, 5, i, i, i}},
   //
   {idx{5}, lvl{1}, pn{0}, cs{41, 42, 43, 44, 45, 46, 47, 48}, pip{4},
    fn{i, 6, i, 7, 1, i}, en{i, i, i, 8, i, 2, i, 3, i, i, i, i},
    cn{i, i, i, 4, i, i, i, i}},
   {idx{6}, lvl{1}, pn{0}, cs{49, 50, 51, 52, 53, 54, 55, 56}, pip{5},
    fn{5, i, i, 8, 2, i}, en{i, i, 7, i, 1, i, i, 4, i, i, i, i},
    cn{i, i, 3, i, i, i, i, i}},
   {idx{7}, lvl{1}, pn{0}, cs{57, 58, 59, 60, 61, 62, 63, 64}, pip{6},
    fn{i, 8, 5, i, 3, i}, en{i, 6, i, i, i, 4, 1, i, i, i, i, i},
    cn{i, 2, i, i, i, i, i, i}},
   {idx{8}, lvl{1}, pn{0}, cs{65, 66, 67, 68, 69, 70, 71, 72}, pip{7},
    fn{7, i, 6, i, 4, i}, en{5, i, i, i, 3, i, 2, i, i, i, i, i},
    cn{1, i, i, i, i, i, i, i}},
   // level 2:
   {idx{9}, lvl{2}, pn{1}, cs{}, pip{0, 0}, fn{i, 10, i, 11, i, 13},
    en{i, i, i, 12, i, i, i, i, i, 14, i, 15}, cn{i, i, i, i, i, i, i, 16}},
   {idx{10}, lvl{2}, pn{1}, cs{}, pip{0, 1}, fn{9, 17, i, 12, i, 14},
    en{i, i, 11, 19, i, i, i, i, 13, 21, i, 16}, cn{i, i, i, i, i, i, 15, 23}},
   {idx{11}, lvl{2}, pn{1}, cs{}, pip{0, 2}, fn{i, 12, 9, 25, i, 15},
    en{i, 10, i, 26, i, i, i, i, i, 16, 13, 29}, cn{i, i, i, i, i, 14, i, 30}},
   {idx{12}, lvl{2}, pn{1}, cs{}, pip{0, 3}, fn{11, 19, 10, 26, i, 16},
    en{9, 17, 25, 33, i, i, i, i, 15, 23, 14, 30},
    cn{i, i, i, i, 13, 21, 29, 37}},
   {idx{13}, lvl{2}, pn{1}, cs{}, pip{0, 4}, fn{i, 14, i, 15, 9, 41},
    en{i, i, i, 16, i, 10, i, 11, i, 42, i, 43}, cn{i, i, i, 12, i, i, i, 44}},
   {idx{14}, lvl{2}, pn{1}, cs{}, pip{0, 5}, fn{13, 21, i, 16, 10, 42},
    en{i, i, 15, 23, 9, 17, i, 12, 41, 49, i, 44},
    cn{i, i, 11, 19, i, i, 43, 51}},
   {idx{15}, lvl{2}, pn{1}, cs{}, pip{0, 6}, fn{i, 16, 13, 29, 11, 43},
    en{i, 14, i, 30, i, 12, 9, 25, i, 44, 41, 57},
    cn{i, 10, i, 26, i, 42, i, 58}},
   {idx{16}, lvl{2}, pn{1}, cs{73, 74, 75, 76, 77, 78, 79, 80}, pip{0, 7},
    fn{15, 23, 14, 30, 12, 44},
    en{13, 21, 29, 37, 11, 19, 10, 26, 43, 51, 42, 58},
    cn{9, 17, 25, 33, 41, 49, 57, 65}},
   //
   {idx{17}, lvl{2}, pn{2}, cs{}, pip{1, 0}, fn{10, 18, i, 19, i, 21},
    en{i, i, 12, 20, i, i, i, i, 14, 22, i, 23}, cn{i, i, i, i, i, i, 16, 24}},
   {idx{18}, lvl{2}, pn{2}, cs{}, pip{1, 1}, fn{17, i, i, 20, i, 22},
    en{i, i, 19, i, i, i, i, i, 21, i, i, 24}, cn{i, i, i, i, i, i, 23, i}},
   {idx{19}, lvl{2}, pn{2}, cs{}, pip{1, 2}, fn{12, 20, 17, 33, i, 23},
    en{10, 18, 26, 34, i, i, i, i, 16, 24, 21, 37},
    cn{i, i, i, i, 14, 22, 30, 38}},
   {idx{20}, lvl{2}, pn{2}, cs{}, pip{1, 3}, fn{19, i, 18, 34, i, 24},
    en{17, i, 33, i, i, i, i, i, 23, i, 22, 38}, cn{i, i, i, i, 21, i, 37, i}},
   {idx{21}, lvl{2}, pn{2}, cs{}, pip{1, 4}, fn{14, 22, i, 23, 17, 49},
    en{i, i, 16, 24, 10, 18, i, 19, 42, 50, i, 51},
    cn{i, i, 12, 20, i, i, 44, 52}},
   {idx{22}, lvl{2}, pn{2}, cs{}, pip{1, 5}, fn{21, i, i, 24, 18, 50},
    en{i, i, 23, i, 17, i, i, 20, 49, i, i, 52}, cn{i, i, 19, i, i, i, 51, i}},
   {idx{23}, lvl{2}, pn{2}, cs{}, pip{1, 6}, fn{16, 24, 21, 37, 19, 51},
    en{14, 22, 30, 38, 12, 20, 17, 33, 44, 52, 49, 65},
    cn{10, 18, 26, 34, 42, 50, 58, 66}},
   {idx{24}, lvl{2}, pn{2}, cs{}, pip{1, 7}, fn{23, i, 22, 38, 20, 52},
    en{21, i, 37, i, 19, i, 18, 34, 51, i, 50, 66},
    cn{17, i, 33, i, 49, i, 65, i}},
   //
   {idx{25}, lvl{2}, pn{3}, cs{}, pip{2, 0}},
   {idx{26}, lvl{2}, pn{3}, cs{}, pip{2, 1}},
   {idx{27}, lvl{2}, pn{3}, cs{}, pip{2, 2}},
   {idx{28}, lvl{2}, pn{3}, cs{}, pip{2, 3}},
   {idx{29}, lvl{2}, pn{3}, cs{}, pip{2, 4}},
   {idx{30}, lvl{2}, pn{3}, cs{}, pip{2, 5}, fn{29, 37, 16, 32, 26, 58},
    en{15, 23, 31, 39, 25, 33, 12, 28, 57, 65, 44, 60},
    cn{11, 19, 27, 35, 43, 51, 59, 67}},
   {idx{31}, lvl{2}, pn{3}, cs{}, pip{2, 6}},
   {idx{32}, lvl{2}, pn{3}, cs{}, pip{2, 7}},
   //
   {idx{33}, lvl{2}, pn{4}, cs{}, pip{3, 0}},
   {idx{34}, lvl{2}, pn{4}, cs{}, pip{3, 1}},
   {idx{35}, lvl{2}, pn{4}, cs{}, pip{3, 2}},
   {idx{36}, lvl{2}, pn{4}, cs{}, pip{3, 3}},
   {idx{37}, lvl{2}, pn{4}, cs{}, pip{3, 4}, fn{30, 38, 23, 39, 33, 65},
    en{16, 24, 32, 40, 26, 34, 19, 35, 58, 66, 51, 67},
    cn{12, 20, 28, 36, 44, 52, 60, 68}},
   {idx{38}, lvl{2}, pn{4}, cs{}, pip{3, 5}},
   {idx{39}, lvl{2}, pn{4}, cs{}, pip{3, 6}},
   {idx{40}, lvl{2}, pn{4}, cs{}, pip{3, 7}},
   //
   {idx{41}, lvl{2}, pn{5}, cs{}, pip{4, 0}, fn{i, 42, i, 43, 13, 45},
    en{i, i, i, 44, i, 14, i, 15, i, 46, i, 47}, cn{i, i, i, 16, i, i, i, 48}},
   {idx{42}, lvl{2}, pn{5}, cs{}, pip{4, 1}},
   {idx{43}, lvl{2}, pn{5}, cs{}, pip{4, 2}},
   {idx{44}, lvl{2}, pn{5}, cs{}, pip{4, 3}, fn{43, 51, 42, 58, 16, 48},
    en{41, 49, 57, 65, 15, 23, 14, 30, 47, 55, 46, 62},
    cn{13, 21, 29, 37, 45, 53, 61, 69}},
   {idx{45}, lvl{2}, pn{5}, cs{}, pip{4, 4}, fn{i, 46, i, 47, 41, i},
    en{i, i, i, 48, i, 42, i, 43, i, i, i, i}, cn{i, i, i, 44, i, i, i, i}},
   {idx{46}, lvl{2}, pn{5}, cs{}, pip{4, 5}},
   {idx{47}, lvl{2}, pn{5}, cs{}, pip{4, 6}},
   {idx{48}, lvl{2}, pn{5}, cs{}, pip{4, 7}},
   //
   {idx{49}, lvl{2}, pn{6}, cs{}, pip{5, 0}},
   {idx{50}, lvl{2}, pn{6}, cs{}, pip{5, 1}, fn{49, i, i, 52, 22, 54},
    en{i, i, 51, i, 21, i, i, 24, 53, i, i, 56}, cn{i, i, 23, i, i, i, 55, i}},
   {idx{51}, lvl{2}, pn{6}, cs{}, pip{5, 2}, fn{44, 52, 49, 65, 23, 55},
    en{42, 50, 58, 66, 16, 24, 21, 37, 48, 56, 53, 69},
    cn{14, 22, 30, 38, 46, 54, 62, 70}},
   {idx{52}, lvl{2}, pn{6}, cs{}, pip{5, 3}},
   {idx{53}, lvl{2}, pn{6}, cs{}, pip{5, 4}},
   {idx{54}, lvl{2}, pn{6}, cs{}, pip{5, 5}, fn{53, i, i, 56, 50, i},
    en{i, i, 55, i, 49, i, i, 52, i, i, i, i}, cn{i, i, 51, i, i, i, i, i}},
   {idx{55}, lvl{2}, pn{6}, cs{}, pip{5, 6}},
   {idx{56}, lvl{2}, pn{6}, cs{}, pip{5, 7}},
   //
   {idx{57}, lvl{2}, pn{7}, cs{}, pip{6, 0}},
   {idx{58}, lvl{2}, pn{7}, cs{}, pip{6, 1}, fn{57, 65, 44, 60, 30, 62},
    en{43, 51, 59, 67, 29, 37, 16, 32, 61, 69, 48, 64},
    cn{15, 23, 31, 39, 47, 55, 63, 71}},
   {idx{59}, lvl{2}, pn{7}, cs{}, pip{6, 2}, fn{i, 60, 57, i, 31, 63},
    en{i, 58, i, i, i, 32, 29, i, i, 64, 61, i}, cn{i, 30, i, i, i, 62, i, i}},
   {idx{60}, lvl{2}, pn{7}, cs{}, pip{6, 3}},
   {idx{61}, lvl{2}, pn{7}, cs{}, pip{6, 4}},
   {idx{62}, lvl{2}, pn{7}, cs{}, pip{6, 5}},
   {idx{63}, lvl{2}, pn{7}, cs{}, pip{6, 6}, fn{i, 64, 61, i, 59, i},
    en{i, 62, i, i, i, 60, 57, i, i, i, i, i}, cn{i, 58, i, i, i, i, i, i}},
   {idx{64}, lvl{2}, pn{7}, cs{}, pip{6, 7}},
   //
   {idx{65}, lvl{2}, pn{8}, cs{81, 82, 83, 84, 85, 86, 87, 88}, pip{7, 0},
    fn{58, 66, 51, 67, 37, 69},
    en{44, 52, 60, 68, 30, 38, 23, 39, 62, 70, 55, 71},
    cn{16, 24, 32, 40, 48, 56, 64, 72}},
   {idx{66}, lvl{2}, pn{8}, cs{}, pip{7, 1}},
   {idx{67}, lvl{2}, pn{8}, cs{}, pip{7, 2}},
   {idx{68}, lvl{2}, pn{8}, cs{}, pip{7, 3}, fn{67, i, 66, i, 40, 72},
    en{65, i, i, i, 39, i, 38, i, 71, i, 70, i}, cn{37, i, i, i, 69, i, i, i}},
   {idx{69}, lvl{2}, pn{8}, cs{}, pip{7, 4}},
   {idx{70}, lvl{2}, pn{8}, cs{}, pip{7, 5}},
   {idx{71}, lvl{2}, pn{8}, cs{}, pip{7, 6}},
   {idx{72}, lvl{2}, pn{8}, cs{}, pip{7, 7}, fn{71, i, 70, i, 68, i},
    en{69, i, i, i, 67, i, 66, i, i, i, i, i}, cn{65, i, i, i, i, i, i, i}},
   // level: 3
   {idx{73}, lvl{3}, pn{16}, cs{}, pip{0, 7, 0}},
   {idx{74}, lvl{3}, pn{16}, cs{}, pip{0, 7, 1}},
   {idx{75}, lvl{3}, pn{16}, cs{}, pip{0, 7, 2}},
   {idx{76}, lvl{3}, pn{16}, cs{}, pip{0, 7, 3}},
   {idx{77}, lvl{3}, pn{16}, cs{}, pip{0, 7, 4}},
   {idx{78}, lvl{3}, pn{16}, cs{}, pip{0, 7, 5}},
   {idx{79}, lvl{3}, pn{16}, cs{}, pip{0, 7, 6}},
   {idx{80}, lvl{3}, pn{16}, cs{}, pip{0, 7, 7}, fn{79, i, 78, i, 76, i},
    en{77, i, i, i, 75, i, 74, i, i, i, i, i}, cn{73, i, i, i, i, i, i, 81},
    an{23, 30, 37, 44, 51, 58, 73, 74, 75, 76, 77, 78, 79, 81}},
   //
   {idx{81}, lvl{3}, pn{65}, cs{}, pip{7, 0, 0}, fn{i, 82, i, 83, i, 85},
    en{i, i, i, 84, i, i, i, i, i, 86, i, 87}, cn{80, i, i, i, i, i, i, 88},
    an{23, 30, 37, 44, 51, 58, 80, 82, 83, 84, 85, 86, 87, 88}},
   {idx{82}, lvl{3}, pn{65}, cs{}, pip{7, 0, 1}},
   {idx{83}, lvl{3}, pn{65}, cs{}, pip{7, 0, 2}},
   {idx{84}, lvl{3}, pn{65}, cs{}, pip{7, 0, 3}},
   {idx{85}, lvl{3}, pn{65}, cs{}, pip{7, 0, 4}},
   {idx{86}, lvl{3}, pn{65}, cs{}, pip{7, 0, 5}},
   {idx{87}, lvl{3}, pn{65}, cs{}, pip{7, 0, 6}},
   {idx{88}, lvl{3}, pn{65}, cs{}, pip{7, 0, 7}},
   //
  };
};

template <template <ndtree::uint_t, class...> class Loc> void test_tree() {
  {  // check construction
    tree<3> t(1);
    CHECK(t.capacity() == 1_u);
    CHECK(t.size() == 1_u);
    CHECK(!t.empty());
    CHECK(t.is_leaf(0_n));
  }

  {  // check capacity
    CHECK(tree<3>(1).capacity() == 1_u);
    CHECK(tree<3>(2).capacity() == 9_u);
    CHECK(tree<3>(3).capacity() == 9_u);
    CHECK(tree<3>(4).capacity() == 9_u);
    CHECK(tree<3>(5).capacity() == 9_u);
    CHECK(tree<3>(6).capacity() == 9_u);
    CHECK(tree<3>(7).capacity() == 9_u);
    CHECK(tree<3>(8).capacity() == 9_u);
    CHECK(tree<3>(9).capacity() == 9_u);
    CHECK(tree<3>(10).capacity() == 17_u);
    CHECK(tree<3>(11).capacity() == 17_u);
    CHECK(tree<3>(12).capacity() == 17_u);
    CHECK(tree<3>(13).capacity() == 17_u);
    CHECK(tree<3>(17).capacity() == 17_u);
    CHECK(tree<3>(18).capacity() == 25_u);
  }
  {
    tree<3> t(89);
    CHECK(t.capacity() == 89_u);
    CHECK(t.size() == 1_u);
    CHECK(t.is_leaf(0_n));
    t.refine(0_n);
    CHECK(!t.is_leaf(0_n));
    CHECK(t.size() == 9_u);

    t.refine(1_n);
    t.refine(2_n);
    t.refine(3_n);
    t.refine(4_n);
    t.refine(5_n);
    t.refine(6_n);
    t.refine(7_n);
    t.refine(8_n);
    CHECK(t.size() == 73_u);

    check_tree(t, uniform_tree{}, Loc<3>{});
    CHECK(t == uniformly_refined_tree<3>(2, 2));

    t.refine(16_n);
    t.refine(65_n);

    CHECK(t.size() == 89_u);
    check_tree(t, tree_after_refine{}, Loc<3>{});
    CHECK(t != uniformly_refined_tree<3>(2, 3));
  }

  {
    auto t = uniformly_refined_tree<3>(2, 3);
    check_tree(t, uniform_tree{}, Loc<3>{});

    t.refine(16_n);
    t.refine(65_n);

    CHECK(t.size() == 89_u);
    check_tree(t, tree_after_refine{}, Loc<3>{});
  }
}

int main() {
  test_tree<location::fast>();
  test_tree<location::slim>();

  return test::result();
}
