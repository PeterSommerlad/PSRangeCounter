# PSRangeCounter
Counting range-for loop in C++

If you have the ability to use C++20, you can use `std::ranges::views::iota(0,N)` for the similar capabilities.

Use cases:

* counting with a range-for loop (MISRA-C++ compliance)
* input subsequent integral values starting with 0 for an algorithm call

The types are enum types to ensure efficient pass by value, regardless of bad ABI for class types.

Example usage:

```C++
#include "range_counter.h"
using ps_counter::operator""_times;


void testRangeLoopLoops(){
	size_t sum{};
	for(auto const i : 11_times){
		sum +=i;
	}
	ASSERT_EQUAL(55u,sum);
}
void test_with_algorithm(){
	std::string s{};
	using it=ps_counter::iterator;
	std::transform(it{},it{10},std::back_inserter(s),[](size_t i){return std::to_string(i)[0];});
	ASSERT_EQUAL("0123456789",s);
}
```

play with it on godbolt: [https://godbolt.org/z/n9137oh67](https://godbolt.org/z/n9137oh67)

Using my reverse range adapter, one can even walk backwards:

```C++
#include "reversed.h"

#include <algorithm>
#include <iterator>

#include <string>

using adapter::reversed;

void testReversedRange(){
	std::string sum{};
	for(auto const i : reversed(11_times) ){
		sum.append(std::to_string(i));
	}
	ASSERT_EQUAL("109876543210",sum);
}
```