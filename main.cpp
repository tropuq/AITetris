#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>
#include <cassert>
#include "include/all.h"
#include "include/tetris.h"
using namespace std;
using namespace sf;

typedef Vector2i V2i;
typedef Vector2f V2f;
typedef pair<int, int> PII;

///////////////////////////////////////////////////////////////////////////////

RenderWindow window;
HudRenderWindow hud_window(window);

V2i RES(360, 500);
Color BACKGROUND(127, 127, 127);

Font ubuntu_mono, arial;

// fps
Clock cl; // frame clock
const int fps_limit = 60; // max fps
double real_fps = fps_limit; // measured fps

DeviceHandler dev(window);

///////////////////////////////  GAME FUNCTIONS  //////////////////////////////

void handleEvents() {
	dev.pollEvents();

	if (dev.closePressed() || dev.keyPressed(Keyboard::Escape))
		window.close();
}

void frameEnd() {
	real_fps = 1.0 / cl.getElapsedTime().asSeconds();	
	BetterText fps_txt(to_stringp(real_fps, 0), ubuntu_mono);
	fps_txt.setMaxSize(40, 20);
	fps_txt.setPosition(RES.x - 42, 2);
	
	draw(hud_window, fps_txt);
	
	cl.restart();
	window.display();
	window.clear(BACKGROUND);
}

///////////////////////////////  MAIN PART  ///////////////////////////////

void initVisualization() {
	window.create(VideoMode(RES.x, RES.y), "Tetris");
	window.setFramerateLimit(fps_limit);
	assert(ubuntu_mono.loadFromFile("fonts/ubuntu_mono.ttf"));
	assert(arial.loadFromFile("fonts/arial.ttf"));
}

int main(int argc, char **argv) {
	initVisualization();
	
	if (argc == 2 && strcmp(argv[1], "wait") == 0) {
		string z;
		cin >> z;
	}
	
	Tetris tetris({10, 20}, dev);
	
	while (window.isOpen()) {
		handleEvents();
		
		if (!tetris.run())
			tetris.restart();
		
		window.draw(tetris);
		
		frameEnd();
	}

	return 0;
}
/*
*/

