#pragma once

#include "ofMain.h"
#include "Face.h"
#include "ofxFaceTracker.h"

enum expression {
	NEUTRAL,
	HAPPY,
	SURPRISED,
	ANGRY
};

enum gameState {
	INTRO,
	INSTR,
	PLAY,
	SCORE
};

class gameApp: public ofBaseApp {
	private:
		bool debug;
		gameState gameState;

		// Resources
		ofImage intro;
		ofImage instr;
		ofImage scoreboard;
		ofImage background;
		ofImage levelLayer;
		ofImage picturesIcon;
		ofImage clockIcon;
		ofImage wellDoneText;
		ofTrueTypeFont dosisFont24;
		ofTrueTypeFont dosisFont40;
		ofTrueTypeFont dosisFont24bold;
		ofTrueTypeFont dosisFont40bold;
		ofColor darkBlue;

		ofVideoGrabber cam;
		ofxFaceTracker tracker;
		ExpressionClassifier classifier;
		int count;
		expression currentExpression;

		// Variables
		int score;
		int picturesTotal;
		int picturesLeft;
		ofImage flashLight;
		int flashCount;
		int shotScore;
		int shotScoreCount;
		int startTime;
		int currentTime;
		int roundTime;
		int extraPadding;

		Face face;

		// Events
		void onCameraShotEvent();
		void onFaceExpression();

		// Game
		void drawIntro();
		void drawInstr();
		void drawGame();
		void drawScoreboard();

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
