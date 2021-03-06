//  toggle.hpp -- bounded type for utsushi::value objects
//  Copyright (C) 2012  SEIKO EPSON CORPORATION
//
//  License: GPL-3.0+
//  Author : AVASYS CORPORATION
//
//  This file is part of the 'Utsushi' package.
//  This package is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License or, at
//  your option, any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//  You ought to have received a copy of the GNU General Public License
//  along with this package.  If not, see <http://www.gnu.org/licenses/>.

#ifndef utsushi_toggle_hpp_
#define utsushi_toggle_hpp_

#include <iosfwd>
#include <boost/operators.hpp>

namespace utsushi {

class toggle
  : private boost::equality_comparable< toggle >
{
public:
  toggle (const bool& b);
  explicit toggle ();

  bool operator== (const toggle& t) const;

  operator bool () const;

  friend
  std::ostream& operator<< (std::ostream& os, const toggle& t);
  friend
  std::istream& operator>> (std::istream& is, toggle& t);

private:
  bool toggle_;
};

}       // namespace utsushi

#endif  /* utsushi_toggle_hpp_ */
