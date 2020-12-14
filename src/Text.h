//
//  Text.hpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef Text_h
#define Text_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class Text : public Node
	{
	public:
		Text();
		Text(Json::Value& styleJson);
		void setStyle(Json::Value& styleJson);
		void setHeb(bool set);
		void setText(const string& str);
		void setHAlign(const string& halign);
		string& getText() { return text; }
		void updateSize();
		virtual void update(float dt) override;
		virtual void draw() override;
		virtual void drawDebug() override;
		virtual bool contains(const ofVec3f& p) override;

	private:
		string text;
		struct style_t {
			float fontSize;
			ofColor color;
			string fontName;
			float lineHeight;
			float maxLineWidth;
			ofxNanoVG::TextHorizontalAlign halign;
			ofxNanoVG::TextVerticalAlign valign;
			bool isHeb;
			int maxCharsPerLine;
		} style;
		struct cache_t {
			bool bSizeNeedsUpdate;
			ofRectangle textRect;
			float xpos;
			float ypos;
		} cache;
		string toHebrewParagraph(const string& str, int maxCharsPerLine);
	};
}
#endif /* Text_h */
