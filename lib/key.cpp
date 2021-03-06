//  key.cpp -- for use with settings and groups
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "utsushi/key.hpp"

namespace utsushi {

const std::string key::separator_ = "/";

key::key (const std::string& s)
  : key_(s)
{}

key::key (const char *str)
  : key_(str)
{}

key::key ()
  : key_()
{}

bool
key::operator== (const key& k) const
{
  return key_ == k.key_;
}

bool
key::operator< (const key& k) const
{
  return key_ < k.key_;
}

key&
key::operator/= (const key& k)
{
  key_ += separator_;
  key_ += k.key_;
  return *this;
}

key::operator bool () const
{
  return !key_.empty ();
}

key::operator std::string () const
{
  return key_;
}

}       // namespace utsushi
