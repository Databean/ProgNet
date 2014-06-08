#include "BinaryStream.h"

#include <string>

using std::string;
using std::ostream;
using std::istream;

bostream::bostream(ostream& out) : out(out) {
	
}

bostream& bostream::write(const char* s, std::streamsize count) {
	out.write(s, count);
	return *this;
}

bostream& operator<<(bostream& out, const std::string& s) {
	out << s.size();
	return out.write(s.c_str(), s.size());
}

bistream::bistream(istream& in) : in(in) {
	
}

bistream& bistream::read(char* s, std::streamsize count) {
	in.read(s, count);
	return *this;
}

bistream& operator>>(bistream& in, std::string& s) {
	std::size_t size;
	in >> size;
	char* data = new char[size];
	return in.read(data, size);
}
