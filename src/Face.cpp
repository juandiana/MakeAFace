#include "Face.h"



Face::Face() {
}

void Face::setup() {
	img[0].loadImage("face_happy.png");
	img[1].loadImage("face_surprised.png");
	img[2].loadImage("face_angry.png");

	for (int i = 0; i < 3; i++) {
		img[i].setAnchorPoint(img[i].getWidth()/2, img[i].getHeight()/2);
	}

	y = -100;
	type = 0;
}

void Face::update() {
	x = ofGetWindowWidth() / 2;
	
	int newY = (y + 4) % ofGetWindowHeight();

	if (y > newY) {
		type = ofRandom(0, 3);
	}

	y = newY;
}

void Face::draw() {
	ofSetColor(255, 255, 255);
	img[type].draw(x, y);
}

int Face::getShotScore(int exprType) {
	int score;

	if (exprType == type + 1) {
		float dist = ofDist(x, y, 300, 652);

		if (dist < 10) {
			score = 100;
		} else if (dist < 50) {
			score = 50;
		} else if (dist < 100) {
			score = 10;
		} else {
			score = 0;
		}
	} else {
		score = 0;
	}

	return score;
}

Face::~Face() {
}
