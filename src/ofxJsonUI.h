//
//  ofxJsonUI.h
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#ifndef ofxJsonUI_h
#define ofxJsonUI_h

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxNanoVG.h"
#include "ofxJSON.h"
#include "ofxJsonParser.h"

// Components
#include "AnimatableNode.h"
#include "Container.h"
#include "Text.h"
#include "Image.h"
#include "Video.h"
#include "SVG.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	Node* create(Json::Value& config);
	Json::Value getStyle(const string& name);
}

#endif /* ofxJsonUI_h */
