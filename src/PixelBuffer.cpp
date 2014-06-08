#include "PixelBuffer.h"
#include "Util.h"
#include <stdexcept>
#include <cstdlib>
#include <algorithm>

using std::min;
using std::max;

using std::runtime_error;

Color::Color() : r(0), g(0), b(0) {
	
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {
	
}

Color::Color(const Color& other) : r(other.r), g(other.g), b(other.b) {
	
}

Color::~Color() {
	
}

const Color& Color::operator=(const Color& other) {
	red() = other.red();
	green() = other.green();
	blue() = other.blue();
	return *this;
}

const unsigned char& Color::red() const {
	return r;
}

const unsigned char& Color::green() const {
	return g;
}

const unsigned char& Color::blue() const {
	return b;
}

unsigned char& Color::red() {
	return r;
}

unsigned char& Color::green() {
	return g;
}

unsigned char& Color::blue() {
	return b;
}

bool Color::operator==(const Color& c) const {
	return red() == c.red() && blue() == c.blue() && green() == c.green();
}

Color Color::operator+(const Color& c) const {
	return Color(red() + c.red(), blue() + c.blue(), red() + c.red());
}

Color Color::operator-(const Color& c) const {
	return Color(red() - c.red(), blue() - c.blue(), red() - c.red());
}

Color Color::operator*(unsigned char i) const {
	return Color(red() * i, blue() * i, red() * i);
}

Color Color::operator/(unsigned char i) const {
	return Color(red() / i, blue() / i, red() / i);
}

const Color& Color::operator+=(const Color& c) {
	red() += c.red();
	blue() += c.blue();
	green() += c.green();
	return *this;
}

const Color& Color::operator-=(const Color& c) {
	red() -= c.red();
	blue() -= c.blue();
	green() -= c.green();
	return *this;
}

const Color& Color::operator*=(unsigned char i) {
	red() *= i;
	blue() *= i;
	green() *= i;
	return *this;
}


const Color& Color::operator/=(unsigned char i) {
	red() /= i;
	blue() /= i;
	green() /= i;
	return *this;
}

bostream& operator<<(bostream& out, const Color& c) {
	return out << c.red() << c.green() << c.blue();
}

bistream& operator>>(bistream& in, Color& c) {
	return in >> c.red() >> c.green() >> c.blue();
}

PixelBuffer::PixelBuffer(unsigned int xmax, unsigned int ymax) : xmax(xmax), ymax(ymax) {
	data = new Color*[maxX()];
	for(unsigned int x = 0; x < maxX(); x++) {
		data[x] = new Color[maxY()];
	}
}

PixelBuffer::PixelBuffer(const PixelBuffer& other) : data(NULL) {
	(*this) = other;
}

PixelBuffer::PixelBuffer(PixelBuffer&& old) : xmax(old.xmax), ymax(old.ymax), data(old.data) {
	old.data = NULL;
}

PixelBuffer::~PixelBuffer() {
	deleteData();
}

const PixelBuffer& PixelBuffer::operator=(const PixelBuffer& other) {
	deleteData();
	xmax = other.maxX();
	ymax = other.maxY();
	data = new Color*[maxX()];
	for(unsigned int x = 0; x < maxX(); x++) {
		data[x] = new Color[maxY()];
		for(unsigned int y = 0; y < maxY(); y++) {
			data[x][y] = other(x, y);
		}
	}
	return *this;
}

void PixelBuffer::deleteData() {
	if(data != NULL) {
		for(unsigned int x = 0; x < maxX(); x++) {
			delete data[x];
		}
		delete data;
	}
}

const Color& PixelBuffer::operator()(unsigned int x, unsigned int y) const {
	if(x < maxX() && y < maxY()) {
		return data[x][y];
	} else {
		throw runtime_error("PixelBuffer: Access (" + toStr(x) + ", " + toStr(y) + ") is out of the bounds (" + toStr(maxX()) + ", " + toStr(maxY()) + ").");
	}
}


Color& PixelBuffer::operator()(unsigned int x, unsigned int y) {
	if(x < maxX() && y < maxY()) {
		return data[x][y];
	} else {
		throw runtime_error("PixelBuffer: Access (" + toStr(x) + ", " + toStr(y) + ") is out of the bounds (" + toStr(maxX()) + ", " + toStr(maxY()) + ").");
	}
}

const unsigned int& PixelBuffer::width() const {
	return xmax;
}

const unsigned int& PixelBuffer::height() const {
	return ymax;
}

const unsigned int& PixelBuffer::maxX() const {
	return xmax;
}

const unsigned int& PixelBuffer::maxY() const {
	return ymax;
}

PixelBuffer PixelBuffer::resize(unsigned int newMaxX, unsigned int newMaxY) const {
	PixelBuffer newBuffer(newMaxX, newMaxY);
	for(unsigned int x = 0; x < min(maxX(), newMaxX); x++) {
		for(unsigned int y = 0; y < min(maxY(), newMaxY); y++) {
			newBuffer.data[x][y] = data[x][y];
		}
	}
	return newBuffer;
}


