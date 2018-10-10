#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

template<class T>
std::ostream &operator<<(std::ostream &ost, const sf::Vector2<T> &_cnt) {
	return ost << '(' << _cnt.x << ", " << _cnt.y << ')';
}

// drawing many things at the same time

template<class Rend, class T1, class T2, class T3>
void _draw(Rend &window, const std::map<T1, T2, T3> &v);

template<class Rend, class T1>
void _draw(Rend &window, const std::vector<T1> &v);

template<class Rend, class T1, size_t T2>
void _draw(Rend &window, const std::array<T1, T2> &v);

template<class Rend, class T1>
void _draw(Rend &window, const T1 &a) {
	window.draw(a);
}

template<class Rend, class T1, class T2, class T3>
void _draw(Rend &window, const std::map<T1, T2, T3> &v) {
	for (auto &i : v)
		_draw(window, i.second);
}
template<class Rend, class T1>
void _draw(Rend &window, const std::vector<T1> &v) {
	for (auto &i : v)
		_draw(window, i);
}
template<class Rend, class T1, size_t T2>
void _draw(Rend &window, const std::array<T1, T2> &v) {
	for (auto &i : v)
		_draw(window, i);
}

template<class Rend, class T1>
void draw(Rend &window, T1 a) {
	_draw(window, a);
}

template<class Rend, class T1, class... Args>
void draw(Rend &window, T1 a, Args... args) {
	_draw(window, a);
	draw(window, args...);
}

// returns color contrasting with 'color'
sf::Color contrastColor(sf::Color color);


sf::Color heatMapColor(float value);

// returns drawable bounding box of an object
template<class A>
sf::VertexArray getBoundingBox(const A &a, sf::Color color = sf::Color::Red) {
	sf::FloatRect tmp = a.getGlobalBounds();
	float mnx = tmp.left, mny = tmp.top, mxx = tmp.left + tmp.width, mxy =  tmp.top + tmp.height;
	sf::VertexArray strip(sf::LinesStrip);
	strip.append(sf::Vertex({mnx, mny}, color));
	strip.append(sf::Vertex({mnx, mxy}, color));
	strip.append(sf::Vertex({mxx, mxy}, color));
	strip.append(sf::Vertex({mxx, mny}, color));
	strip.append(sf::Vertex({mnx, mny}, color));
	return strip;
}

template<class A>
void fitIntoBox(A &a, sf::Vector2f siz) {
	siz = {std::max(1.f, siz.x), std::max(1.f, siz.y)};
	sf::Vector2f tmp(a.getGlobalBounds().width / a.getScale().x, a.getGlobalBounds().height / a.getScale().y);
	float sc1 = siz.y / tmp.y;
	if(tmp.x * sc1 <= siz.x)
		return a.setScale(sc1, sc1);
	float sc2 = siz.x / tmp.x;
	a.setScale(sc2, sc2);
}

template <typename T>
std::string to_stringp(const T a_value, const int n = 3) {
	std::ostringstream out;
	out << std::setprecision(n) << std::fixed << a_value;
	return out.str();
}