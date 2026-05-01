#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "grid_service.h"

using namespace TowerDefence;

TEST_CASE("SingleIDGridService - Базовые операции") {
    SingleIDGridService grid{5,5};
    grid.add(77, {1,1});
    REQUIRE(grid.atPoint({1,1})[0] == 77);
    grid.add(66, {2,2});
    REQUIRE(grid.atPoint({2,2})[0] == 66);
    auto res = grid.getInRadius({1,1}, 3);
    REQUIRE(std::find(res.begin(), res.end(), 77) != res.end());
    REQUIRE(std::find(res.begin(), res.end(), 66) != res.end());
    grid.remove({1,1}, 77);
    grid.replace(66, {2,2}, {1,1});
    REQUIRE(grid.atPoint({2,2})[0] == 0);
    REQUIRE(grid.atPoint({1,1})[0] == 66);
}
TEST_CASE("VecIDGridService - Базовые операции") {
    VecIDGridService grid{5,5};
    grid.add(77, {1,1});
    auto x = grid.atPoint({1,1});
    REQUIRE(std::ranges::find(x, 77) != x.end());
    grid.add(66, {2,2});
    auto y = grid.atPoint({2,2});
    REQUIRE(std::ranges::find(y, 66) != y.end());
    auto res = grid.getInRadius({1,1}, 3);
    REQUIRE(std::find(res.begin(), res.end(), 77) != res.end());
    REQUIRE(std::find(res.begin(), res.end(), 66) != res.end());
    grid.remove({1,1}, 77);
    grid.replace(66, {2,2}, {1,1});
    auto z = grid.atPoint({2,2});
    auto w = grid.atPoint({1,1});
    REQUIRE(std::find(z.begin(), z.end(), 66) == z.end());
    REQUIRE(std::find(w.begin(), w.end(), 66) != w.end());
}

