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
		styleJson = Json::objectValue;
		styleJson["stroke"] = true;
		styleJson["stroke-color"] = "#FFFFFF 100%";
		styleJson["stroke-width"] = 1;
		styleJson["fill"] = false;
		styleJson["fill-color"] = "#000000 100%";
		styleJson["units"] = "px";
		styleJson["dpi"] = 96;
		styleJson["scale"] = Json::arrayValue;
		styleJson["scale"][0] = 1;
		styleJson["scale"][1] = 1;
		styleJson["anchor"] = Json::arrayValue;
		styleJson["anchor"][0] = 0.5;
		styleJson["anchor"][1] = 0.5;
		styleJson["bezier"] = false;
		setStyle(styleJson);
	}

	SVG::SVG(Json::Value& styleJson)
	{
		setStyle(styleJson);
	}

	void SVG::setStyle(Json::Value& newStyle)
	{
		ofxJsonParser::objectMerge(styleJson, newStyle);
		
		string styleName = ofxJsonParser::parseString(styleJson["style-name"]);
		if (styleName != "") {
			ofxJsonParser::objectMerge(styleJson, ofxJsonUI::getStyle(styleName));
		}

		style.bStroke = ofxJsonParser::parseBool(styleJson["stroke"]);
		style.strokeColor = ofxJsonParser::parseColor(styleJson["stroke-color"], ofColor(255));
		style.strokeWidth = ofxJsonParser::parseFloat(styleJson["stroke-width"], 1);
		style.bFill = ofxJsonParser::parseBool(styleJson["fill"]);
		style.fillColor = ofxJsonParser::parseColor(styleJson["fill-color"], ofColor(255));
		style.scale = ofxJsonParser::parseVector(styleJson["scale"], ofVec2f(1,1));
		style.anchor = ofxJsonParser::parseVector(styleJson["anchor"], ofVec2f(0.5,0.5));
		style.units = ofxJsonParser::parseString(styleJson["units"]);
		style.dpi = ofxJsonParser::parseFloat(styleJson["dpi"]);
		style.bBezier = ofxJsonParser::parseBool(styleJson["bezier"]);
		
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
		
		ofPushMatrix();
		ofScale(style.scale.x, style.scale.y);
		ofTranslate(-style.anchor.x*cache.svg->width, -style.anchor.y*cache.svg->height);
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().beginPath();
		ofxNanoVG::one().followSvg(cache.svg, 0, 0, (style.bBezier?ofxNanoVG::SVG_BEZIER:ofxNanoVG::SVG_LINEAR));
		if (style.bFill) {
			ofxNanoVG::one().fillPath(style.fillColor);
		}
		if (style.bStroke) {
			ofxNanoVG::one().setStrokeWidth(style.strokeWidth);
			ofxNanoVG::one().strokePath(style.strokeColor);
		}
		ofPopMatrix();
	}

}
