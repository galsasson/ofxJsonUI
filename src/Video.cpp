//
//  Video.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "Video.h"
#include "Global.h"

namespace ofxJsonUI
{
	Video::~Video()
	{
		if (cache.player.isLoaded()) {
			cache.player.close();
		}
	}

	Video::Video()
	{
		style.tint = ofColor(255);
		style.size = ofVec2f(100,100);
		style.bAutoPlay = false;
	}

	Video::Video(Json::Value& styleJson)
	{
		setStyle(styleJson);
	}

	void Video::setStyle(Json::Value& styleJson)
	{
		string styleName = ofxJsonParser::parseString(styleJson["style-name"]);
		if (styleName != "") {
			ofxJsonParser::objectMerge(styleJson, ofxJsonUI::getStyle(styleName));
		}

		style.tint = ofxJsonParser::parseColor(styleJson["tint"], ofColor(255));
		style.size = ofxJsonParser::parseVector(styleJson["size"]);
		style.bAutoPlay = (ofxJsonParser::parseString(styleJson["play"])=="auto");
		setSize(style.size);
		setVideo(ofxJsonParser::parseString(styleJson["file"]));
	}

	void Video::setVideo(const string& filename)
	{
		if (ofFile(filename).exists()) {
			cache.player.load(filename);
			if (style.bAutoPlay) {
				cache.player.play();
			}
		}
		else {
			cache.player.close();
		}
	}

	void Video::update(float dt)
	{
		if (cache.player.isLoaded()) {
			cache.player.update();
		}
	}

	void Video::draw()
	{
		if (cache.player.isLoaded()) {
			ofSetColor(style.tint);
			cache.player.draw(0, 0, style.size.x, style.size.y);
		}
	}

}
