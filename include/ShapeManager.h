#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H

#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <ostream>
#include <sstream>
#include <functional>

#include "BinaryStream.h"
#include "PixelBuffer.h"
#include "Point.h"
#include "Shape.h"
#include "Util.h"

/**
 * 
 */
template<unsigned int dim, class CoordType>
class ShapeManager /*: public ShapeObserver<dim, CoordType> */ {
private:
	std::map<unsigned int, Shape<dim, CoordType>> shapes;
	//std::vector<std::reference_wrapper<ShapeObserver<dim, CoordType>>> observers;
	unsigned int count;
public:
	typedef Point<dim, CoordType> PointType;
	typedef Shape<dim, CoordType> ShapeType;
	
	ShapeManager() : count(0) {}
	ShapeManager(const ShapeManager& other) = default;
	~ShapeManager() = default;
	ShapeManager& operator=(const ShapeManager& other) = default;
	
	inline void insertShape(unsigned int index, ShapeType insert) {
		shapes.insert(std::make_pair(index, insert));
	}
	
	inline ShapeType& operator[](unsigned int index) {
		if(shapes.count(index)) {
			return shapes[index];
		} else {
			throw std::runtime_error("ShapeManager has no shape with index " + toStr(index));
		}
	}
	
	inline const ShapeType& operator[](unsigned int index) const {
		if(shapes.count(index)) {
			return shapes.find(index)->second;
		} else {
			throw std::runtime_error("ShapeManager has no shape with index " + toStr(index));
		}
	}
	
	inline unsigned int numShapes() const {
		return shapes.size();
	}
	
	inline typename std::map<unsigned int, ShapeType>::iterator begin() {
		return shapes.begin();
	}
	
	inline typename std::map<unsigned int, ShapeType>::iterator end() {
		return shapes.end();
	}
	
	inline typename std::map<unsigned int, ShapeType>::const_iterator begin() const {
		return shapes.begin();
	}
	
	inline typename std::map<unsigned int, ShapeType>::const_iterator end() const {
		return shapes.end();
	}
	
	inline unsigned int makeQuad(const PointType& p1, const PointType& p2, const PointType& p3, const PointType& p4) {
		unsigned int index = count;
		shapes[index] = ShapeType(::ShapeType::SHAPE_QUAD, {p1, p2, p3, p4});
		//shapes[index].setObserver(this);
		update(shapes[index]);
		count++;
		return index;
	}
	
	inline unsigned int makeTriangle(const PointType& p1, const PointType& p2, const PointType& p3) {
		unsigned int index = count;
		shapes[index] = ShapeType(::ShapeType::SHAPE_TRIANGLE, {p1, p2, p3});
		//shapes[index].setObserver(this);
		update(shapes[index]);
		count++;
		return index;
	}
	
	inline unsigned int makeLine(const PointType& p1, const PointType& p2) {
		unsigned int index = count;
		shapes[index] = ShapeType(::ShapeType::SHAPE_LINE, {p1, p2});
		//shapes[index].setObserver(this);
		update(shapes[index]);
		count++;
		return index;
	}
	
	inline void deleteShape(unsigned int index) {
		shapes.erase(index);
	}
	
	inline void clear() {
		shapes.clear();
	}
	
	inline void addObserver(ShapeObserver<dim, CoordType>& observer) {
		//observers.push_back(std::ref(observer));
	}
	
	inline void update(const Shape<dim, CoordType>& shape) {
		/*for(auto& observer : observers) {
			observer.get().update(shape);
		}*/
	}
	
	inline bool operator==(const ShapeManager<dim, CoordType>& other) const {
		if(numShapes() != other.numShapes()) {
			return false;
		}
		try {
			for(auto& it : *this) {
				if(!(it.second == other[it.first])) {
					return false;
				}
			}
		} catch(std::runtime_error&) {
			return false;
		}
		return true;
	}
};

template<unsigned int dim, class CoordType>
bostream& operator<<(bostream& out, const ShapeManager<dim, CoordType>& manager) {
	out << manager.numShapes();
	for(auto& it : manager) {
		out << it.first << it.second;
	}
	return out;
}

template<unsigned int dim, class CoordType>
bistream& operator>>(bistream& in, ShapeManager<dim, CoordType>& manager) {
	unsigned int numShapes;
	in >> numShapes;
	ShapeManager<dim, CoordType> newShapeManager;
	for(unsigned int i = 0; i < numShapes; i++) {
		unsigned int index;
		Shape<dim, CoordType> shape;
		in >> index >> shape;
		std::cout << index << ": " << shape << std::endl;
		newShapeManager.insertShape(index, shape);
	}
	manager = newShapeManager;
	return in;
}

template<unsigned int dim, class CoordType>
std::ostream& operator<<(std::ostream& out, const ShapeManager<dim, CoordType>& manager) {
	for(auto& it : manager) {
		out << it.first << ": " << it.second << std::endl;
	}
	return out;
}

typedef ShapeManager<2, double> ShapeManager2d;
typedef ShapeManager<2, float> ShapeManager2f;
typedef ShapeManager<2, int> ShapeManager2i;

#endif