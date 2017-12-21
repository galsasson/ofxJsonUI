//
//  Shape.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "Shape.h"
#include "Global.h"

namespace ofxJsonUI
{
	Shape::Shape()
	{
		cache.tex = NULL;
		Json::Value styleJson = Json::objectValue;
		styleJson["tint"] = "#FFFFFF 100%";
		styleJson["size"] = Json::arrayValue;
		styleJson["size"][0] = 100;
		styleJson["size"][1] = 100;
		Shape((Json::Value&)styleJson);
	}

	Shape::Shape(Json::Value& styleJson)
	{
		setStyle(styleJson);
	}

	void Shape::setStyle(Json::Value& styleJson)
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
		setShape(ofxJsonParser::parseString(styleJson["file"]));
	}

	void Shape::setShape(const string& filename)
	{
		if (filename == "" ||
			!ofFile(filename).exists()) {
			cache.tex = NULL;
			return;
		}

		cache.svg = g->assets->getSvg(filename);
	}

	void Shape::update(float dt)
	{
	}

	void Shape::draw()
	{
		if (cache.tex != NULL) {
			ofSetColor(style.tint);
			cache.tex->draw(0, 0, style.size.x, style.size.y);
		}
	}

}
