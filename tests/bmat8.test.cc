// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2017 Finn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <unordered_set>
#include "catch.hpp"
#include "../src/timer.h"

#include "../src/bmat8.h"

#define BMAT_REPORT false

using namespace libsemigroups;

TEST_CASE("BMat8 01: transpose", "[quick][bmat][01]") {
  BMat8 bm1(0);
  REQUIRE(bm1.transpose() == bm1);

  BMat8 bm2({{1, 1}, {0, 1}});
  REQUIRE(bm2.transpose() == BMat8({{1, 0}, {1, 1}}));

  BMat8 bm3({{0, 0, 0, 1, 0, 0, 1, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {0, 1, 1, 1, 0, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 1, 1, 1, 0, 1, 0}});

  REQUIRE(bm3.transpose()
          == BMat8({{0, 1, 0, 1, 0, 1, 0, 0},
                    {0, 1, 1, 1, 0, 1, 1, 1},
                    {0, 1, 1, 0, 1, 0, 0, 1},
                    {1, 1, 1, 1, 0, 0, 0, 1},
                    {0, 1, 0, 1, 0, 0, 0, 1},
                    {0, 1, 1, 1, 1, 0, 0, 0},
                    {1, 0, 0, 1, 1, 0, 1, 1},
                    {1, 1, 1, 1, 1, 1, 1, 0}}));
}

TEST_CASE("BMat8 02: multiplication", "[quick][bmat][02]") {
  BMat8 bm({{0, 0, 0, 1, 0, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {0, 1, 1, 1, 0, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 1, 1},
            {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 tmp = bm * bm.one();
  REQUIRE(tmp == bm);
  REQUIRE(tmp == bm * bm.one());

  tmp = bm.one() * bm;
  REQUIRE(tmp == bm);
  REQUIRE(tmp == bm.one() * bm);

  tmp = bm * BMat8(0);
  REQUIRE(tmp == BMat8(0));

  BMat8 bm2({{0, 0, 0, 1, 0, 0, 1, 1},
             {0, 0, 1, 0, 0, 1, 0, 1},
             {1, 1, 0, 0, 1, 1, 0, 1},
             {1, 1, 0, 0, 0, 0, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 0, 1, 1, 1, 1, 1},
             {0, 1, 0, 1, 0, 1, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 0}});

  tmp = bm * bm2;

  BMat8 bm3({{1, 1, 0, 1, 0, 1, 1, 1},
             {1, 1, 1, 1, 1, 1, 1, 1},
             {1, 1, 1, 1, 1, 1, 1, 1},
             {1, 1, 1, 1, 1, 1, 1, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 1, 1, 1, 0, 1, 1, 1},
             {0, 1, 1, 1, 0, 1, 1, 1},
             {1, 1, 1, 1, 1, 1, 1, 1}});

  REQUIRE(tmp == bm3);
  REQUIRE(tmp == bm * bm2);
}

TEST_CASE("BMat8 03: identity matrix", "[quick][bmat][03]") {
  BMat8 bm({{0, 1, 1, 1, 0, 1, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 1, 1},
            {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 id({{1, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 1}});

  REQUIRE(bm.one() == id);
}

TEST_CASE("BMat8 04: random", "[quick][bmat][04]") {
  for (size_t d = 1; d < 8; ++d) {
    BMat8 bm = BMat8::random(d);
    for (size_t i = d + 1; i < 8; ++i) {
      for (size_t j = 0; j < 8; ++j) {
        REQUIRE(bm(i, j) == 0);
        REQUIRE(bm(j, i) == 0);
      }
    }
  }
}

TEST_CASE("BMat8 05: call operator", "[quick][bmat][05]") {
  std::vector<std::vector<bool>> mat = {{0, 0, 0, 1, 0, 0, 1},
                                        {0, 1, 1, 1, 0, 1, 0},
                                        {1, 1, 0, 1, 1, 1, 1},
                                        {0, 0, 1, 0, 0, 1, 1},
                                        {1, 1, 0, 0, 0, 0, 0},
                                        {0, 1, 0, 0, 0, 0, 1},
                                        {0, 1, 1, 1, 1, 0, 1}};
  BMat8                          bm(mat);

  for (size_t i = 0; i < 7; ++i) {
    for (size_t j = 0; j < 7; ++j) {
      REQUIRE(static_cast<size_t>(bm(i, j)) == mat[i][j]);
    }
  }
}

TEST_CASE("BMat8 06: operator<<", "[quick][bmat][06]") {
  std::ostringstream oss;
  oss << BMat8::random();  // Does not do anything visible

  std::stringbuf buff;
  std::ostream   os(&buff);
  os << BMat8::random();  // Also does not do anything visible
}

TEST_CASE("BMat8 07: set", "[quick][bmat][07]") {
  BMat8 bm({{0, 1, 1, 1, 0, 1, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 1, 1},
            {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm2({{1, 1, 1, 1, 0, 1, 0, 1},
             {0, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm3({{1, 0, 1, 1, 0, 1, 0, 1},
             {0, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm4({{1, 0, 1, 1, 0, 1, 0, 1},
             {0, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm5({{1, 0, 1, 1, 0, 1, 0, 1},
             {0, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 1, 1, 1, 1, 0, 1, 1}});

  bm.set(0, 0, 1);
  REQUIRE(bm == bm2);

  bm.set(0, 1, false);
  REQUIRE(bm == bm3);

  bm.set(5, 6, true);
  REQUIRE(bm == bm4);

  bm.set(7, 7, true);
  REQUIRE(bm == bm5);

  for (size_t i = 0; i < 8; ++i) {
    for (size_t j = 0; j < 8; ++j) {
      bm.set(i, j, true);
    }
  }

  BMat8 ones(0xffffffffffffffff);
  REQUIRE(bm == ones);

  for (size_t i = 0; i < 8; ++i) {
    for (size_t j = 0; j < 8; ++j) {
      bm.set(i, j, false);
    }
  }

  BMat8 zeros(0);
  REQUIRE(bm == zeros);
}

TEST_CASE("BMat8 08: row space basis", "[quick][bmat][08]") {
  BMat8 bm({{0, 1, 1, 1, 0, 1, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 1, 1},
            {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm2({{1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 0, 1, 1, 1, 1, 1},
             {1, 1, 0, 0, 0, 0, 0, 1},
             {0, 1, 1, 1, 1, 0, 1, 0},
             {0, 1, 1, 1, 0, 1, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 0, 1, 0, 0, 1, 1, 1},
             {0, 0, 0, 0, 0, 0, 0, 1}});

  REQUIRE(bm.row_space_basis() == bm2.row_space_basis());

  BMat8 bm3({{1, 1, 1, 1, 0, 1, 0, 1},
             {0, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 0, 0, 1, 0, 1},
             {1, 1, 0, 0, 0, 1, 1, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {1, 0, 0, 0, 0, 1, 0, 0}});

  BMat8 bm4({{1, 1, 1, 1, 0, 1, 0, 1},
             {1, 1, 1, 0, 0, 1, 0, 1},
             {1, 0, 0, 0, 0, 1, 0, 0},
             {0, 1, 1, 1, 1, 1, 0, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0, 0}});

  REQUIRE(bm3.row_space_basis() == bm4);
  REQUIRE(bm4.row_space_basis() == bm4);

  BMat8 bm5(0xff00000000000000);

  uint64_t data = 0xffffffffffffffff;

  for (size_t i = 0; i < 7; ++i) {
    REQUIRE(BMat8(data).row_space_basis() == bm5);
    data = data >> 8;
  }

  for (size_t i = 0; i < 1000; ++i) {
    bm = BMat8::random();
    REQUIRE(bm.row_space_basis().row_space_basis() == bm.row_space_basis());
  }
}

TEST_CASE("BMat 04: col space basis", "[quick][bmat][04]") {
  BMat8 bm({{0, 1, 1, 1, 0, 1, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 0, 1},
            {1, 1, 0, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 0, 1, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 1, 1},
            {0, 1, 1, 1, 1, 0, 1, 0}});

  BMat8 bm2({{1, 1, 1, 1, 1, 0, 0, 0},
             {1, 0, 0, 0, 0, 0, 0, 0},
             {1, 1, 1, 1, 1, 1, 1, 0},
             {1, 1, 1, 1, 0, 1, 1, 1},
             {1, 1, 0, 0, 1, 0, 0, 1},
             {1, 0, 1, 0, 0, 1, 0, 0},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {0, 0, 1, 1, 1, 0, 1, 1}});

  REQUIRE(bm.col_space_basis() == bm2);

  BMat8 bm3({{1, 1, 1, 1, 0, 1, 0, 1},
             {0, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 1, 1, 1, 0, 1},
             {1, 1, 1, 0, 0, 1, 0, 1},
             {1, 1, 0, 0, 0, 1, 1, 1},
             {0, 1, 0, 0, 0, 0, 1, 1},
             {1, 0, 0, 0, 0, 1, 0, 0}});

  BMat8 bm4({{1, 1, 1, 0, 0, 0, 0, 0},
             {1, 1, 0, 1, 0, 0, 0, 0},
             {1, 1, 1, 1, 0, 0, 0, 0},
             {1, 1, 1, 1, 0, 0, 0, 0},
             {1, 0, 1, 0, 0, 0, 0, 0},
             {0, 0, 1, 0, 1, 0, 0, 0},
             {0, 0, 0, 0, 1, 0, 0, 0},
             {0, 0, 1, 0, 0, 0, 0, 0}});

  REQUIRE(bm3.col_space_basis() == bm4);

  uint64_t col = 0x8080808080808080;
  BMat8    bm5(col);

  uint64_t data = 0xffffffffffffffff;

  for (size_t i = 0; i < 7; ++i) {
    REQUIRE(BMat8(data).col_space_basis() == bm5);
    data &= ~(col >> i);
  }

  for (size_t i = 0; i < 1000; ++i) {
    bm = BMat8::random();
    REQUIRE(bm.col_space_basis().col_space_basis() == bm.col_space_basis());
  }
}


TEST_CASE("BMat8 09: row space basis", "[quick][bmat][09]") {

  Timer t;
  const std::vector<BMat8> gens =
    { BMat8({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}),
      BMat8({{0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}),
      BMat8({{0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 0}}),
      BMat8({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 0, 1}}),
      BMat8({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}) };

  int lg = 0;
  using std::unordered_set;
  unordered_set<BMat8> res;
  res.insert(BMat8::one().row_space_basis());

  std::vector<BMat8> todo, newtodo;
  todo.push_back(BMat8::one().row_space_basis());
  while (todo.size()) {
    newtodo.clear();
    lg++;
    for (auto v : todo) {
      for (auto g : gens) {
        auto el = (v * g).row_space_basis();
        if (res.insert(el).second)
          newtodo.push_back(el);
      }
    }
    std::swap(todo, newtodo);
    // std::cout << lg << ", todo = " << todo.size() << ", res = " << res.size()
    //     << ", #Bucks = " << res.bucket_count() << std::endl;
  }
  // std::cout << "res =  " << res.size() << std::endl;
  std::cout << t;
}
