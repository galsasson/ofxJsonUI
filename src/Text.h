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
		Text(Json::Value& config);
		void setup(Json::Value& config);
		virtual void update(float dt) override;
		virtual void draw() override;
		virtual void drawDebug() override;
		virtual bool contains(const ofVec3f& p) override;

	private:
		struct config_t {
			string text;
			float fontSize;
			ofColor color;
			string fontName;
			float lineHeight;
			float maxLineWidth;
			ofxNanoVG::TextHorizontalAlign halign;
			ofxNanoVG::TextVerticalAlign valign;
			bool isHeb;
		} conf;
		struct cache_t {
			bool bTextRectNeedsUpdate;
			ofRectangle textRect;
			float xpos;
			float ypos;
		} cache;
		void refreshTextRect();
		string toHebrewParagraph(const string& str, int maxCharsPerLine);
	};
}
#endif /* Text_h */
