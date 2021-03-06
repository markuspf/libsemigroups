// libsemigroups - C++ library for semigroups and monoids
// Copyright (C) 2020 James D. Mitchell, Maria Tsalakou
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

#ifndef LIBSEMIGROUPS_DIGRAPH_HELPER_HPP_
#define LIBSEMIGROUPS_DIGRAPH_HELPER_HPP_

#include <stack>        // for stack
#include <type_traits>  // for decay
#include <utility>      // for pair
#include <vector>       // for vector

#include "libsemigroups/constants.hpp"

namespace libsemigroups {
  template <typename T>
  class ActionDigraph;

  namespace action_digraph_helper {
    //! This function returns \c true if \p ad is an acyclic digraph.
    //!
    //! \tparam T the type used as the template parameter for the ActionDigraph.
    //!
    //! \param ad the ActionDigraph object to check.
    //!
    //! \returns
    //! A value of type `bool`.
    //!
    //! \exception
    //! \no_libsemigroups_except
    //!
    //! \par Complexity
    //! \f$O(m + n)\f$ where \f$m\f$ is the number of nodes in the ActionDigraph
    //! \p ad and \f$n\f$ is the number of edges. Note that for ActionDigraph
    //! objects the number of edges is always \f$mk\f$ where \f$k\f$ is the
    //! ActionDigraph::out_degree.
    //!
    //! A digraph is *acyclic* if every directed cycle on the digraph is
    //! trivial.
    //!
    //! \par Example
    //! \code
    //! ActionDigraph<size_t> ad;
    //! ad.add_nodes(2);
    //! ad.add_to_out_degree(1);
    //! ad.add_edge(0, 1, 0);
    //! ad.add_edge(1, 0, 0);
    //! action_digraph_helper::is_acyclic(ad); // returns false
    //! \endcode
    template <typename T>
    bool is_acyclic(ActionDigraph<T> const& ad) {
      using node_type  = typename ActionDigraph<T>::node_type;
      using label_type = typename ActionDigraph<T>::label_type;

      node_type const                              N = ad.nr_nodes();
      size_t const                                 M = ad.out_degree();
      std::stack<std::pair<node_type, label_type>> stck;
      stck.emplace(0, 0);
      std::vector<uint8_t> seen(N, 0);

      for (node_type m = 0; m < N; ++m) {
        if (seen[m] == 0) {
          stck.emplace(m, 0);
          do {
            auto& p = stck.top();
            if (seen[p.first] == 2) {
              return false;
            } else if (seen[p.first] == 1 || p.second >= M) {
              seen[p.first] = 1;
              stck.pop();
              seen[stck.top().first] = 0;
              stck.top().second++;
            } else {
              node_type n;
              do {
                n = ad.unsafe_neighbor(p.first, p.second);
                p.second++;
              } while (n == libsemigroups::UNDEFINED && p.second < M);
              if (n != libsemigroups::UNDEFINED) {
                seen[p.first] = 2;
                stck.emplace(n, 0);
              }
            }
          } while (stck.size() > 1);
        }
      }
      return true;
    }
  }  // namespace action_digraph_helper
}  // namespace libsemigroups
#endif  // LIBSEMIGROUPS_DIGRAPH_HELPER_HPP_
