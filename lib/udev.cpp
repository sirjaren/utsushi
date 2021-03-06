//  udev.cpp -- OO wrapper around bits and pieces of the libudev API
//  Copyright (C) 2013-2015  SEIKO EPSON CORPORATION
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

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <boost/throw_exception.hpp>

#include "udev.hpp"

namespace udev_ {

void
get_property (struct udev_device *device,
              const std::string& name, std::string& value)
{
  struct udev_device *p (device);
  const char *rv (nullptr);

  while (p && !rv)
    {
      rv = udev_device_get_property_value (p, name.c_str ());
      p  = udev_device_get_parent (p);
    }
  if (!rv) return;

  value = rv;
}

void
get_sysattr (struct udev_device *device,
             const std::string& name, int& value,
             std::ios_base& (radix)(std::ios_base&))
{
  struct udev_device *p (device);
  const char *rv (nullptr);

  while (p && !rv)
    {
      rv = udev_device_get_sysattr_value (p, name.c_str ());
      p  = udev_device_get_parent (p);
    }
  if (!rv) return;

  if ("devpath" == name)
    {
      const char *tmp;
      tmp = strrchr (rv, '-');
      if (tmp) rv = tmp + 1;
      tmp = strrchr (rv, '.');
      if (tmp) rv = tmp + 1;
    }

  std::stringstream ss (rv);
  ss >> radix >> value;
}

namespace {

//! Handle to udev config file content, needed by all udev API calls
struct udev *ctx_(nullptr);

}       // namespace

device::device (const std::string& interface,
                const std::string& path)
{
  if (ctx_)
    {
      ctx_ = udev_ref (ctx_);
    }
  else
    {
      ctx_ = udev_new ();
      if (!ctx_)
        {
          // Looking at the udev_new() implementation, it returns NULL
          // when it fails to allocate memory for a struct udev object
          // or when it cannot fopen() the udev configuration file but
          // there is *no* way we can reliably determine what exactly
          // went wrong.
          BOOST_THROW_EXCEPTION
            (std::runtime_error ("cannot initialize libudev"));
        }
    }

  dev_ = udev_device_new_from_syspath (ctx_, path.c_str ());
  if (!dev_)
    {
      BOOST_THROW_EXCEPTION
        (std::runtime_error (strerror (ENODEV)));
    }
}

device::~device ()
{
  udev_device_unref (dev_);
  /*! \todo Release context if possible but this requires version 183
   *        or later which changed the return value of udev_unref().
   *        Versions before 183, i.e. before libudev1, return nothing.
   *
   *  ctx_ = udev_unref (ctx_);
   */
}

std::string
device::subsystem () const
{
  return udev_device_get_subsystem (dev_);
}

uint16_t
device::usb_vendor_id () const
{
  int id;
  get_sysattr (dev_, "idVendor", id);
  return id;
}

uint16_t
device::usb_product_id () const
{
  int id;
  get_sysattr (dev_, "idProduct", id);
  return id;
}

std::string
device::usb_serial () const
{
  std::string s;
  get_property (dev_, "ID_SERIAL_SHORT", s);
  return s;
}

uint8_t
device::usb_configuration () const
{
  int i = 1;
  get_sysattr (dev_, "bConfigurationValue", i);
  return i;
}

uint8_t
device::usb_interface () const
{
  int i = 0;
  get_sysattr (dev_, "bInterfaceNumber", i);
  return i;
}

uint8_t
device::usb_bus_number () const
{
  int i = 0;
  get_sysattr (dev_, "busnum", i, std::dec);
  return i;
}

uint8_t
device::usb_port_number () const
{
  int i = 0;
  get_sysattr (dev_, "devpath", i, std::dec);
  return i;
}

uint8_t
device::usb_device_address () const
{
  int i = 0;
  get_sysattr (dev_, "devnum", i, std::dec);
  return i;
}

}       // namespace udev_
