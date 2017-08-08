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
#include "Container.h"
#include "Text.h"

using namespace ofxInterface;

namespace ofxJsonUI
{
	static Node* create(Json::Value& config);
}

#endif /* ofxJsonUI_h */
