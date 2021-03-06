#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(239);
	ofSetWindowTitle("Insta");

	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	// Link pixels And pixels_mat
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);
	ofTranslate(this->fbo.getWidth() / 2, this->fbo.getHeight() / 2);

	ofFill();
	// Draw Small Object 
	ofSetColor(200, 39, 39);
	int radius = 150;
	ofBeginShape();
	for (int deg = 0; deg <= 360; deg += 1) {

		ofPoint point(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		float noise_value = ofMap(ofNoise(point.x * 0.005, point.y * 0.005, ofGetFrameNum() * 0.005), 0, 1, 0.5, 1);
		ofVertex(point * noise_value);
	}

	ofNextContour();
	radius *= 0.5;
	for (int deg = 0; deg <= 360; deg += 1) {

		ofPoint point(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		float noise_value = ofMap(ofNoise(point.x * 0.005, point.y * 0.005, ofGetFrameNum() * 0.005), 0, 1, 0.5, 1);
		ofVertex(point * noise_value);
	}
	ofEndShape(true);

	// Draw Big Object 
	ofSetColor(39, 39, 200);
	radius = 350;
	ofBeginShape();
	for (int deg = 0; deg <= 360; deg += 1) {

		ofPoint point(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		float noise_value = ofMap(ofNoise(point.x * 0.005, point.y * 0.005, ofGetFrameNum() * 0.001), 0, 1, 0.5, 1.0);

		ofVertex(point * noise_value);
	}

	ofNextContour();
	for (int deg = 0; deg <= 360; deg += 1) {

		ofPoint point(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		float noise_value = ofMap(ofNoise(point.x * 0.005, point.y * 0.005, ofGetFrameNum() * 0.001), 0, 1, 0.5, 1.0);

		ofVertex(point * noise_value * 0.95);
	}
	ofEndShape(true);

	this->fbo.end();
	// Copy Fbo To pixels
	this->fbo.readToPixels(this->pixels);

	// Gray
	cv::Mat gray_mat;
	cv::cvtColor(this->pixels_mat, gray_mat, cv::COLOR_RGBA2GRAY);
	for (int y = 0; y < this->pixels_mat.cols; y++) {

		for (int x = 0; x < this->pixels_mat.rows; x++) {

			if (gray_mat.at<unsigned char>(y, x) > 0) {

				this->pixels_mat.at<cv::Vec4b>(y, x) = cv::Vec4b(gray_mat.at<unsigned char>(y, x), gray_mat.at<unsigned char>(y, x), gray_mat.at<unsigned char>(y, x), this->pixels_mat.at<cv::Vec4b>(y, x)[3]);
			}
		}
	}

	// Blur Process
	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(19, 19), 10, 10);
}

//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Shadow
	ofSetColor(255, 192);
	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(-5, -5);

	// Draw Fbo Image 
	ofSetColor(255);
	this->fbo.draw(-25, -25);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}