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
		Container(Json::Value& config);
		Node* getNodeById(const string& id);
		Container* getContainerById(const string& id);
	};
}
#endif /* Container_h */
