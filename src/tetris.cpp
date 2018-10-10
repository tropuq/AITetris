#include "../include/tetris.h"
#include "../include/line_shape.h"
#include <iostream>

const V2i Tetris::ACTION_DIR[5] = {{0, 0},   // STAY
                                   {-1, 0},  // LEFT
                                   {1, 0},   // RIGHT
                                   {0, -1},  // DOWN
                                   {0, -1}}; // FAST_DOWN

const Color Tetris::COLORS[9] = {Color::Yellow,         // SQUARE
                                 Color::Cyan,           // LONGER
                                 Color::Blue,           // LLEFT
                                 Color(255,165,0),      // LRIGHT
                                 Color::Green,          // ZRIGHT
                                 Color::Red,            // ZLEFT
                                 Color(148,0,211),      // T
                                 Color::White,          // BLANK
                                 Color(127, 127, 127)}; // PREDICT
// {{0, 0}, {1, 0}, {0, -1}, {1, -1}},
const vector<V2i> Tetris::Tetro::BLOCK[7] = {
      {{0, 0}, {1, 0}, {0, -1}, {1, -1}},    // SQUARE
	   {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},     // LONGER
	   {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}}, // LLEFT
	   {{-1, -1}, {0, -1}, {1, -1}, {1, 0}},  // LRIGHT
	   {{-1, -1}, {0, -1}, {0, 0}, {1, 0}},   // ZRIGHT
	   {{-1, 0}, {0, 0}, {0, -1}, {1, -1}},   // ZLEFT
	   {{-1, -1}, {0, -1}, {0, 0}, {1, -1}}}; // T

///////// Board

Tetris::Board::Board(const V2i &_size, const Tetro &_tetro) 
	: tetro(_tetro) {
	size = _size;
	block.resize(size.y, vector<int>(size.x, Tetro::BLANK));
}

void Tetris::Board::restart() {
	block.clear();
	block.resize(size.y, vector<int>(size.x, Tetro::BLANK));
}

size_t Tetris::Board::breakLines() {
	vector<vector<int>> nblock;
	int broken = 0;
	for (auto &i : block) {
		if (find(i.begin(), i.end(), Tetro::BLANK) == i.end())
			broken++;
		else
			nblock.emplace_back(i);
	}
	block = nblock;
	while ((int)block.size() != size.y)
		block.emplace_back(size.x, Tetro::BLANK);
	return broken * broken;
}

size_t Tetris::Board::applyTetro() {
	for (auto &i : tetro.block)
		block[tetro.pos.y + i.y][tetro.pos.x + i.x] = tetro.type;
	return breakLines();
}

///////// Tetro

Tetris::Tetro::Tetro(const Board &_board) 
	: board(_board) {
	regen();
}

void Tetris::Tetro::restart() {
	type = next_type = stash_type = BLANK;
	pos = predict_pos = {0, 0};
	block.clear();
	regen();
}

bool Tetris::Tetro::regen(Tetro::TETRO_TYPE tp) {
	if (tp == BLANK) {
		if (next_type == BLANK) {
			type = TETRO_TYPE(rand() % 7);
			next_type = TETRO_TYPE(rand() % 7);
		}
		else {
			type = next_type;
			next_type = TETRO_TYPE(rand() % 7);
		}
	}
	else
		type = tp;
	
	pos = {(board.size.x - 1) / 2, board.size.y - 2};
	
	block = BLOCK[type];
	
	preparePrediction();
	
	return !collide(pos, block);
}

bool Tetris::Tetro::rotate() {
	if (type == Tetro::SQUARE)
		return true;
	vector<V2i> nblock = block;
	for (V2i &i : nblock)
		i = V2i(i.y, -i.x);
	if (!collide(pos, nblock)) {
		block = nblock;
		preparePrediction();
		return true;
	}
	return false;
}

bool Tetris::Tetro::collide(const V2i &_pos, const vector<V2i> &_block) {
	for (auto &i : _block) {
		auto tm = _pos + i;
		if (tm.x < 0 || tm.y < 0 || tm.x >= board.size.x || tm.y >= board.size.y ||
		    board.block[tm.y][tm.x] != Tetro::BLANK)
			return true;
	}
	return false;
}

bool Tetris::Tetro::move(const V2i &dir) {
	V2i npos = pos + dir;
	if (!collide(npos, block)) {
		pos = npos;
		preparePrediction();
		return true;
	}
	return false;
}

///////// Other

Tetris::Tetris(const V2i &_size, DeviceHandler &_dev)
	: dev(_dev), board(_size, tetro), tetro(board) {
	stateLog();
}

void Tetris::stateLog() const {
	cout << "STATE" << endl;
	// board
	for (int y = board.size.y - 1; y >= 0 ; y--)
		for (int x = 0; x < board.size.x; x++)
			cout << (board.block[y][x] != Tetro::BLANK) << " \n"[x + 1 == board.size.x];
	vector<vector<bool>> b(board.size.y, vector<bool>(board.size.x));
	// current tetro
	for (auto &i : tetro.block)
		b[tetro.pos.y + i.y][tetro.pos.x + i.x] = true;
	for (int y = board.size.y - 1; y >= 0 ; y--)
		for (int x = 0; x < board.size.x; x++)
			cout << b[y][x] << " \n"[x + 1 == board.size.x];
	cout.flush();
	// next tetro
	// stash
}

