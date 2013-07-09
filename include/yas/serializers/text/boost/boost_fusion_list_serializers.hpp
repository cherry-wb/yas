
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

#ifndef _yas__text__boost_fusion_list_serializer_hpp__included_
#define _yas__text__boost_fusion_list_serializer_hpp__included_

#include <yas/detail/config/config.hpp>

#if defined(YAS_HAS_BOOST_FUSION)
#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/properties.hpp>
#include <yas/detail/type_traits/selector.hpp>
#include <yas/detail/preprocessor/preprocessor.hpp>

#include <boost/fusion/container/list.hpp>
#include <boost/fusion/include/list.hpp>
#include <boost/fusion/container/list/list_fwd.hpp>
#include <boost/fusion/include/list_fwd.hpp>
#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/include/make_list.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename Archive>
struct text_list_serializer {
	text_list_serializer(Archive& ar)
		:ar(ar)
	{}

	template<typename T>
	void operator()(const T& v) const {
		ar & v;
	}

	Archive& ar;
};

template<typename Archive>
struct text_list_deserializer {
	text_list_deserializer(Archive& ar)
		:ar(ar)
	{}

	template<typename T>
	void operator()(T& v) const {
		ar & v;
	}

	Archive& ar;
};

/***************************************************************************/

#define YAS__TEXT__GENERATE_EMPTY_SAVE_SERIALIZE_LIST_SPEC_VARIADIC() \
	template<> \
	struct serializer<type_prop::type_prop::not_a_pod, ser_method::use_internal_serializer, \
		archive_type::text, direction::out, boost::fusion::list<> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, const boost::fusion::list<>&) { return ar; } \
	};

#define YAS__TEXT__GENERATE_EMPTY_LOAD_SERIALIZE_LIST_SPEC_VARIADIC() \
	template<> \
	struct serializer<type_prop::type_prop::not_a_pod, ser_method::use_internal_serializer, \
		archive_type::text, direction::in, boost::fusion::list<> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, boost::fusion::list<>&) { return ar; } \
	};

#define YAS__TEXT__GENERATE_SAVE_SERIALIZE_LIST_SPEC_VARIADIC(unused, count, unused2) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<type_prop::type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::text, direction::out, boost::fusion::list<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, \
			const boost::fusion::list<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& list) \
		{ \
			ar & YAS_PP_INC(count); \
			boost::fusion::for_each(list, detail::text_list_serializer<Archive>(ar)); \
			return ar; \
		} \
	};

#define YAS__TEXT__GENERATE_SAVE_SERIALIZE_LIST_SPEC_VARIADICS(count) \
	YAS__TEXT__GENERATE_EMPTY_SAVE_SERIALIZE_LIST_SPEC_VARIADIC() \
	YAS_PP_REPEAT( \
		count, \
		YAS__TEXT__GENERATE_SAVE_SERIALIZE_LIST_SPEC_VARIADIC, \
		~ \
	)

#define YAS__TEXT__GENERATE_LOAD_SERIALIZE_LIST_SPEC_VARIADIC(unused, count, unused2) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<type_prop::type_prop::not_a_pod,ser_method::use_internal_serializer, \
		archive_type::text, direction::in, boost::fusion::list<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, \
			boost::fusion::list<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& list) \
		{ \
			yas::int32_t size = 0; \
			ar & size; \
			if ( size != YAS_PP_INC(count) ) throw std::runtime_error("size error on deserialize fusion::list"); \
			boost::fusion::for_each(list, detail::text_list_deserializer<Archive>(ar)); \
			return ar; \
		} \
	};

#define YAS__TEXT__GENERATE_LOAD_SERIALIZE_LIST_SPEC_VARIADICS(count) \
	YAS__TEXT__GENERATE_EMPTY_LOAD_SERIALIZE_LIST_SPEC_VARIADIC() \
	YAS_PP_REPEAT( \
		count, \
		YAS__TEXT__GENERATE_LOAD_SERIALIZE_LIST_SPEC_VARIADIC, \
		~ \
	)

