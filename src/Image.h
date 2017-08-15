//
//  Image.hpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef Image_h
#define Image_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class Image : public Node
	{
	public:
		Image();
		Image(Json::Value& styleJson);
		void setStyle(Json::Value& styleJson);
		void setImage(const string& str);
		virtual void update(float dt) override;
		virtual void draw() override;

	private:
		struct style_t {
			ofColor tint;
			ofVec2f size;
		} style;
		struct cache_t {
			ofTexture* tex;
		} cache;
	};
}
#endif /* Image_h */
