#include "gameApp.h"

//--------------------------------------------------------------
void gameApp::setup(){
	// Set application properties
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofSetWindowTitle("Make a face!");
	debug = false;

	// Load image resources
	intro.loadImage("splash_screen.jpg");
	instr.loadImage("instrucciones.jpg");
	scoreboard.loadImage("scoreboard.jpg");
	background.loadImage("background.jpg");
	levelLayer.loadImage("level_layer.png");
	picturesIcon.loadImage("pictures_icon.png");
	picturesIcon.resize(picturesIcon.getWidth() * 0.70, picturesIcon.getHeight() * 0.70);
	picturesIcon.setAnchorPoint(picturesIcon.getWidth()/2, picturesIcon.getHeight()/2);
	clockIcon.loadImage("clock_icon.png");
	clockIcon.setAnchorPoint(clockIcon.getWidth()/2, clockIcon.getHeight()/2);
	wellDoneText.loadImage("well_done.png");
	flashLight.loadImage("flash_light.png");
	flashLight.setAnchorPoint(flashLight.getWidth()/2, flashLight.getHeight()/2);

	// Load font resources
	ofTrueTypeFont::setGlobalDpi(96);
	dosisFont24.loadFont("Dosis-ExtraLight.ttf", 24);
	dosisFont24.setLineHeight(18.0f);
	dosisFont24.setLetterSpacing(1.037);
	dosisFont40.loadFont("Dosis-ExtraLight.ttf", 40);
	dosisFont40.setLineHeight(18.0f);
	dosisFont40.setLetterSpacing(1.037);
	dosisFont24bold.loadFont("Dosis-Regular.ttf", 24);
	dosisFont24bold.setLineHeight(18.0f);
	dosisFont24bold.setLetterSpacing(1.037);
	dosisFont40bold.loadFont("Dosis-Regular.ttf", 40);
	dosisFont40bold.setLineHeight(18.0f);
	dosisFont40bold.setLetterSpacing(1.037);

	// Load color resources
	darkBlue.set(65, 75, 100);

	// Initialize camara and tracker
	cam.initGrabber(640, 480);
	tracker.setup();
	tracker.setRescale(1.2);
	classifier.load("expressions");
	count = 0;
	currentExpression = NEUTRAL;

	// Initialize variables
	score = 0;
	picturesTotal = 10;
	picturesLeft = picturesTotal;
	flashCount = 0;
	shotScoreCount = 0;
	roundTime = 60;
	extraPadding = 14;
	face.setup();

	gameState = INTRO;
}

//--------------------------------------------------------------
void gameApp::update(){
	if (gameState == PLAY) {
		face.update();
		currentTime = ofGetElapsedTimef() - startTime;
		if (currentTime >= roundTime) {
			gameState = SCORE;
			picturesIcon.resize(picturesIcon.getWidth() * 1.30, picturesIcon.getHeight() * 1.30);
		}
	}

	if (gameState != SCORE) {
		count++;
		cam.update();
		if (cam.isFrameNew() && ((count % 3) == 0)) {
			count = 0;
			if (tracker.update(ofxCv::toCv(cam))) {
				classifier.classify(tracker);

				expression newExpr;
			
				switch (classifier.getPrimaryExpression()) {
					default: case 0: newExpr = NEUTRAL; break;
					case 1: newExpr = HAPPY; break;
					case 2: newExpr = SURPRISED; break;
					case 3: newExpr = ANGRY; break;
				}

				if (newExpr != NEUTRAL && newExpr != currentExpression) {
					currentExpression = newExpr;
					onFaceExpression();
				}

				currentExpression = newExpr;
			}
		}
	}
}

//--------------------------------------------------------------
void gameApp::draw(){
	switch (gameState) {
		case INTRO: drawIntro(); break;
		case INSTR: drawInstr(); break;
		case PLAY: drawGame(); break;
		case SCORE: drawScoreboard(); break;
	}

	if (debug) {
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(classifier.getDescription(currentExpression), 20, ofGetWindowHeight() - 10);
		ofSetWindowTitle("Make a face! - " + ofToString(ofGetFrameRate(), 2) + " fps");
	}
}

