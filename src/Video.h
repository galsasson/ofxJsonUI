//
//  Video.hpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef Video_h
#define Video_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class Video : public Node
	{
	public:
		virtual ~Video() override;
		Video();
		Video(Json::Value& styleJson);
		void setStyle(Json::Value& styleJson);
		void setVideo(const string& str);
		ofVideoPlayer getPlayer() { return cache.player; }
		virtual void update(float dt) override;
		virtual void draw() override;

	private:
		struct style_t {
			ofColor tint;
			ofVec2f size;
			bool bAutoPlay;
		} style;
		struct cache_t {
			ofVideoPlayer player;
		} cache;
	};
}
#endif /* Video_h */
