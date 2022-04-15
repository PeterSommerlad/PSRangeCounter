# PSRangeCounter
Counting range-for loop in C++

```C++
#include "range_counter.h"
```

If you have the ability to use C++20, you can use `std::ranges::views::iota(0,N)` for the similar capabilities.

Use cases:

* counting with a range-for loop (MISRA-C++ compliance)
* input subsequent integral values starting with 0 for an algorithm call

The types for iterators and zero-based ranges are enum types to ensure efficient pass by value, regardless of bad ABI for class types.

Example usage:

```C++
#include "range_counter.h"
using namespace ps_counter::literals;
// or using ps_counter::operator""_times;


void testRangeLoopLoops(){
	size_t sum{};
	for(auto const i : 11_times){ // or 11_up_to
		sum +=i;
	}
	ASSERT_EQUAL(55u,sum); // [0..10]
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

Or simpler, use `ps_counter::reverse_range` by using suffix `_down_from`

```C++
void testForwardReversedRange(){
	std::string sum{};
	for(auto const i : 11_down_from ){
		sum.append(std::to_string(i));
	}
	ASSERT_EQUAL("109876543210",sum);
}
```

## Ranges with non-zero start (only positive integers!)

```C++
#include "range_counter_range.h"
```

As an exercise of integer parsing with template UDL operators, 
I implemented also ranges where you can specify start and end. 
However, this requires a class type for keeping both values around, instead of a simple enum.

Also the syntax for specifying the ranges is a bit unintuitive, because it employs valid floating point syntax to separate start from end and 
to allow inclusive and exclusive ranges to be specified with the same UDL suffix `_range`. 
The underlying scaffolding needed in addtion to the contents of `"range_counter.h"` is not part of the public API.
If you need to specify a range with variable content, use a `ps_counter::(reverse_)iterator` pair or in case of C++20 `std::iota_view`.

The following are supported `.` - dot = half-open range, `'` - quote = closed range, counting down is automatically determined if the first integer is bigger than the second one:

*  `10_range` -> 0,1,2,3,4,5,6,7,8,9
*  `.10_range` -> 0,1,2,3,4,5,6,7,8,9,10
*  `5.10_range` -> 5,6,7,8,9,10
*  `1'10_range` -> 1,2,3,4,5,6,7,8,9
*  `5.1_range` -> 5,4,3,2,1
*  `5'1_range` -> 4,3,2,1
*  `5'0_range` -> 4,3,2,1,0
