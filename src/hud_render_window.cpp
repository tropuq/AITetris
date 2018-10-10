#include "../include/hud_render_window.h"
using namespace sf;

HudRenderWindow::HudRenderWindow(RenderWindow& a)
	: wind(a) {}
		
void HudRenderWindow::draw(const sf::Drawable& drawable, const sf::RenderStates& states) const {
	sf::Vector2u res = wind.getSize();
	sf::View view_HUD(sf::FloatRect(0, 0, res.x, res.y));
	sf::View view_prev = wind.getView();
	
	wind.setView(view_HUD);
	wind.draw(drawable, states);
	wind.setView(view_prev);
}