void Tetris::rewardLog(float rew) const {
	cout << "REWARD" << endl;
	cout << rew << endl;
	// if (rew != 0)
	// 	cerr << re w << endl;
}

void Tetris::endLog() const {
	cout << "END" << endl;
}

void Tetris::Tetro::preparePrediction() {
	predict_pos = pos;
	while (!collide(predict_pos + ACTION_DIR[DOWN], block))
		predict_pos += ACTION_DIR[DOWN];
}

size_t Tetris::addPoints(size_t pt) {
	size_t ret = pt + combo;
	points += pt + combo;
	if (pt > 0)
		combo++;
	else
		combo = 0;
	return ret;
}

bool Tetris::run() {
	// read next move
	if (dev.keyPressed(Keyboard::Space))
		next_move = FAST_DOWN;
	else if ((dev.keyPressed(Keyboard::LShift) || dev.keyPressed(Keyboard::RShift)) && cur_turn_stash == false)
		next_move = STASH;
	else if (dev.keyPressed(Keyboard::A) || dev.keyPressed(Keyboard::Left))
		next_move = LEFT;
	else if (dev.keyPressed(Keyboard::D) || dev.keyPressed(Keyboard::Right))
		next_move = RIGHT;
	else if (dev.keyHeld(Keyboard::S) || dev.keyHeld(Keyboard::Down))
		next_move = DOWN;
	else if (dev.keyPressed(Keyboard::W) || dev.keyPressed(Keyboard::Up))
		next_move = ROTATE;
	
	down_counter.first--;
	move_counter.first--;
	
	// move
	bool moved = false, blocked = false, down = false;
	if (move_counter.first == 0) {
		// make action
		if (next_move == ROTATE)
			blocked = !tetro.rotate();
		else if (next_move == FAST_DOWN) {
			while (tetro.move(ACTION_DIR[next_move]));
			blocked = true;
		}
		else if (next_move == STASH) {
			Tetro::TETRO_TYPE tm = tetro.type;
			if (!tetro.regen(tetro.stash_type)) {
				rewardLog(-4);
				endLog();
				return false;
			}
			tetro.stash_type = tm;
			cur_turn_stash = true;
		}
		else
			blocked = !tetro.move(ACTION_DIR[next_move]);
		
		// reset counters
		move_counter.first = move_counter.second;
		if (next_move == DOWN || next_move == FAST_DOWN) {
			down_counter.first = down_counter.second;
			down = true;
		}
		
		moved = next_move != STAY;
		
		next_move = STAY;
	}
	if (down_counter.first == 0) {
		moved = true;
		blocked = !tetro.move(ACTION_DIR[DOWN]);
		down_counter.first = down_counter.second;
		down = true;
	}
	
	// next tetro
	if (blocked == true && down == true) {
		size_t pt = board.applyTetro();
		pt = addPoints(pt);
		if (!tetro.regen()) {
			rewardLog(-4);
			endLog();
			return false;
		}
		rewardLog(pt);
		stateLog();
		cur_turn_stash = false;
	}
	else if (moved) {
		if (blocked == true)
			rewardLog(-0.1);
		else
			rewardLog(0);
		stateLog();
	}
	
	return true;
}

void Tetris::draw(RenderTarget& target, RenderStates states) const {
	// prepare board
	vector<vector<int>> b = board.block;
	for (auto &i : tetro.block)
		b[tetro.predict_pos.y + i.y][tetro.predict_pos.x + i.x] = Tetro::PREDICT;
	for (auto &i : tetro.block)
		b[tetro.pos.y + i.y][tetro.pos.x + i.x] = tetro.type;
	reverse(b.begin(), b.end());
	
	// draw board
	RectangleShape block({block_siz - 2, block_siz - 2});
	block.setOutlineThickness(1);
	block.setOutlineColor(Color::Black);
	
	for (int y = board.size.y - 1; y >= 0 ; y--) {
		for (int x = 0; x < board.size.x; x++) {
			block.setPosition(x * block_siz, y * block_siz);
			block.setFillColor(COLORS[b[y][x]]);
			target.draw(block, states);
		}
	}
	
	auto drawBlock = [&](Tetro::TETRO_TYPE t, float add_y) {
		auto &tm = Tetro::BLOCK[t];
		block.setFillColor(COLORS[t]);
		for (auto &i : tm) {
			block.setPosition((board.size.x + 1) * block_siz + i.x * block_siz + 5,
			                 -i.y * block_siz + block_siz + 5 + add_y);
			target.draw(block, states);
		}
	};
	
	// draw stash
	if (tetro.stash_type != Tetro::BLANK)
		drawBlock(tetro.stash_type, 0);
	
	// draw next tetro
	if (tetro.next_type != Tetro::BLANK)
		drawBlock(tetro.next_type, 2 * block_siz + 5);
}

void Tetris::restart() {
	down_counter = {30, 30};
	move_counter = {3, 3};
	points = 0;
	combo = 0;
	cur_turn_stash = false;
	next_move = STAY;
	board.restart();
	tetro.restart();
	stateLog();
}
