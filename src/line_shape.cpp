#include "../include/line_shape.h"
#include <cmath>
using namespace sf;
using namespace std;

LineShape::LineShape(const Vector2f& point1, const Vector2f& point2)
	: p1(point1), p2(point2), m_direction(point2 - point1) {
	setPosition(p1);
	setThickness(default_thickness);    
}

void LineShape::setThickness(float thickness) {
	m_thickness = thickness;
	update();
}

void LineShape::setPoint1(sf::Vector2f point1) {
	p1 = point1;
	setPosition(p1);
	m_direction = p1 - p2;
	update();
}

void LineShape::setPoint2(sf::Vector2f point2) {
	p2 = point2;
	m_direction = p1 - p2;
	update();
}

void LineShape::setPoint1(float x, float y) {
	p1 = {x, y};
	setPosition(p1);
	m_direction = p1 - p2;
	update();
}

void LineShape::setPoint2(float x, float y) {
	p2 = {x, y};
	m_direction = p1 - p2;
	update();
}


float LineShape::getThickness() const {
	return m_thickness;
}

const sf::Vector2f& LineShape::getPoint1() const {
	return p1;
}

const sf::Vector2f& LineShape::getPoint2() const {
	return p2;
}


float LineShape::getLength() const {
	return std::sqrt(m_direction.x*m_direction.x+m_direction.y*m_direction.y);
}


size_t LineShape::getPointCount() const {
	return 4;
}


Vector2f LineShape::getPoint(size_t index) const {
	Vector2f unitDirection = m_direction/getLength();
	Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

	Vector2f offset = (m_thickness/2.f)*unitPerpendicular;

	switch (index) {
		default:
		case 0: return offset;
		case 1: return (m_direction + offset);
		case 2: return (m_direction - offset);
		case 3: return (-offset);
	}
}

