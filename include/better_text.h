#pragma once
#include <SFML/Graphics.hpp>

class BetterText : public sf::Drawable {
public:
	BetterText();
	BetterText(const sf::String& string, const sf::Font& font, unsigned int characterSize = 400);
	
	////////// getters
	
	const sf::Vector2f& getPosition() const;
	
	sf::Vector2f getSize() const;
	float getLineHeight() const;
	float getHeight() const;
	float getWidth() const;
	
	const sf::Color& getColor() const;
	const sf::String& getString() const;
	const sf::Font* getFont() const;
	unsigned int getCharacterSize() const;
	sf::Uint32 getStyle() const;
	
	// float getRotation() const; // not implemented
	const sf::Vector2f& getScale() const;
	const sf::Vector2f& getOrigin() const;
	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
	
	const sf::Transform& getTransform() const;
	const sf::Transform& getInverseTransform() const;
	
	////////// setters
	
	void setPosition(const sf::Vector2f& position);
	void setPosition(float x, float y);
	void move(float offsetX, float offsetY);
	void move(const sf::Vector2f& offset);
	
	void setSize(float width, float height);
	void setSize(const sf::Vector2f& size);
	void setMaxSize(float width, float height); // restrictions for the bounding box of the text
	void setMaxSize(const sf::Vector2f& size); // restrictions for the bounding box of the text
	void setLineHeight(float height);
	void setHeight(float height);
	void setWidth(float width);
	
	void removeMaxSize(); // turn off bounding box restriction
	
	void setColor(const sf::Color& color);
	void setString(const sf::String& string);
	void setFont(const sf::Font& font);
	void setCharacterSize(unsigned int size);
	void setStyle(sf::Uint32 style);
	
	// void setRotation(float angle); // not implemented
	// void rotate(float angle); // not implemented
	void setScale(float factorX, float factorY);
	void setScale(const sf::Vector2f& factors);
	void scale(float factorX, float factorY);
	void scale(const sf::Vector2f& factor);
	void setOrigin(float x, float y);
	void setOrigin(const sf::Vector2f& origin);
	
	////////// other
	
	sf::Vector2f findCharacterPos(std::size_t index) const;

protected:
	const sf::Text& getText() const;
private:
	void setup(); // setup default options
	void fullUpdate(); // run updateSize() and update()
	void updateSize(); // update sf::Text size after transformations
	int getNumberOfLines(); // returns number of lines in text.getStirng()
	void update(); // update sf::Text attribute using BetterText transformations
	sf::Vector2f getRealPosition(const sf::Vector2f& position); // transform 'position' into real text position
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	sf::Text text; // sf::Text object to manipulate
	sf::Vector2f pos, siz_set, siz_real, ori, scal; // position, size set with setter, real size, origin, scale
	// float rot; // rotation
	int nlines; // number of lines in text.getString()
	bool size_by_line; // true - text height is nlines * siz.y and need to be updated
	bool max_size_set; // is size of the bounding box restricted
	sf::Vector2f max_size; // size of the bounding box resctriction

	const int default_character_size = 400;
};
