//
//  SVG.h
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef SVG_h
#define SVG_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class SVG : public Node
	{
	public:
		~SVG();
		SVG();
		SVG(Json::Value& styleJson);
		void setStyle(Json::Value& styleJson);
		void setSVG(const string& svg);
		virtual void update(float dt) override;
		virtual void draw() override;

	private:
		struct style_t {
			bool bStroke;
			ofColor strokeColor;
			bool bFill;
			ofColor fillColor;
			ofVec2f size;
			string units;
			float dpi;
		} style;
		struct cache_t {
			NSVGimage* svg;
		} cache;
	};
}
#endif /* SVG_h */
