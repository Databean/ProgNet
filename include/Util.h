#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>
#include <memory>

/**
 * Converts any element with a global @code std::ostream& operator<<(std::ostream&, T) @endcode
 * method into a string. Instead of each class providing a toString method, classes which can
 * sensibly convert to strings should overload the global << operator with the previous convention,
 * as this is more versatile and also more efficient when outputting a sequence of things than using
 * the string + operator.
 */
template<class T>
std::string toStr(const T& t) {
	std::stringstream strm;
	strm << t;
	return strm.str();
}

/**
 * Construct a resource unique_ptr, given a constructor, destructor, and arugments.
 * Intended for use with C-style constructor and destructor functions that operate on pointers.
 * From http://ericscottbarr.com/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
 * @param c The creator/initialization function.
 * @param d The destructor function
 * @param args The arguments to give to the constructor.
 * @return A unique pointer to the constructed resource.
 */
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
-> std::unique_ptr<typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type, void(*)(typename std::decay<decltype(*c(std::forward<Arguments>(args)...))>::type*)>
{
	auto r = c(std::forward<Arguments>(args)...);
	if (!r) { throw std::runtime_error {"Unable to create resource"}; }
	typedef typename std::decay<decltype(*r)>::type ResourceType;
	return std::unique_ptr<ResourceType, void(*)(ResourceType*)>(r, d);
}

#endif