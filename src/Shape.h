//
//  Shape.h
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef Shape_h
#define Shape_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class Shape : public Node
	{
	public:
		Shape();
		Shape(Json::Value& styleJson);
		void setStyle(Json::Value& styleJson);
		void setShape(const string& str);
		virtual void update(float dt) override;
		virtual void draw() override;

	private:
		struct style_t {
			ofColor color;
			ofColor tint;
			ofVec2f size;
		} style;
		struct cache_t {
			ofTexture* tex;
			NSVGimage* svg;
		} cache;
	};
}
#endif /* Shape_h */
