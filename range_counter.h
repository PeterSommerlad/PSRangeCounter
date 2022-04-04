#ifndef RANGE_COUNTER_H_
#define RANGE_COUNTER_H_

#include <cstddef> // for size_t
namespace ps_counter {
enum class range:size_t{};
enum class iterator:size_t{};
enum class reverse_iterator:size_t{};
constexpr auto
operator"" _times(unsigned long long count){
	return range{count};
}
constexpr auto
begin(range){
	return iterator{};
}
constexpr auto
end(range const r){
	return iterator{r};
}

constexpr auto
rbegin(range const r){
	return reverse_iterator{r};
}
constexpr auto
rend(range ){
	return reverse_iterator{};
}

constexpr auto
operator*(iterator const it){
	return static_cast<size_t>(it);
}
constexpr auto&
operator++(iterator &it){
	return it = iterator{*it+1};
}

constexpr auto
operator*(reverse_iterator const it){
	return static_cast<size_t>(it)-1;
}
constexpr auto&
operator++(reverse_iterator &it){
	return it = reverse_iterator{*it};
}
}



#endif /* RANGE_COUNTER_H_ */