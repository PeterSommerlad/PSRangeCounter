#include "range_counter.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"


using ps_counter::operator""_times;



void testBeginReturnsZero() {
	auto const range{10_times};
	ASSERT_EQUAL(*begin(range),0u);
}

void testEndReturnsNumber(){
	auto const range { 10_times };
	ASSERT_EQUAL(*end(range),10u);
}
void testIteratorsCompareEqual(){
	auto const range { 10_times };
	ASSERT_EQUAL(begin(range),begin(range));
}
void testBeginEndCompareInequal(){
	auto const range { 10_times };
	ASSERT_NOT_EQUAL_TO(begin(range),end(range));
}

void testEmptyRangeBeginEndEqual(){
	auto const range { 0_times };
	ASSERT_EQUAL(begin(range),end(range));
}

void testUnitRangeIteratorCanIncrement(){
	auto const range { 1_times };
	auto it=begin(range);
	++it;
	ASSERT_EQUAL(it,end(range));
}
void testRangeLoopLoops(){
	size_t sum{};
	for(auto const i : 11_times){
		sum +=i;
	}
	ASSERT_EQUAL(55u,sum);
}
void testRangeForWithVariable(){
	int const steps{5};
	size_t sum{};
	for(auto const i: ps_counter::range(steps)){
		sum += i;
	}
	ASSERT_EQUAL(10u,sum);
}
void testRbeginOneFromEnd(){
	auto const range { 10_times };
	auto rit=rbegin(range);
	ASSERT_EQUAL(9u, *rit);
}
void testRbeginIncDecrementsValue(){
	auto const range { 10_times };
	auto rit=rbegin(range);
	++rit;
	ASSERT_EQUAL(8u, *rit);
}




#include "reversed.h"

#include <string>

using adapter::reversed;

void testReversedRange(){
	std::string sum{};
	for(auto const i : reversed(11_times) ){
		sum.append(std::to_string(i));
	}
	ASSERT_EQUAL("109876543210",sum);
}




bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(testBeginReturnsZero));
	s.push_back(CUTE(testEndReturnsNumber));
	s.push_back(CUTE(testIteratorsCompareEqual));
	s.push_back(CUTE(testBeginEndCompareInequal));
	s.push_back(CUTE(testEmptyRangeBeginEndEqual));
	s.push_back(CUTE(testUnitRangeIteratorCanIncrement));
	s.push_back(CUTE(testRangeLoopLoops));
	s.push_back(CUTE(testRangeForWithVariable));
	s.push_back(CUTE(testReversedRange));
	s.push_back(CUTE(testRbeginOneFromEnd));
	s.push_back(CUTE(testRbeginIncDecrementsValue));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
