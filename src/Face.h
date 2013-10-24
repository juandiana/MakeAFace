#pragma once

#include "ofMain.h"

class Face {
	public:
		int x;
		int y;
		int type;
		ofImage img[3];

		Face();
		~Face();

		void setup();
		void update();
		void draw();

		int getShotScore(int exprType);
};

