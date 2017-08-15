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
	Text::Text()
	{
		Json::Value styleJson = Json::objectValue;
		styleJson["color"] = "#FFFFFF 100%";
		styleJson["size"] = 80;
		styleJson["font"] = "regular";
		styleJson["h-align"] = "left";
		styleJson["v-align"] = "top";
		styleJson["max-line-width"] = 800;
		styleJson["heb"] = false;
		styleJson["max-chars-per-line"] = 15;
		Text((Json::Value&)styleJson);
	}

	Text::Text(Json::Value& styleJson)
	{
		text = "- empty -";
		setStyle(styleJson);
	}

	void Text::setStyle(Json::Value& styleJson)
	{
		string styleName = ofxJsonParser::parseString(styleJson["style-name"]);
		if (styleName != "") {
			ofxJsonParser::objectMerge(styleJson, ofxJsonUI::getStyle(styleName));
		}

		style.fontName = ofxJsonParser::parseString(styleJson["font"]);
		if (ofxNanoVG::one().getFont(style.fontName) == NULL) {
			ofLogError("ofxJsonUI::Text") << "no such font: '"<<style.fontName<<"'";
			return;
		}
		style.fontSize = ofxJsonParser::parseFloat(styleJson["size"]);
		style.color = ofxJsonParser::parseColor(styleJson["color"], ofColor(255));
		style.maxLineWidth = ofxJsonParser::parseFloat(styleJson["max-line-width"],-1);
		style.lineHeight = ofxJsonParser::parseFloat(styleJson["line-height"], ofxNanoVG::one().getFont(style.fontName)->lineHeight);
		string halign = ofxJsonParser::parseString(styleJson["h-align"]);
		style.halign = (halign=="right")?ofxNanoVG::NVG_ALIGN_RIGHT:(halign=="center")?ofxNanoVG::NVG_ALIGN_CENTER:ofxNanoVG::NVG_ALIGN_LEFT;
		string valign = ofxJsonParser::parseString(styleJson["v-align"]);
		style.valign = (valign=="bottom")?ofxNanoVG::NVG_ALIGN_BOTTOM:(valign=="middle")?ofxNanoVG::NVG_ALIGN_MIDDLE:ofxNanoVG::NVG_ALIGN_TOP;
		style.isHeb = ofxJsonParser::parseBool(styleJson["heb"],false);
		style.maxCharsPerLine = ofxJsonParser::parseInt(styleJson["max-chars-per-line"], 20);

		cache.bSizeNeedsUpdate = true;
	}

	void Text::setText(const string& str)
	{
		if (style.isHeb) {
			text = toHebrewParagraph(str, style.maxCharsPerLine);
		}
		else {
			text = str;
		}

		cache.bSizeNeedsUpdate = true;
	}

	void Text::update(float dt)
	{
		if (cache.bSizeNeedsUpdate) {
			updateSize();
			cache.bSizeNeedsUpdate = false;
		}
	}

	void Text::draw()
	{
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().setFillColor(style.color);
		ofxNanoVG::one().setTextAlign(style.halign, style.valign);

		if (style.maxLineWidth > 0) {
			ofxNanoVG::one().drawTextBox(style.fontName, cache.xpos, cache.ypos-cache.textRect.y, text, style.fontSize, style.maxLineWidth, style.lineHeight);
		}
		else {
			ofxNanoVG::one().drawText(style.fontName, 0, 0, text, style.fontSize);
		}
	}

	void Text::drawDebug()
	{
		ofxNanoVG::one().applyOFMatrix();
		ofxNanoVG::one().fillCircle(0, 0, 10, ofColor(255, 0, 0));

		ofColor color = isTouched()?ofColor(255, 255, 100):ofColor(255, 40, 100);
		if (style.maxLineWidth > 0) {
			ofTranslate(cache.xpos, cache.ypos);
			ofxNanoVG::one().applyOFMatrix();
			ofxNanoVG::one().strokeRect(0, 0, getWidth(), getHeight(), color, 2);
			ofxNanoVG::one().strokeLine(style.maxLineWidth, 0, style.maxLineWidth, getHeight(), ofColor(50, 255, 70), 2);
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
		if (style.maxLineWidth > 0) {
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

	void Text::updateSize()
	{
		ofxNanoVG::one().resetMatrix();
		ofxNanoVG::one().setTextAlign(style.halign, style.valign);
		if (style.maxLineWidth > 0) {
			cache.textRect = ofxNanoVG::one().getTextBoxBounds(style.fontName, 0, 0, text, style.fontSize, style.maxLineWidth, style.lineHeight);
		}
		else {
			cache.textRect = ofxNanoVG::one().getTextBounds(style.fontName, 0, 0, text, style.fontSize);
		}
		setSize(cache.textRect.width, cache.textRect.height);

		cache.xpos = style.halign==ofxNanoVG::NVG_ALIGN_LEFT?0:style.halign==ofxNanoVG::NVG_ALIGN_CENTER?-0.5*getWidth():-1*getWidth();
		cache.ypos = style.valign==ofxNanoVG::NVG_ALIGN_TOP?0:style.valign==ofxNanoVG::NVG_ALIGN_MIDDLE?-0.5*getHeight():-1*getHeight();

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
			chars++;
			if (c == '(') {
				currentLine.push_front(')');
			}
			else if (c == ')') {
				currentLine.push_front('(');
			}
			else if (c == '\n') {
				lines.push_back(currentLine);
				chars=0;
				currentLine.clear();
			}
			else {
				currentLine.push_front(c);
			}
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

