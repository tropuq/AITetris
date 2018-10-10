#include "../include/useful.h"
using namespace std;
using namespace sf;

Color contrastColor(Color color) {
	if(color.r * 0.299 + color.g * 0.587 + color.b * 0.114 < 128)
		return Color::White;
	else
		return Color::Black;
}

struct ColorPoint {
	float r, g, b;
	float val;        // Position of color along the gradient (between 0 and 1).
	ColorPoint(float red, float green, float blue, float value)
	: r(red), g(green), b(blue), val(value) {}
};

Color heatMapColor(float value) {
	vector<ColorPoint> color;
	color.push_back(ColorPoint(0, 0, 255,   0.0f));      // Blue.
	color.push_back(ColorPoint(0, 255, 255,   0.25f));     // Cyan.
	color.push_back(ColorPoint(0, 255, 0,   0.5f));      // Green.
	color.push_back(ColorPoint(255, 255, 0,   0.75f));     // Yellow.
	color.push_back(ColorPoint(255, 0, 0,   1.0f));      // Red.
 
	Color ret;
	for(size_t i = 0; i < color.size(); i++) {
		ColorPoint &currC = color[i];
		if(value < currC.val) {
			ColorPoint &prevC  = color[max(0, (int)i - 1)];
			float valueDiff    = (prevC.val - currC.val);
			float fractBetween = abs(valueDiff) < 1e-12 ? 0 : (value - currC.val) / valueDiff;
			ret.r = (prevC.r - currC.r)*fractBetween + currC.r;
			ret.g = (prevC.g - currC.g)*fractBetween + currC.g;
			ret.b = (prevC.b - currC.b)*fractBetween + currC.b;
			return ret;
		}
	}
	return Color(color.back().r, color.back().g, color.back().b);
}