#pragma once
#include <map>
#include <set>
#include <SFML/Graphics.hpp>
#include <vector>
#include "device_handler.h"
#include "better_text.h"
using namespace std;
using namespace sf;

typedef Vector2i V2i;
typedef Vector2f V2f;
typedef pair<size_t, size_t> PSS;

class Tetris : public Drawable {
private:
	void draw(RenderTarget& target, RenderStates states) const;
	float block_siz = 25;
	
	static const Color COLORS[];
	static const V2i ACTION_DIR[];
	enum ACTION {
		STAY,
		LEFT,
		RIGHT,
		DOWN,
		FAST_DOWN,
		ROTATE,
		STASH
	};
	ACTION next_move = STAY;
	
	DeviceHandler &dev;
	
	// (cur, max)
	PSS down_counter = {30, 30};
	PSS move_counter = {3, 3};
	
	size_t addPoints(size_t pt);
	size_t points = 0;
	size_t combo = 0;
	
	struct Tetro;
	
	struct Board {
		V2i size;
		vector<vector<int>> block;
		const Tetro &tetro;
		
		Board(const V2i &size, const Tetro &tetro);
		
		void restart();
		size_t breakLines();
		size_t applyTetro();
	} board;
	
	struct Tetro {
		enum TETRO_TYPE {
			SQUARE,
			LONGER,
			LLEFT,
			LRIGHT,
			ZRIGHT,
			ZLEFT,
			T,
			BLANK,
			PREDICT,
		};
		static const vector<V2i> BLOCK[];
		
		V2i pos, predict_pos;
		vector<V2i> block;
		const Board &board;
		TETRO_TYPE type = BLANK, next_type = BLANK;
		TETRO_TYPE stash_type = BLANK;

		Tetro(const Board &board);

		void restart();
		void preparePrediction();
		bool collide(const V2i &pos, const vector<V2i> &block);
		bool regen(TETRO_TYPE tp = BLANK);
		bool move(const V2i &dir);
		bool rotate();
	} tetro;
	bool cur_turn_stash = false;
	
	void stateLog() const;
	void rewardLog(float rew) const;
	void endLog() const;
	
public:
	Tetris(const V2i &size, DeviceHandler &dev);
	
	bool run();
	void restart();

};