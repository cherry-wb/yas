
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

#ifndef _yas__text_file_stream_hpp__included_
#define _yas__text_file_stream_hpp__included_

#include <ostream>
#include <istream>

#include <yas/detail/type_traits/properties.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<archive_type::type>
struct ofilestream;

template<>
struct ofilestream<archive_type::text>: std::ostream {
	ofilestream(std::ostream& file)
		:std::ostream(file.rdbuf())
	{}

	std::streamsize write(const char* ptr, yas::uint32_t size) {
		return (std::ostream::write(ptr, size).good())?size:0;
	}

	template<typename T>
	std::streamsize write(const T* ptr, yas::uint32_t) {
		std::ostream::operator <<(*ptr);
		return 0;
	}
};

/***************************************************************************/

template<archive_type::type>
struct ifilestream;

template<>
struct ifilestream<archive_type::text>: std::istream {
	ifilestream(std::istream& file)
		:std::istream(file.rdbuf())
	{}

	inline std::streamsize read(char* ptr, yas::uint32_t size) {
		return std::istream::read(ptr, size).gcount();
	}
	template<typename T>
	inline std::streamsize read(T*, yas::uint32_t) {
		return 0;
	}
};

/***************************************************************************/

} // ns detail
} // ns yas

#endif // _yas__text_file_stream_hpp__included_
