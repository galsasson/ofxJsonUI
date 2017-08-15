//
//  Container.h
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef Container_h
#define Container_h

#include "ofxJsonUI.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	class Container : public Node
	{
	public:
		Container();
		void setContent(Json::Value& cont);
		Node* getNodeById(const string& id);
		Container* getContainerById(const string& id);

	private:
		Node* root;
		bool bHaveContent;
		ofFbo fbo;
		float pad;

		void allocateFbo(const ofVec2f& size);
		void renderContents();
		void update(float dt);
		void draw();
	};
}
#endif /* Container_h */