/***************************************************************************/

YAS__TEXT__GENERATE_SAVE_SERIALIZE_LIST_SPEC_VARIADICS(FUSION_MAX_LIST_SIZE)
YAS__TEXT__GENERATE_LOAD_SERIALIZE_LIST_SPEC_VARIADICS(FUSION_MAX_LIST_SIZE)

/***************************************************************************/
#if 0
#define YAS__BINARY__GENERATE_EMPTY_SAVE_SERIALIZE_LIST_SPEC() \
	template<> \
	struct serializer<false, false, false, false, false, \
		archive_type::binary, direction::out, boost::fusion::list0<> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, const boost::fusion::list0<>&) { return ar; } \
	};

#define YAS__BINARY__GENERATE_EMPTY_LOAD_SERIALIZE_LIST_SPEC() \
	template<> \
	struct serializer<false, false, false, false, false, \
		archive_type::binary, direction::in, boost::fusion::list0<> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, boost::fusion::list0<>&) { return ar; } \
	};

#define YAS__BINARY__GENERATE_SAVE_SERIALIZE_LIST_SPEC(unused, count, text) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<false, false, false, false, false, \
		archive_type::binary, direction::out, \
		YAS_PP_CAT(boost::fusion::list, YAS_PP_INC(count)) \
			<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, \
			const YAS_PP_CAT(boost::fusion::list, YAS_PP_INC(count))<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& list) \
		{ \
			YAS__BINARY__WRITE_FUSION_LIST_SIZE(YAS_PP_INC(count)); \
			boost::fusion::for_each(list, detail::list_serializer<Archive>(ar)); \
			return ar; \
		} \
	};

#define YAS__BINARY__GENERATE_SAVE_SERIALIZE_LIST_SPECS(count) \
	YAS__BINARY__GENERATE_EMPTY_SAVE_SERIALIZE_LIST_SPEC(); \
	YAS_PP_REPEAT( \
		count, \
		YAS__BINARY__GENERATE_SAVE_SERIALIZE_LIST_SPEC, \
		~ \
	)

#define YAS__BINARY__GENERATE_LOAD_SERIALIZE_LIST_SPEC(unused, count, text) \
	template<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), typename T)> \
	struct serializer<false, false, false, false, false, \
		archive_type::binary, direction::in, \
		YAS_PP_CAT(boost::fusion::list, YAS_PP_INC(count)) \
			<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)> > \
	{ \
		template<typename Archive> \
		static Archive& apply(Archive& ar, \
			YAS_PP_CAT(boost::fusion::list, YAS_PP_INC(count))<YAS_PP_ENUM_PARAMS(YAS_PP_INC(count), T)>& list) \
		{ \
			YAS__BINARY__WRITE_FUSION_LIST_SIZE(YAS_PP_INC(count)); \
			boost::fusion::for_each(list, detail::list_deserializer<Archive>(ar)); \
			return ar; \
		} \
	};

#define YAS__BINARY__GENERATE_LOAD_SERIALIZE_LIST_SPECS(count) \
	YAS__BINARY__GENERATE_EMPTY_LOAD_SERIALIZE_LIST_SPEC(); \
	YAS_PP_REPEAT( \
		count, \
		YAS__BINARY__GENERATE_LOAD_SERIALIZE_LIST_SPEC, \
		~ \
	)

/***************************************************************************/

YAS__BINARY__GENERATE_SAVE_SERIALIZE_LIST_SPECS(FUSION_MAX_LIST_SIZE);
YAS__BINARY__GENERATE_LOAD_SERIALIZE_LIST_SPECS(FUSION_MAX_LIST_SIZE);

#endif
/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // defined(YAS_HAS_BOOST_FUSION)

#endif // _yas__text__boost_fusion_list_serializer_hpp__included_
