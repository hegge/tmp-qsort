/* Compile with:
 * g++ -Wall -Wextra -Werror -std=gnu++11 -o tlist tlist.cpp
 */

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

// Template list of ints (cons).
template<int T, class U>
struct tlist
{
	typedef int head;
	typedef U tail;
};

struct null_type
{};


// Basic pattern matching.
template<class tlist> struct length;
template<> struct length<null_type>
{
	enum { value = 0 };
};
template<int T, class U> struct length<tlist<T,U> >
{
	enum { value = 1 + length<U>::value };
};

template<class tlist> struct sum;
template<> struct sum<null_type>
{
	enum { value = 0 };
};
template<int T, class U> struct sum<tlist<T,U> >
{
	enum { value = T + sum<U>::value };
};

// Concatenation of two tlists.
template<class tlist1, class tlist2> struct concat;
template<class tlist2> struct concat<null_type, tlist2>
{
	typedef tlist2 result;
};
template<int T, class U, class tlist2> struct concat<tlist<T,U>, tlist2>
{
private:
	typedef typename concat<U, tlist2>::result tail;
public:
	typedef tlist<T, tail> result;
};

// Build a new list tlist<head, tail> or keep tail.
template<bool B, int head, class tail> struct take;
template<int head, class tail> struct take<true, head, tail>
{
	typedef tlist<head, tail> result;
};
template<int head, class tail> struct take<false, head, tail>
{
	typedef tail result;
};

// Binary predicates.
template<int lhs, int rhs>
struct is_less
{
	enum { value = lhs < rhs };
};

template<int lhs, int rhs>
struct is_ge
{
	enum { value = lhs >= rhs };
};

// Filter using binary predicates.
template<
	template<int, int> class pred,
	int limit,
	class tlist >
struct filter;

template<
	template<int, int> class  pred,
	int limit >
struct filter<pred, limit, null_type>
{
	typedef null_type result;
};

template<
	template<int, int> class pred,
	int limit,
	int head,
	class tail >
struct filter<pred, limit, tlist<head,tail>>
{
private:
	typedef typename filter<pred, limit, tail>::result comp_tail;
	static const bool should_take = pred<head, limit>::value;
public:
	typedef typename take<should_take, head, comp_tail>::result result;
};

// Quicksort.
template<class tlist> struct quicksort;
template<> struct quicksort<null_type>
{
	typedef null_type result;
};
template<int head, class tail>
struct quicksort< tlist<head, tail> >
{
private:
	typedef typename quicksort<typename filter<is_less, head, tail>::result >::result left;
	typedef typename quicksort<typename filter<is_ge, head, tail>::result >::result right;
public:
	typedef typename concat<left, tlist<head, right> >::result result;
};

// Alternative implementation of filter and quicksort using nested, templated
// structs to emulate partial application of predicates.
//
// The quicksort bit doesn't compile with gcc 4.7, and might not be valid
// C++11.
template<int rhs>
struct is_less1
{
	template<int lhs>
	struct lambda {
		enum { value = lhs < rhs };
	};
};

template<int rhs>
struct is_ge1
{
	template<int lhs>
	struct lambda {
		enum { value = lhs >= rhs };
	};
};

template<int input>
struct not_null
{
	enum { value = 1 };
};
template<>
struct not_null<0>
{
	enum { value = 0 };
};

// Filter using unary predicates.
template<
	template<int> class pred,
	class tlist >
struct filter1;

template<
	template<int> class pred >
struct filter1<pred, null_type>
{
	typedef null_type result;
};

template<
	template<int> class pred,
	int head,
	class tail >
struct filter1<pred, tlist<head,tail>>
{
private:
	typedef typename filter1<pred, tail>::result comp_tail;
	static const bool should_take = pred<head>::value;
public:
	typedef typename take<should_take, head, comp_tail>::result result;
};

#if 0
template<class tlist> struct quicksort1;
template<> struct quicksort1<null_type>
{
	typedef null_type result;
};
template<int head, class tail>
struct quicksort1< tlist<head, tail> >
{
private:
	typedef typename is_less1<head>::lambda pred_left;
	typedef typename is_ge1<head>::lambda pred_right;
	typedef typename filter1<pred_left, tail>::result partition_left;
	typedef typename filter1<pred_right, tail>::result partition_rigth;
	typedef typename quicksort1<partition_left>::result left;
	typedef typename quicksort1<partition_right>::result right;
public:
	typedef typename concat<left, tlist<head, right> >::result result;
};
#endif

// Pretty printing.
template<class tlist> struct format;
template<> struct format<null_type>
{
	std::string operator()(){
		return "";
	}
};
template<int T, class U> struct format<tlist<T,U> >
{
	std::string operator()(){
		std::stringstream ss;
		format<U> print;
		ss << T << " " << print();
		return ss.str();
	}
};

// Simpler building of lists.
template<int...> struct
build_list;

template<> struct
build_list<> {
	typedef null_type result;
};

template<int i, int... tail> struct
build_list<i, tail...> {
	typedef tlist<i, typename build_list<tail...>::result > result;
};

int main()
{
	typedef tlist<1, tlist<5, tlist<2, null_type> > > numlist;
	typedef tlist<7, numlist> numlist2;

	std::cout << "length 1: " << length<numlist>::value << std::endl;
	std::cout << "length 2: " << length<numlist2>::value << std::endl;

	std::cout << "sum 1: " << sum<numlist>::value << std::endl;
	std::cout << "sum 2: " << sum<numlist2>::value << std::endl;

	std::cout << "filtered sum 1: " << sum<filter<is_less, 2, numlist2>::result>::value << std::endl;

	typedef concat<numlist, numlist2>::result combined;
	std::cout << "sum combined: " << sum<combined>::value << std::endl;

	std::cout << "list: " << format<combined>()() << std::endl;
	std::cout << "sorted: " << format<quicksort<combined>::result>()() << std::endl;

	typedef build_list<1, 3, 0, 5, 2, 1>::result simple_list;

	std::cout << "list: " << format<simple_list>()() << std::endl;
	std::cout << "sorted: " << format<quicksort<simple_list>::result>()() << std::endl;

	// Unary version
	std::cout << "cmp: " << is_ge1<3>::lambda<4>::value << " " <<
				is_ge1<5>::lambda<4>::value << std::endl;

	std::cout << "filtered sum 1: " << sum<filter1<is_less1<2>::lambda, numlist2>::result>::value << std::endl;
	std::cout << "filtered: " << format<filter1<not_null, simple_list>::result>()() << std::endl;
#if 0
	std::cout << "sorted: " << format<quicksort1<simple_list>::result>()() << std::endl;
#endif
	
	return 0;
}
