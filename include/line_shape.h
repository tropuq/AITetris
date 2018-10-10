#pragma once
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>


class SFML_GRAPHICS_API LineShape : public sf::Shape {
public :

	LineShape(const sf::Vector2f& point1 = {0, 0}, const sf::Vector2f& point2 = {0, 0});

	////////// setters
	
	void setPoint1(sf::Vector2f point);
	void setPoint2(sf::Vector2f point);
	
	void setPoint1(float x, float y);
	void setPoint2(float x, float y);
	
	void setThickness(float thickness);

	////////// getters
		
	const sf::Vector2f& getPoint1() const;
	const sf::Vector2f& getPoint2() const;
	
	float getThickness() const;
	float getLength() const;

	virtual size_t getPointCount() const;
	virtual sf::Vector2f getPoint(size_t index) const;

private :
	
	sf::Vector2f p1, p2;
	sf::Vector2f m_direction; // direction of the line
	float m_thickness;    // thickness of the line
	
	const float default_thickness = 2.;
};

