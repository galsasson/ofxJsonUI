//
//  Image.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "Image.h"
#include "Global.h"

namespace ofxJsonUI
{
	Image::Image()
	{
		cache.tex = NULL;
		Json::Value styleJson = Json::objectValue;
		styleJson["tint"] = "#FFFFFF 100%";
		styleJson["size"] = Json::arrayValue;
		styleJson["size"][0] = 100;
		styleJson["size"][1] = 100;
		Image((Json::Value&)styleJson);
	}

	Image::Image(Json::Value& styleJson)
	{
		setStyle(styleJson);
	}

	void Image::setStyle(Json::Value& styleJson)
	{
		string styleName = ofxJsonParser::parseString(styleJson["style-name"]);
		if (styleName != "") {
			ofxJsonParser::objectMerge(styleJson, ofxJsonUI::getStyle(styleName));
		}

		if (styleJson.isMember("tint")) {
			style.tint = ofxJsonParser::parseColor(styleJson["tint"], ofColor(255));
		}
		style.size = ofxJsonParser::parseVector(styleJson["size"]);
		setSize(style.size);
		setImage(ofxJsonParser::parseString(styleJson["file"]));
	}

	void Image::setImage(const string& filename)
	{
		if (filename == "" ||
			!ofFile(filename).exists()) {
			cache.tex = NULL;
			return;
		}

		cache.tex = g->assets->getTexture(filename);
	}

	void Image::update(float dt)
	{
	}

	void Image::draw()
	{
		if (cache.tex != NULL) {
			ofSetColor(style.tint);
			cache.tex->draw(0, 0, style.size.x, style.size.y);
		}
	}

}
