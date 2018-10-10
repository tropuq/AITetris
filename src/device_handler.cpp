#include "../include/device_handler.h"
using namespace std;
using namespace sf;

DeviceHandler::DeviceHandler(RenderWindow& _wind)
	: wind(_wind) {}

void DeviceHandler::pollEvents() {
	Event event;
	devh.clear();
	while (wind.pollEvent(event)) {
		events[event.type].emplace_back(event);
		if (event.type == Event::KeyPressed) {
			keyhold[event.key.code] = true;
			count[event.key.code]++;
			devh[{event.type, event.key.code}]++;
		}
		else if (event.type == Event::KeyReleased) {
			keyhold[event.key.code] = false;
			devh[{event.type, event.key.code}]++;
		}
		else if (event.type == Event::MouseButtonPressed) {
			buttonhold[event.mouseButton.button] = true;
			devh[{event.type, event.key.code}]++;
		}
		else if (event.type == Event::MouseButtonReleased) {
			buttonhold[event.mouseButton.button] = false;
			devh[{event.type, event.key.code}]++;
		}
		else
			devh[{event.type, -1}]++;
	}
}

vector<Event> DeviceHandler::getEventList() {
	vector<Event> ret;
	for (auto& i : events)
		for (auto& j : i.second)
			ret.emplace_back(j);
	return ret;
}

vector<Event> DeviceHandler::getEventList(Event::EventType type) {
	vector<Event> ret;
	for (auto& i : events[type])
		ret.emplace_back(i);
	return ret;
}

size_t DeviceHandler::isActive(Event::EventType type, int code) {
	if (devh.find({type, code}) == devh.end())
		return 0;
	return devh[{type, code}];
}

size_t DeviceHandler::keyPressed(Keyboard::Key code) {
	return isActive(Event::KeyPressed, code);
}
bool DeviceHandler::keyHeld(Keyboard::Key code) {
	return keyhold[code];
}
size_t DeviceHandler::keyReleased(Keyboard::Key code) {
	return isActive(Event::KeyReleased, code);
}

size_t DeviceHandler::mouseButtonPressed(Mouse::Button button) {
	return isActive(Event::MouseButtonPressed, button);
}
bool DeviceHandler::mouseButtonHeld(Mouse::Button button) {
	return buttonhold[button];
}
size_t DeviceHandler::mouseButtonReleased(Mouse::Button button) {
	return isActive(Event::MouseButtonReleased, button);
}

size_t DeviceHandler::mouseWheelScrolled() {
	return isActive(Event::MouseWheelScrolled);
}

size_t DeviceHandler::mouseMoved() {
	return isActive(Event::MouseMoved);
}

size_t DeviceHandler::closePressed() {
	return isActive(Event::Closed);
}

sf::Vector2i DeviceHandler::getMousePosition() {
	return Mouse::getPosition();
}

size_t DeviceHandler::counter(sf::Keyboard::Key code) {
	return count[code];
}