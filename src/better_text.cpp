#include "../include/better_text.h"
#include <iostream>
using namespace std;
using namespace sf;

BetterText::BetterText() {
	setCharacterSize(default_character_size);
	setup();
}

BetterText::BetterText(const String& string, const Font& font, unsigned int characterSize) {
	text = Text(string, font, characterSize);
	setup();
}

void BetterText::setup() {
	siz_set.y = getCharacterSize();
	nlines = getNumberOfLines();
	size_by_line = false;
	scal = Vector2f(1, 1);
	max_size_set = false;
	update();
}

//////////////////////////////////////////////////////////////// getters

const Vector2f& BetterText::getPosition() const {
	return pos;
}


Vector2f BetterText::getSize() const {
	auto tm = getGlobalBounds();
	return {tm.width, tm.height};
}

float BetterText::getLineHeight() const {
	return getHeight() / nlines;
}

float BetterText::getHeight() const {
	return getSize().y;
}

float BetterText::getWidth() const {
	return getSize().x;
}

const Color& BetterText::getColor() const {
	return text.getColor();
}

const String& BetterText::getString() const {
	return text.getString();
}

const Font* BetterText::getFont() const {
	return text.getFont();
}

unsigned int BetterText::getCharacterSize() const {
	return text.getCharacterSize();
}

Uint32 BetterText::getStyle() const {
	return text.getStyle();
}


// float BetterText::getRotation() const {
// 	return rot;
// }

const Vector2f& BetterText::getScale() const {
	return scal;
}

const Vector2f& BetterText::getOrigin() const {
	return ori;
}

FloatRect BetterText::getLocalBounds() const {
	return text.getLocalBounds();
}

FloatRect BetterText::getGlobalBounds() const {
	return text.getGlobalBounds();
}


const Transform& BetterText::getTransform() const {
	return text.getTransform();
}

const Transform& BetterText::getInverseTransform() const {
	return text.getInverseTransform();
}


const Text& BetterText::getText() const {
	return text;
}

//////////////////////////////////////////////////////////////// setters

void BetterText::setPosition(const Vector2f& position) {
	pos = position;
	update();
}

void BetterText::setPosition(float x, float y) {
	setPosition({x, y});
}

void BetterText::move(float offsetX, float offsetY) {
	setPosition(pos + Vector2f(offsetX, offsetY));
}

void BetterText::move(const Vector2f& offset) {
	setPosition(pos + offset);
}


void BetterText::setSize(float width, float height) {
	size_by_line = false;
	siz_set = {width, height};
	update();
}

void BetterText::setSize(const Vector2f& size) {
	setSize(size.x, size.y);
}

void BetterText::setMaxSize(const Vector2f& size) {
	max_size_set = true;
	max_size = size;
}

void BetterText::setMaxSize(float width, float height) {
	max_size_set = true;
	setMaxSize({width, height});
}

void BetterText::setLineHeight(float height) {
	size_by_line = true;
	siz_set = {0, height};
	update();
}

void BetterText::setHeight(float height) {
	setSize(0, height);
}

void BetterText::setWidth(float width) {
	setSize(width, 0);
}

void BetterText::setColor(const Color& color) {
	text.setColor(color);
}

void BetterText::setString(const String& string) {
	text.setString(string);
	nlines = getNumberOfLines();
	update();
}

void BetterText::setFont(const Font& font) {
	text.setFont(font);
	update();
}

void BetterText::setCharacterSize(unsigned int size) {
	text.setCharacterSize(size);
	update();
}

void BetterText::setStyle(Uint32 style) {
	text.setStyle(style);
	update();
}


void BetterText::setScale(float factorX, float factorY) {
	scale(1. / scal.x, 1. / scal.y);
	scale(factorX, factorY);
}

void BetterText::setScale(const Vector2f& factor) {
	setScale(factor.x, factor.y);
}

void BetterText::scale(float factorX, float factorY) {
	scal.x *= factorX;
	scal.y *= factorY;
	update();
}

void BetterText::scale(const Vector2f& factor) {
	scale(factor.x, factor.y);
}

void BetterText::setOrigin(float x, float y) {
	ori = Vector2f(x, y);
	update();
}

void BetterText::setOrigin(const Vector2f& origin) {
	setOrigin(origin.x, origin.y);
}

//////////////////////////////////////////////////////////////// other

void BetterText::removeMaxSize() {
	max_size_set = false;
}

Vector2f BetterText::findCharacterPos(size_t index) const {
	return text.findCharacterPos(index);
}

void BetterText::updateSize() {
	siz_real = siz_set;
	if (size_by_line)
		siz_real = Vector2f(siz_real.x, siz_real.y * nlines);
	
	if(siz_real.x == 0.0 && siz_real.y != 0.0)
		siz_real.x = getGlobalBounds().width * siz_real.y / getGlobalBounds().height;
	else if(siz_real.y == 0.0 && siz_real.x != 0.0)
		siz_real.y = getGlobalBounds().height * siz_real.x / getGlobalBounds().width;
	if(siz_real.y != 0.0 && siz_real.x != 0.0)
		text.setScale(siz_real.x / getGlobalBounds().width, siz_real.y / getGlobalBounds().height);
	text.scale(scal.x, scal.y);
	if (max_size_set) {
		if (getGlobalBounds().width > max_size.x) {
			text.scale(max_size.x / getGlobalBounds().width, max_size.x / getGlobalBounds().width);
		}
		if (getGlobalBounds().height > max_size.y) {
			text.scale(max_size.y / getGlobalBounds().height, max_size.y / getGlobalBounds().height);
		}
	}
}

int BetterText::getNumberOfLines() {
	int ret = 1;
	for (char i : getString())
		ret += i == '\n';
	return ret;
}

Vector2f BetterText::getRealPosition(const Vector2f& position) {
	Vector2f tm_pos(getPosition());
	Vector2f tm_bound(getGlobalBounds().left, getGlobalBounds().top);
	return position - (tm_bound - tm_pos);
}

static const double EPS = 1e-5;
void BetterText::update() {
	if (getFont() == NULL || getString().isEmpty() || (abs(siz_set.x) < EPS && abs(siz_set.y) < EPS) || (abs(scal.x) < EPS && 
		 abs(scal.y) < EPS) || abs(getGlobalBounds().height) < EPS || abs(getGlobalBounds().width) < EPS || 
		 abs(getLocalBounds().width) < EPS || abs(getLocalBounds().height) < EPS)
		return;
	
	// nie wiem po co to, ale naprawia
	text.scale(getLocalBounds().width / getGlobalBounds().width, getLocalBounds().height / getGlobalBounds().height);

	updateSize();
	
	text.setPosition(pos);
	text.setPosition(getRealPosition(pos - Vector2f(ori.x * scal.x, ori.y * scal.y)));
}

void BetterText::draw(RenderTarget& target, RenderStates states) const {
	target.draw(text, states);
}