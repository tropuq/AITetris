#pragma once
#include <map>
#include <set>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;
using namespace sf;

class DeviceHandler {
public:
	DeviceHandler(RenderWindow& _wind);
	
	// gather all events
	void pollEvents();
	
	vector<Event> getEventList(); // get all events
	vector<Event> getEventList(Event::EventType type); // get events of type 'type'
	
	size_t keyPressed(Keyboard::Key code);
	bool keyHeld(Keyboard::Key code);
	size_t keyReleased(Keyboard::Key code);

	size_t mouseButtonPressed(Mouse::Button button);
	bool mouseButtonHeld(Mouse::Button button);
	size_t mouseButtonReleased(Mouse::Button button);
	
	size_t mouseWheelScrolled();
	
	size_t mouseMoved();
	
	size_t closePressed();

	Vector2i getMousePosition();
	
	// return number of presses of the 'code'
	size_t counter(Keyboard::Key code);

private:
	map<Keyboard::Key, size_t> count;
	// stores number of occurences of specific event
	map<pair<Event::EventType, int>, size_t> devh;
	map<Event::EventType, vector<Event>> events; // storing events (get event list of specific type)
	map<Keyboard::Key, bool> keyhold; // for storing currently held keys
	map<Mouse::Button, bool> buttonhold; // for storing currently held mouse buttons

	size_t isActive(Event::EventType type, int code = -1);

	RenderWindow& wind;
};
