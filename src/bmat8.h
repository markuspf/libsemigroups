//
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

// This file contains a declaration of fast boolean matrices up to dimension 8.

#ifndef LIBSEMIGROUPS_SRC_BMAT8_H_
#define LIBSEMIGROUPS_SRC_BMAT8_H_

#include <climits>
#include <functional>
#include <iostream>
#include <random>

#include <x86intrin.h>

#include "libsemigroups-debug.h"
#include "timer.h"
#include "to_string.h"
#include "hpcombi.hpp"

namespace libsemigroups {

  //! Class for fast boolean matrices of dimension up to 8 x 8
  //!
  //! The methods for these small matrices over the boolean semiring
  //! are more optimised than the generic methods for boolean matrices.
  //! Note that all BMat8 are represented internally as an 8 x 8 matrix;
  //! any entries not defined by the user are taken to be 0. This does
  //! not affect the results of any calculations.
  //!
  //! BMat8 is a trivial class.
  class BMat8 {
   public:
    //! A default constructor.
    //!
    //! This constructor gives no guarantees on what the matrix will contain.
    BMat8() = default;

    //! A constructor.
    //!
    //! This constructor initializes a BMat8 to have rows equal to the
    //! 8 chunks, of 8 bits each, of the binary representation of \p mat.
    explicit BMat8(uint64_t mat) : _data(mat) {}

    //! A constructor.
    //!
    //! This constructor initializes a matrix where the rows of the matrix
    //! are the vectors in \p mat.
    explicit BMat8(std::vector<std::vector<bool>> const& mat);

    //! A constructor.
    //!
    //! This is the copy constructor.
    BMat8(BMat8 const&) = default;

    //! A constructor.
    //!
    //! This is the move constructor.
    BMat8(BMat8&&) = default;

    //! A constructor.
    //!
    //! This is the copy assignement constructor.
    BMat8& operator=(BMat8 const&) = default;

    //! A constructor.
    //!
    //! This is the move assignment  constructor.
    BMat8& operator=(BMat8&&) = default;

    //! A default destructor.
    ~BMat8() = default;

    //! Returns \c true if \c this equals \p that.
    //!
    //! This method checks the mathematical equality of two BMat8 objects.
    bool operator==(BMat8 const& that) const {
      return _data == that._data;
    }

    //! Returns \c true if \c this does not equal \p that
    //!
    //! This method checks the mathematical inequality of two BMat8 objects.
    bool operator!=(BMat8 const& that) const {
      return _data != that._data;
    }

    //! Returns \c true if \c this is less than \p that.
    //!
    //! This method checks whether a BMat8 objects is less than another.
    //! We order by the results of to_int() for each matrix.
    bool operator<(BMat8 const& that) const {
      return _data < that._data;
    }

    //! Returns \c true if \c this is greater than \p that.
    //!
    //! This method checks whether a BMat8 objects is greater than another.
    //! We order by the results of to_int() for each matrix.
    bool operator>(BMat8 const& that) const {
      return _data > that._data;
    }

    //! Returns the entry in the (\p i, \p j)th position.
    //!
    //! This method returns the entry in the (\p i, \p j)th position.
    //! Note that since all matrices are internally represented as 8 x 8, it
    //! is possible to access entries that you might not believe exist.
    bool operator()(size_t i, size_t j) const {
      LIBSEMIGROUPS_ASSERT(i < 8);
      LIBSEMIGROUPS_ASSERT(j < 8);
      return (_data << (8 * i + j)) >> 63;
    }

    //! Sets the (\p i, \p j)th position to \p val.
    //!
    //! This method sets the (\p i, \p j)th entry of \c this to \p val.
    //! Uses the bit twiddle for setting bits found
    //! <a href=http://graphics.stanford.edu/~seander/bithacks>here</a>.
    inline void set(size_t i, size_t j, bool val) {
      LIBSEMIGROUPS_ASSERT(i < 8);
      LIBSEMIGROUPS_ASSERT(j < 8);
      _data ^= (-val ^ _data) & BIT_MASK[8 * i + j];
    }

    //! Returns the integer representation of \c this.
    //!
    //! Returns an unsigned integer obtained by interpreting an 8 x 8
    //! BMat8 as a sequence of 64 bits (reading rows left to right,
    //! from top to bottom) and then this sequence as an unsigned int.
    inline uint64_t to_int() const {
      return _data;
    }

