
// Copyright (c) 2010-2013 niXman (i dot nixman dog gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__binary__std_string_serializer_hpp__included_
#define _yas__binary__std_string_serializer_hpp__included_

#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/selector.hpp>

#include <string>

namespace yas {
namespace detail {

/***************************************************************************/

template<>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::out,
	std::string
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, const std::string& string) {
		const yas::uint32_t size = string.length();
		ar.write(reinterpret_cast<const char*>(&size), sizeof(size));
		ar.write(&string[0], string.length());
		return ar;
	}
};

template<>
struct serializer<
	type_prop::not_a_pod,
	ser_method::use_internal_serializer,
	archive_type::binary,
	direction::in,
	std::string
> {
	template<typename Archive>
	static Archive& apply(Archive& ar, std::string& string) {
		yas::uint32_t size = 0;
		ar.read(reinterpret_cast<char*>(&size), sizeof(size));
		string.resize(size);
		ar.read(&string[0], size);
		return ar;
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__binary__std_string_serializer_hpp__included_
