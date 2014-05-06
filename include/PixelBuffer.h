#ifndef PIXEL_BUFFER_H
#define PIXEL_BUFFER_H

/**
 * Represents a color. Contains red, green, and blue elements, which range from
 * 0 to 255.
 */
class Color {
private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
public:
	Color();
	Color(unsigned char red, unsigned char green, unsigned char blue);
	Color(const Color&);
	virtual ~Color();
	virtual const Color& operator=(const Color&);
	
	virtual const unsigned char& red() const;
	virtual const unsigned char& green() const;
	virtual const unsigned char& blue() const;
	
	virtual unsigned char& red();
	virtual unsigned char& green();
	virtual unsigned char& blue();
	
	virtual bool operator==(const Color&) const;
	
	virtual Color operator+(const Color&) const;
	virtual Color operator-(const Color&) const;
	virtual Color operator*(unsigned char) const;
	virtual Color operator/(unsigned char) const;
	
	virtual const Color& operator+=(const Color&);
	virtual const Color& operator-=(const Color&);
	virtual const Color& operator*=(unsigned char);
	virtual const Color& operator/=(unsigned char);
};

/**
 * @deprecated
 * Represents a virtual screen of pixels, where all the pixel colors are set individually. This has been
 * deprecated in favor of the new ShapeManager approach, which presents a better interface to the users
 * and has more opportunity for optimization, both in operation speed and network usage.
 */
class PixelBuffer {
private:
	unsigned int xmax;
	unsigned int ymax;
	Color** data;
	
	void deleteData();
public:
	PixelBuffer(unsigned int xmax, unsigned int ymax);
	PixelBuffer(const PixelBuffer&);
	PixelBuffer(PixelBuffer&&);
	virtual ~PixelBuffer();
	virtual const PixelBuffer& operator=(const PixelBuffer&);
	
	virtual const Color& operator()(unsigned int x, unsigned int y) const;
	virtual Color& operator()(unsigned int x, unsigned int y);
	
	virtual const unsigned int& width() const;
	virtual const unsigned int& height() const;
	
	virtual const unsigned int& maxX() const;
	virtual const unsigned int& maxY() const;
	
	virtual PixelBuffer resize(unsigned int width, unsigned int height) const;
	
	template<class T>
	void forAll(T func) {
		for(unsigned int x = 0; x < maxX(); x++) {
			for(unsigned int y = 0; y < maxY(); y++) {
				func(x, y, data[x][y]);
			}
		}
	}
	
	template<class T>
	void forAll(T func) const {
		for(unsigned int x = 0; x < maxX(); x++) {
			for(unsigned int y = 0; y < maxY(); y++) {
				func(x, y, data[x][y]);
			}
		}
	}
};

#endif