#ifndef POINT_H
#define POINT_H

#include "Util.h"

/**
 * Represents a coordinate in n-dimensional space, templated to have the type system enforce lengths
 * and to support integer and floating point coordinate systems.
 */
template<unsigned int dim, class CoordType>
class Point {
private:
    CoordType coords[dim];
public:
    typedef Point<dim, CoordType> Type;
    
    Point() {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] = 0;
        }
    }
    Point(const std::initializer_list<CoordType>& init) {
        if(init.size() == dim) {
            int index = 0;
            for(auto it = init.begin(); it != init.end(); it++) {
                coords[index] = *it;
                index++;
            }
        } else {
            throw std::runtime_error("Given initializer list has length " + toStr(init.size()) + " but this is a point of dimension " + toStr(dim));
        }
    }
    Point(std::istream& in) {
        in >> (*this);
    }
    Point(const Type& other) {
        (*this) = other;
    }
    ~Point() {
        
    }
    inline const Type& operator=(const Type& other) {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] = other[i];
        }
        return *this;
    }
    
    inline CoordType& operator[](const unsigned int& i) {
        if(i < dim) {
            return coords[i];
        } else {
            throw std::runtime_error("Given index " + toStr(i) + " is less than the dimension " + toStr(dim));
        }
    }
    
    inline const CoordType& operator[](const unsigned int& i) const {
        if(i < dim) {
            return coords[i];
        } else {
            throw std::runtime_error("Given index " + toStr(i) + " is less than the dimension " + toStr(dim));
        }
    }
    
    inline Type operator+(const Type& other) const {
        Type ret;
        for(unsigned int i = 0; i < dim; i++) {
            ret[i] = coords[i] + other[i];
        }
        return ret;
    }
    
    inline Type operator+() const {
        return Type(*this);
    }
    
    inline const Type& operator+=(const Type& other) {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] += other[i];
        }
        return *this;
    }
    
    inline Type operator-(const Type& other) const {
        Type ret;
        for(unsigned int i = 0; i < dim; i++) {
            ret[i] = coords[i] - other[i];
        }
        return ret;
    }
    
    inline Type operator-() const {
        Type ret;
        for(unsigned int i = 0; i < dim; i++) {
            ret[i] = -(coords[i]);
        }
        return ret;
    }
    
    inline const Type& operator-=(const Type& other) {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] -= other[i];
        }
        return *this;
    }
    
    inline Type operator*(const CoordType& mod) const {
        Type ret;
        for(unsigned int i = 0; i < dim; i++) {
            ret[i] = coords[i] * mod;
        }
        return ret;
    }
    
    inline const Type& operator*=(const CoordType& mod) {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] *= mod;;
        }
        return *this;
    }
    
    inline Type operator/(const CoordType& mod) const {
        Type ret;
        for(unsigned int i = 0; i < dim; i++) {
            ret[i] = coords[i] / mod;
        }
        return ret;
    }
    
    inline const Type& operator/=(const CoordType& mod) {
        for(unsigned int i = 0; i < dim; i++) {
            coords[i] /= mod;;
        }
        return *this;
    }
    
    inline bool operator==(const Type& other) {
        for(unsigned int i = 0; i < dim; i++) {
            if(coords[i] != other[i]) {
                return false;
            }
        }
        return true;
    }
    
    inline bool operator!=(const Type& other) {
        for(unsigned int i = 0; i < dim; i++) {
            if(coords[i] != other[i]) {
                return true;
            }
        }
        return false;
    }
};

/**
 * Produces serialized output for an n-dimensional point.
 */
template<unsigned int dim, class CoordType>
inline std::ostream& operator<<(std::ostream& strm, const Point<dim, CoordType>& pt) {
    for(unsigned int i = 0; i < dim - 1; i++) {
        strm << pt[i] << " ";
    }
    return strm << pt[dim-1];
}

/**
 * Deserializes an n-dimensional point.
 */
template<unsigned int dim, class CoordType>
inline std::istream& operator>>(std::istream& in, Point<dim, CoordType>& pt) {
    for(unsigned int i = 0; i < dim; i++) {
        in >> pt[i];
    }
    return in;
}

/**
 * Convenience point types.
 */
typedef Point<2, double> Point2d;
typedef Point<2, float> Point2f;
typedef Point<2, int> Point2i;

typedef Point<3, double> Point3d;
typedef Point<3, float> Point3f;
typedef Point<3, int> Point3i;

#endif