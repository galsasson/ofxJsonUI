//
//  SVG.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "SVG.h"
#include "Global.h"

namespace ofxJsonUI
{
	SVG::~SVG()
	{
		if (cache.svg != NULL) {
			ofxNanoVG::one().freeSvg(cache.svg);
		}
	}
	
	SVG::SVG()
	{
		cache.svg = NULL;
		Json::Value styleJson = Json::objectValue;
		styleJson["stroke"] = true;
		styleJson["strokeColor"] = "#FFFFFF 100%";
		styleJson["fill"] = false;
		styleJson["fillColor"] = "#000000 100%";
		styleJson["units"] = "px";
		styleJson["dpi"] = 96;
		styleJson["size"] = Json::arrayValue;
		styleJson["size"][0] = 100;
		styleJson["size"][1] = 100;
		SVG((Json::Value&)styleJson);
	}

	SVG::SVG(Json::Value& styleJson)
	{
		setStyle(styleJson);
	}

	void SVG::setStyle(Json::Value& styleJson)
	{
		string styleName = ofxJsonParser::parseString(styleJson["style-name"]);
		if (styleName != "") {
			ofxJsonParser::objectMerge(styleJson, ofxJsonUI::getStyle(styleName));
		}

		style.bStroke = ofxJsonParser::parseBool(styleJson["stroke"]);
		style.strokeColor = ofxJsonParser::parseColor(styleJson["strokeColor"], ofColor(255));
		style.bFill = ofxJsonParser::parseBool(styleJson["fill"]);
		style.fillColor = ofxJsonParser::parseColor(styleJson["fillColor"], ofColor(255));
		style.size = ofxJsonParser::parseVector(styleJson["size"], ofVec2f(0,0));
		setSize(style.size);
		style.units = ofxJsonParser::parseString(styleJson["units"]);
		style.dpi = ofxJsonParser::parseFloat(styleJson["dpi"]);
		
		if (styleJson.isMember("file")) {
			string filename = ofxJsonParser::parseString(styleJson["file"]);
			if (ofFile(filename).exists()) {
				string data = ofBufferFromFile(filename).getText();
				setSVG(data);
			}
			else {
				ofLogError("ofxJsonUI::SVG") << "no such file: "<<filename;
			}
		}
		else if (styleJson.isMember("svg")) {
			string data = ofxJsonParser::parseString(styleJson["svg"]);
			setSVG(data);
		}
	}

	void SVG::setSVG(const string& svg)
	{
		cache.svg = ofxNanoVG::one().parseSvg(svg, style.units, style.dpi);
	}

	void SVG::update(float dt)
	{
	}

	void SVG::draw()
	{
		if (cache.svg == NULL) {
			return;
		}
		
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().beginPath();
		ofxNanoVG::one().followSvg(cache.svg);
		if (style.bFill) {
			ofxNanoVG::one().fillPath(style.fillColor);
		}
		if (style.bStroke) {
			ofxNanoVG::one().strokePath(style.strokeColor);
		}
	}

}
