#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>

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

#endif