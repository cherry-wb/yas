
// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
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

#ifndef _yas__const_buffer_hpp__included_
#define _yas__const_buffer_hpp__included_

#include <cstring>

#include <yas/config/config.hpp>

#if defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR)
#	include <memory>
#elif defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR)
#	include <boost/shared_ptr.hpp>
#endif

namespace yas {

/***************************************************************************/

struct intrusive_buffer {
	intrusive_buffer(const void* data, yas::uint32_t size)
		:data(data)
		,size(size)
	{}
	intrusive_buffer(const intrusive_buffer& o)
		:data(o.data)
		,size(o.size)
	{}

	const void* data;
	const yas::uint32_t size;

private:
	intrusive_buffer();
};

/***************************************************************************/

#if defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR) || \
	defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR)

struct shared_buffer {
	shared_buffer()
		:size(0)
	{}
	shared_buffer(const void* ptr, yas::uint32_t size)
		:size(0)
	{
		data.reset(new char[size], &deleter);
		std::memcpy(data.get(), ptr, size);
		this->size = size;
	}
#if defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR)
	shared_buffer(std::shared_ptr<char> buf, yas::uint32_t size)
		:size(0)
	{
		data = buf;
		this->size = size;
	}
#elif defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR)
	shared_buffer(boost::shared_ptr<char> buf, yas::uint32_t size)
		:size(0)
	{
		data.reset(new char[size], &deleter);
		std::memcpy(data.get(), buf.get(), size);
		this->size = size;
	}
#endif

	shared_buffer(const shared_buffer& buf)
		:size(0)
	{
		data = buf.data;
		size = buf.size;
	}

#if defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR)
	typedef std::shared_ptr<char> shared_array_type;
#elif defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR)
	typedef boost::shared_ptr<char> shared_array_type;
#endif

	shared_array_type data;
	yas::uint32_t size;

	static void deleter(void* ptr) { delete[] ((char*)ptr); }
};

#endif /* defined(YAS_SHARED_BUFFER_USE_STD_SHARED_PTR) \
			 || defined(YAS_SHARED_BUFFER_USE_BOOST_SHARED_PTR) */

/***************************************************************************/

} // namespace yas

#endif // _yas__const_buffer_hpp__included_
