#ifndef BINARY_STREAM_H
#define BINARY_STREAM_H

#include <string>
#include <istream>
#include <ostream>
#include <type_traits>

class bostream {
private:
	std::ostream& out;
public:
	bostream(std::ostream& out);
	
	bostream& write(const char* s, std::streamsize count);
};

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, bostream&>::type operator<<(bostream& out, T num) {
	return out.write(reinterpret_cast<char*>(&num), sizeof(T));
}

bostream& operator<<(bostream& out, const std::string& s);

class bistream {
private:
	std::istream& in;
public:
	bistream(std::istream& in);
	
	bistream& read(char* s, std::streamsize count);
};

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, bistream&>::type operator>>(bistream& in, T& num) {
	in.read(reinterpret_cast<char*>(&num), sizeof(T));
	return in;
}

bistream& operator>>(bistream& in, std::string& s);

#endif