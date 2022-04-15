#ifndef RANGE_COUNTER_RANGE_H_
#define RANGE_COUNTER_RANGE_H_

#include "range_counter.h"
#include <string_view> // for string_view

namespace ps_counter {

// for ranges with non-zero lower bound:
namespace detail_ {
struct be_range{
	using value_type = decltype(*iterator{});
	constexpr
	be_range(value_type const be, value_type const en) :
			b { iterator { be } }, e { iterator { en } } {
		// assert(be <= en); // not constexpr now
	}
	iterator b{},e{};
	constexpr auto begin() const {
		return b;
	}
	constexpr auto end() const {
		return e;
	}
	constexpr auto rbegin() const {
		return reverse_iterator(e);
	}
	constexpr auto rend() const {
		return reverse_iterator(b);
	}
};
struct be_reverse_range{
	using value_type = decltype(*iterator{});
	constexpr
	be_reverse_range(value_type const be, value_type const en) :
			b { reverse_iterator { en } }, e { reverse_iterator { be } } {
		//assert(be > en);
	}
	reverse_iterator b{},e{};
	constexpr auto begin() const {
		return b;
	}
	constexpr auto end() const {
		return e;
	}
	constexpr auto rbegin() const {
		return iterator(e);
	}
	constexpr auto rend() const {
		return iterator(b);
	}
};

struct bi_range{
	using value_type = decltype(*iterator{});
	constexpr
	bi_range(value_type const be, value_type const en) :
			b { iterator { be } }, e { iterator { en + 1 } } {
		//assert(be <= en);
	}
	iterator b{},e{};
	constexpr auto begin() const {
		return b;
	}
	constexpr auto end() const {
		return e;
	}
	constexpr auto rbegin() const {
		return reverse_iterator(e);
	}
	constexpr auto rend() const {
		return reverse_iterator(b);
	}
};

struct bi_reverse_range{
	using value_type = decltype(*iterator{});
	constexpr
	bi_reverse_range(value_type const be, value_type const en) :
			b { reverse_iterator { en + 1 } }, e { reverse_iterator { be } } {
		//assert(be > en); // not constexpr right now
	}
	reverse_iterator b{},e{};
	constexpr auto begin() const {
		return b;
	}
	constexpr auto end() const {
		return e;
	}
	constexpr auto rbegin() const {
		return iterator(e);
	}
	constexpr auto rend() const {
		return iterator(b);
	}
};

template<char ...cs>
constexpr bool contains_dot{false};

template<char maybedot,char ...cs>
constexpr bool contains_dot<maybedot,cs...>{maybedot=='.'?true:contains_dot<cs...>};

static_assert(not contains_dot<>);
static_assert(contains_dot<'.'>);
static_assert(contains_dot<'0','.','0'>);
static_assert(contains_dot<'0','.'>);
static_assert(contains_dot<'.','0'>);
static_assert(not contains_dot<'1','0'>);

template<char ...cs>
constexpr bool contains_quote{false};

template<char maybequote,char ...cs>
constexpr bool contains_quote<maybequote,cs...>{maybequote=='\''?true:contains_quote<cs...>};

template<char c, char ...cs>
constexpr size_t parse_int(){
	if constexpr (sizeof...(cs)){
		return parse_int<c>() * 10 + parse_int<cs...>();
	} else if constexpr (c >= '0' && c <='9'){
		return c - '0';
	} else {
		throw "invalid characters";
	}
}
constexpr auto
parse_int(std::string_view const s){
	size_t result{};
	for (char c:s){
		if (c >='0' && c <= '9'){
			auto const digit{size_t(c - '0')};
			if (result != 0) {
				result *= 10;
			}
			result += digit;
		} else {
			return result;
		}
	}
	return result;
}

template<char delim, char ...cs>
constexpr size_t parse_int_upto(){
	char const  cstr[]={cs..., '\0'};
	std::string_view s{cstr,sizeof...(cs)};
	return parse_int(s.substr(0,s.find(delim)));

}
template<char delim, char ...cs>
constexpr size_t parse_int_from(){
	char const  cstr[]={cs..., '\0'};
	std::string_view s{cstr,sizeof...(cs)};
	auto const pos{s.find(delim)};
	if (pos != std::string_view::npos){
		return parse_int(s.substr(pos+1));
	} else {
		return 0;
	}
}
} // namespace detail_
inline namespace literals {

// 10_range -> 0,1,2,3,4,5,6,7,8,9
// .10_range -> 0,1,2,3,4,5,6,7,8,9,10
// 5.10_range -> 5,6,7,8,9,10
// 1'10_range -> 1,2,3,4,5,6,7,8,9
// 5.1_range -> 5,4,3,2,1
// 5'1_range -> 4,3,2,1
// 5'0_range -> 4,3,2,1,0

#ifdef __cpp_consteval
#define CONSTEVAL consteval
# else
#define CONSTEVAL constexpr
#endif

// show-off UDL, could also return std::iota_view for C++20
template<char ...cs>
CONSTEVAL
auto operator""_range(){
	using namespace ps_counter::detail_;
	if constexpr (contains_dot<cs...>) {
		constexpr auto from { parse_int_upto<'.',cs...>() };
		constexpr auto to   { parse_int_from<'.',cs...>() };
		if constexpr( from <= to ){
		   return bi_range{from,to};
		} else {
			return bi_reverse_range{to,from};
		}
	} else {
		if constexpr (contains_quote<cs...>){
			constexpr auto from { parse_int_upto<'\'',cs...>() };
			constexpr auto to   { parse_int_from<'\'',cs...>() };
			if constexpr( from <= to ){
			   return be_range{from,to};
			} else {
				return be_reverse_range{to,from};
			}
		} else {
			return range{parse_int<cs...>()};
		}
	}
}
#undef CONSTEVAL


}
}
#endif /* RANGE_COUNTER_RANGE_H_ */
