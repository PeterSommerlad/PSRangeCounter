#include "range_counter_range.h"
#include "range_counter_range_tests.h"
#include "cute.h"
#include "reversed.h"

using ps_counter::operator ""_range;
static_assert(std::is_same_v<decltype(1_range),ps_counter::range>);
static_assert(std::is_same_v<decltype(0.1_range),ps_counter::detail_::bi_range>);
static_assert(std::is_same_v<decltype(0'1_range),ps_counter::detail_::be_range>);
static_assert(*end(10_range) == 10u);
static_assert(*begin(10_range) == 0u);
static_assert(*(10'100_range).begin() == 10u);
static_assert(*(10'100_range).end() == 100u);
static_assert(*(10.99_range).begin() == 10u);
static_assert(*(10.99_range).end() == 100u);
static_assert(*(10'1_range).end() == 0);
static_assert(*(10.1_range).end() == 0);
static_assert(*(10'0_range).begin() ==9);
static_assert(*(10.0_range).begin() ==10);

void test_UDL_creates_sane_result(){
	auto rgudl { 10'0_range };
	ps_counter::detail_::be_reverse_range rg{0,10};
	ASSERT_EQUAL(*(rg.begin()),*(rgudl.begin()));
	ASSERT_EQUAL(*(rg.end()),*(rgudl.end()));
}

void test_UDL_inclusive_creates_sane_result(){
	auto rgudl { 10.0_range };
	ps_counter::detail_::bi_reverse_range rg{0,10};
	ASSERT_EQUAL(*(rg.begin()),*(rgudl.begin()));
	ASSERT_EQUAL(*(rg.end()),*(rgudl.end()));
}


void test_with_begin_end(){
	std::string s{};
	for (auto const i : ps_counter::detail_::be_range { 1, 11 }) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("12345678910",s);
}

void test_with_reversed_begin_end(){
	std::string s{};
	for (auto const i: adapter::reversed(ps_counter::detail_::be_range{1,11})){
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);
}
void test_with_reverse_begin_end(){
	std::string s{};
	for (auto const i: ps_counter::detail_::be_reverse_range{1,11}){
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);
}
void test_with_begin_inclusive_end(){
	std::string s{};
	for (auto const i : ps_counter::detail_::bi_range { 1, 10 }) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("12345678910",s);
}

void test_wit_reversed_begin_inclusive_end(){
	std::string s{};
	for (auto const i: adapter::reversed(ps_counter::detail_::bi_range{1,10})){
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);
}
void test_with_reverse_begin_inclusive_end(){
	std::string s{};
	for (auto const i: ps_counter::detail_::bi_reverse_range{1,10}){
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);
}

void test_with_exclusive_range_UDL(){
	std::string s{};
	for (auto const i : 1'11_range ) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("12345678910",s);

}
void test_with_exclusive_reverse_range_UDL(){
	std::string s{};
	for (auto const i : 11'1_range ) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);

}

void test_with_inclusive_range_UDL(){
	std::string s{};
	for (auto const i : 1.10_range ) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("12345678910",s);

}
void test_with_inclusive_reverse_range_UDL(){
	std::string s{};
	for (auto const i : 10.1_range ) {
		s.append(std::to_string(i));
	}
	ASSERT_EQUAL("10987654321",s);

}
cute::suite make_suite_range_counter_range_tests() {
	cute::suite s { };
	s.push_back(CUTE(test_UDL_creates_sane_result));
	s.push_back(CUTE(test_UDL_inclusive_creates_sane_result));
	s.push_back(CUTE(test_with_begin_end));
	s.push_back(CUTE(test_with_reversed_begin_end));
	s.push_back(CUTE(test_with_reverse_begin_end));
	s.push_back(CUTE(test_with_begin_inclusive_end));
	s.push_back(CUTE(test_wit_reversed_begin_inclusive_end));
	s.push_back(CUTE(test_with_reverse_begin_inclusive_end));
	s.push_back(CUTE(test_with_exclusive_range_UDL));
	s.push_back(CUTE(test_with_exclusive_reverse_range_UDL));
	s.push_back(CUTE(test_with_inclusive_range_UDL));
	s.push_back(CUTE(test_with_inclusive_reverse_range_UDL));
	return s;
}
