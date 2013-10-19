/* Compile with:
 * g++ -Wall -Wextra -Werror -std=gnu++11 -o tlist tlist.cpp
 */

#include <iostream>
#include <cassert>
#include <string>

template<int T, class U>
struct tlist
{
	typedef int head;
	typedef U tail;
};

struct null_type
{};


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


int main(int argc, char const* argv[])
{
	assert(argc==1);
	assert(argv[0]);

	typedef tlist<1, tlist<5, tlist<2, null_type> > > numlist;

	typedef tlist<7, numlist> numlist2;

	std::cout << "length 1: " << length<numlist>::value << std::endl;
	std::cout << "length 2: " << length<numlist2>::value << std::endl;

	std::cout << "sum 1: " << sum<numlist>::value << std::endl;
	std::cout << "sum 2: " << sum<numlist2>::value << std::endl;
	
	return 0;
}
