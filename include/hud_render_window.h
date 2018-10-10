#pragma once
#include <SFML/Graphics.hpp>

class HudRenderWindow {
public:
	HudRenderWindow(sf::RenderWindow& a);
	
	// moving doesn't affect those drawings
	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) const;

private:
	sf::RenderWindow& wind;
};