    //! Returns the transpose of \c this
    //!
    //! Returns the standard matrix transpose of a BMat8.
    //! Uses the technique found in Knuth AoCP Vol. 4 Fasc. 1a, p. 15.
    inline BMat8 transpose() const {
      uint64_t x = _data;
      uint64_t y = (x ^ (x >> 7)) & 0xAA00AA00AA00AA;
      x          = x ^ y ^ (y << 7);
      y          = (x ^ (x >> 14)) & 0xCCCC0000CCCC;
      x          = x ^ y ^ (y << 14);
      y          = (x ^ (x >> 28)) & 0xF0F0F0F0;
      x          = x ^ y ^ (y << 28);
      return BMat8(x);
    }

    using epu = HPCombi::epu8;
    using Vect16 = HPCombi::Vect16;
    using Perm16 = HPCombi::Perm16;
    static constexpr epu rotlow  { 7, 0, 1, 2, 3, 4, 5, 6};
    static constexpr epu rothigh { 0, 1, 2, 3, 4, 5, 6, 7,15, 8, 9,10,11,12,13,14};
    static constexpr epu rot     { 7, 0, 1, 2, 3, 4, 5, 6,15, 8, 9,10,11,12,13,14};
    static constexpr epu rot2    { 6, 7, 0, 1, 2, 3, 4, 5,14,15, 8, 9,10,11,12,13};

    inline BMat8 operator*(BMat8 const& that) const {
      epu x = _mm_set_epi64x(_data, _data);
      BMat8 tr = that.transpose();
      epu y = _mm_shuffle_epi8(_mm_set_epi64x(tr._data, tr._data), rothigh);
      epu data {};
      epu diag = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
                  0x80,0x01,0x02,0x04,0x08,0x10,0x20,0x40};
      for (int i = 0; i < 4; ++i) {
        data |= ((x & y) != epu {}) & diag;
        y    = _mm_shuffle_epi8(y, rot2);
        diag = _mm_shuffle_epi8(diag, rot2);
      }
      return BMat8(_mm_extract_epi64(data, 0) | _mm_extract_epi64(data, 1));
    }

    inline BMat8 row_space_basis() const {
      Vect16 res = _mm_set_epi64x(0, _data);
      res = res.revsorted8().remove_dups();
      Vect16 rescy = res;
      // We now compute the union of all the included different rows
      Vect16 andincl {};
      for (int i=0; i<7; i++) {
        rescy = rescy.permuted(rotlow);
        andincl.v |= (rescy.v | res.v) == res.v ? rescy.v : epu {};
      }
      res.v = (res.v != andincl.v) ? res.v : epu {};
      return BMat8(_mm_extract_epi64(res.sorted8(), 0));
    }

    inline BMat8 col_space_basis() const {
      return transpose().row_space_basis().transpose();
    }


    //! Returns the identity BMat8
    //!
    //! This method returns the 8 x 8 BMat8 with 1s on the main diagonal.
    static inline BMat8 one() {
      return BMat8(0x8040201008040201);
    }

    //! Insertion operator
    //!
    //! This method allows BMat8 objects to be inserted into an ostringstream
    friend std::ostringstream& operator<<(std::ostringstream& os,
                                          BMat8 const&        bm) {
      uint64_t x   = bm._data;
      uint64_t pow = 1;
      pow          = pow << 63;
      for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
          if (pow & x) {
            os << "1";
          } else {
            os << "0";
          }
          x = x << 1;
        }
        os << "\n";
      }
      return os;
    }

    //! Insertion operator
    //!
    //! This method allows BMat8 objects to be inserted into a ostream.
    friend std::ostream& operator<<(std::ostream& os, BMat8 const& bm) {
      os << libsemigroups::to_string(bm);
      return os;
    }

    //! Returns a random BMat8
    //!
    //! This method returns a BMat8 chosen at random.
    static BMat8 random();

    //! Returns a random square BMat8 up to dimension \p dim.
    //!
    //! This method returns a BMat8 chosen at random, where only the
    //! top-left \p dim x \p dim entries may be non-zero.
    static BMat8 random(size_t dim);

   private:
    uint64_t                                       _data;
    static std::random_device                      _rd;
    static std::mt19937                            _gen;
    static std::uniform_int_distribution<uint64_t> _dist;
    static std::vector<uint64_t> const             ROW_MASK;
    static std::vector<uint64_t> const             COL_MASK;
    static std::vector<uint64_t> const             BIT_MASK;

  };
}  // namespace libsemigroups

namespace std {
  template <> struct hash<libsemigroups::BMat8> {
    size_t operator()(libsemigroups::BMat8 const& bm) const {
      return hash<uint64_t>()(bm.to_int());
    }
  };
}  // namespace std
#endif  // LIBSEMIGROUPS_SRC_BMAT8_H_
