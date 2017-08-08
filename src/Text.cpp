//
//  Text.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "Text.h"

namespace ofxJsonUI
{
	Text::Text(Json::Value& config)
	{
		setup(config);
	}

	void Text::setup(Json::Value& config)
	{
		setName(ofxJsonParser::parseString(config["name"], "Text"));
		setPosition(ofxJsonParser::parseVector(config["pos"]));
		conf.text = ofxJsonParser::parseString(config["text"]);
		conf.fontName = ofxJsonParser::parseString(config["font"]);
		conf.fontSize = ofxJsonParser::parseFloat(config["size"]);
		conf.color = ofxJsonParser::parseColor(config["color"]);
		conf.maxLineWidth = ofxJsonParser::parseFloat(config["max-line-width"],-1);
		conf.lineHeight = ofxJsonParser::parseFloat(config["line-height"], ofxNanoVG::one().getFont(conf.fontName)->lineHeight);

		string halign = ofxJsonParser::parseString(config["h-align"]);
		conf.halign = (halign=="right")?ofxNanoVG::NVG_ALIGN_RIGHT:(halign=="center")?ofxNanoVG::NVG_ALIGN_CENTER:ofxNanoVG::NVG_ALIGN_LEFT;
		string valign = ofxJsonParser::parseString(config["v-align"]);
		conf.valign = (valign=="bottom")?ofxNanoVG::NVG_ALIGN_BOTTOM:(valign=="middle")?ofxNanoVG::NVG_ALIGN_MIDDLE:ofxNanoVG::NVG_ALIGN_TOP;

		conf.isHeb = ofxJsonParser::parseBool(config["heb"],false);
		if (conf.isHeb) {
			conf.text = toHebrewParagraph(conf.text, ofxJsonParser::parseInt(config["max-chars-per-line"], 20));
		}

		refreshTextRect();
		cache.bTextRectNeedsUpdate = false;
	}

	void Text::update(float dt)
	{
		if (cache.bTextRectNeedsUpdate) {
			refreshTextRect();
			cache.bTextRectNeedsUpdate = false;
		}

	}

	void Text::draw()
	{
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().setFillColor(conf.color);
		ofxNanoVG::one().setTextAlign(conf.halign, conf.valign);

		if (conf.maxLineWidth > 0) {
			ofxNanoVG::one().drawTextBox(conf.fontName, cache.xpos, cache.ypos-cache.textRect.y, conf.text, conf.fontSize, conf.maxLineWidth, conf.lineHeight);
		}
		else {
			ofxNanoVG::one().drawText(conf.fontName, 0, 0, conf.text, conf.fontSize);
		}
	}

	void Text::drawDebug()
	{
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().fillCircle(0, 0, 10, ofColor(255, 0, 0));

		ofColor color = isTouched()?ofColor(255, 255, 100):ofColor(255, 40, 100);
		if (conf.maxLineWidth > 0) {
			ofTranslate(cache.xpos, cache.ypos);
			ofxNanoVG::one().applyOFMatrix();
			ofxNanoVG::one().strokeRect(0, 0, getWidth(), getHeight(), color, 2);
			ofxNanoVG::one().strokeLine(conf.maxLineWidth, 0, conf.maxLineWidth, getHeight(), ofColor(50, 255, 70), 2);
		}
		else {
			ofTranslate(cache.textRect.x, cache.textRect.y);
			ofxNanoVG::one().applyOFMatrix();
			ofxNanoVG::one().strokeRect(0, 0, getWidth(), getHeight(), color, 2);
		}
	}

	bool Text::contains(const ofVec3f &p)
	{
		ofVec2f local = toLocal(p);
		if (conf.maxLineWidth > 0) {
			if (local.x < cache.xpos ||
				local.x > cache.xpos + getWidth() ||
				local.y < cache.ypos ||
				local.y > cache.ypos + getHeight()) {
				return false;
			}
			return true;
		}
		else {
			if (local.x < cache.textRect.x ||
				local.x > cache.textRect.x+getWidth() ||
				local.y < cache.textRect.y ||
				local.y > cache.textRect.y+getHeight()) {
				return false;
			}
			return true;
		}
	}

	void Text::refreshTextRect()
	{
		ofxNanoVG::one().resetMatrix();
		ofxNanoVG::one().setTextAlign(conf.halign, conf.valign);
		if (conf.maxLineWidth > 0) {
			cache.textRect = ofxNanoVG::one().getTextBoxBounds(conf.fontName, 0, 0, conf.text, conf.fontSize, conf.maxLineWidth, conf.lineHeight);
		}
		else {
			cache.textRect = ofxNanoVG::one().getTextBounds(conf.fontName, 0, 0, conf.text, conf.fontSize);
		}
		setSize(cache.textRect.width, cache.textRect.height);
		cache.xpos = conf.halign==ofxNanoVG::NVG_ALIGN_LEFT?0:conf.halign==ofxNanoVG::NVG_ALIGN_CENTER?-0.5*getWidth():-1*getWidth();
		cache.ypos = conf.valign==ofxNanoVG::NVG_ALIGN_TOP?0:conf.valign==ofxNanoVG::NVG_ALIGN_MIDDLE?-0.5*getHeight():-1*getHeight();

	}


	string Text::toHebrewParagraph(const string& str, int maxCharsPerLine)
	{
		if (maxCharsPerLine == -1) {
			deque<unsigned int> utf32line;
			// reverse
			utf8::utf8to32(str.begin(), str.end(), front_inserter(utf32line));
			// convert back to std::string (utf8)
			string utf8line;
			utf8::utf32to8(utf32line.begin(), utf32line.end(), back_inserter(utf8line));
			return utf8line;
		}

		deque<unsigned int> utf32line;
		// convert to utf32
		utf8::utf8to32(str.begin(), str.end(), back_inserter(utf32line));
		int chars = 0;
		vector<deque<unsigned int>> lines;
		deque<unsigned int> currentLine;
		for (auto& c: utf32line) {
			currentLine.push_front(c);
			chars++;
			if (c==' ') {
				if (chars>=maxCharsPerLine) {
					lines.push_back(currentLine);
					chars=0;
					currentLine.clear();
				}
			}
		}
		if (currentLine.size() > 0) {
			lines.push_back(currentLine);
		}

		string utf8line;
		for (auto& line: lines) {
			utf8::utf32to8(line.begin(), line.end(), back_inserter(utf8line));
			utf8line.push_back('\n');
		}
		
		
		return utf8line;
	}

}