//--------------------------------------------------------------
void gameApp::keyPressed(int key){
	if (key == OF_KEY_F12) {
		debug = !debug;

		if (!debug) {
			ofSetWindowTitle("Make a face!");
		}
	}

	if (debug && key == OF_KEY_RETURN) {
		switch (gameState) {
			case INTRO: {
				gameState = INSTR;
				break;
			}
			case INSTR: {
				gameState = PLAY;
				startTime = ofGetElapsedTimef();
				break;
			}
			case PLAY: {
				onCameraShotEvent();
				break;
			}
			case SCORE: {
				ofExit();
				break;
			}
		}
	}
}

void gameApp::onFaceExpression() {
	switch (gameState) {
		case PLAY: {
			onCameraShotEvent();
			break;
		}
		case INTRO: {
			if (currentExpression == HAPPY) {
				gameState = INSTR;
			}
			break;
		}
		case INSTR: {
			if (currentExpression == HAPPY) {
				gameState = PLAY;
				startTime = ofGetElapsedTimef();
			}
			break;
		}
	}
}

//--------------------------------------------------------------
void gameApp::keyReleased(int key){

}

//--------------------------------------------------------------
void gameApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void gameApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void gameApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void gameApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void gameApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void gameApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void gameApp::dragEvent(ofDragInfo dragInfo){ 

}


// -------------- INTRO ---------------------------------------
void gameApp::drawIntro() {
	intro.draw(0, 0);
}

// -------------- INSTRUCTIONS --------------------------------
void gameApp::drawInstr() {
	instr.draw(0, 0);
}

// ----------- GAME -------------------------------------------
void gameApp::drawGame() {
	ofSetColor(255, 255, 255);
	background.draw(0, 0);
	picturesIcon.draw(ofGetWindowWidth() - 80, 30);
	clockIcon.draw(ofGetWindowWidth() - 80, 83);

	ofSetColor(darkBlue);
	dosisFont24.drawString("Puntaje", 20, 45);
	dosisFont24bold.drawString(ofToString(score), 40 + extraPadding, 90);
	dosisFont24.drawString(ofToString(picturesLeft), ofGetWindowWidth() - 50, 45);
	dosisFont24.drawString(ofToString(roundTime - currentTime), ofGetWindowWidth() - 50, 95);

	face.draw();

	ofSetColor(255, 255, 255);
	levelLayer.draw(0, 0);

	if (flashCount > 0) {
		flashLight.draw(151, 568);
		flashCount--;
	}

	if (shotScoreCount > 0) {
		ofSetColor(51 + 2 * shotScoreCount, 255, 153 + 2 * shotScoreCount);
		dosisFont40.drawString("+" + ofToString(shotScore), 30 + shotScoreCount, 500 + shotScoreCount);
		shotScoreCount--;
	}
}

void gameApp::onCameraShotEvent() {
	flashCount = 10;
	shotScoreCount = 50;
	
	shotScore = face.getShotScore(currentExpression);
	score += shotScore;

	if (score < 10) {
		extraPadding = 14;
	} else if (score < 100) {
		extraPadding = 7;
	} else {
		extraPadding = 0;
	}

	picturesLeft--;
	if (picturesLeft <= 0) {
		picturesIcon.resize(picturesIcon.getWidth() * 1.30, picturesIcon.getHeight() * 1.30);
		gameState = SCORE;
	}
}

// ------ SCOREBOARD -----------------------------------------
void gameApp::drawScoreboard() {
	ofSetColor(255, 255, 255);
	scoreboard.draw(0, 0);
	
	picturesIcon.draw(360, 270);

	int picturesTaken = picturesTotal - picturesLeft;
	int timeBonus = 0.01 * score * (roundTime - currentTime);
	int totalScore = score + timeBonus;

	ofSetColor(darkBlue);
	dosisFont40.drawString(ofToString(picturesTaken), 410, 295);
	dosisFont40.drawString(ofToString(score), 410, 370);
	dosisFont40.drawString(ofToString(timeBonus), 410, 450);
	dosisFont40bold.drawString(ofToString(totalScore), 410, 555);

	if (totalScore > 100) {
		ofSetColor(255, 255, 255);
		wellDoneText.draw(400, 530);
	}
}