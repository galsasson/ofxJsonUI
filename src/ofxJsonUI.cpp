//
//  ofxJsonUI.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "ofxJsonUI.h"

namespace ofxJsonUI
{
	Node* create(Json::Value& config)
	{
		string type = ofxJsonParser::parseString(config["type"], "unknown");
		if (type == "unknown") {
			return NULL;
		}

		if (type == "text") {
			Text* t = new Text();
			t->setStyle(config);
			t->setText(ofxJsonParser::parseString(config["text"]));
			return t;
		}
		else if (type == "container") {
			Container* c = new Container();
			c->setContent(config);
			return c;
		}
		else {
			return NULL;
		}
	}

	Json::Value getStyle(const string& name)
	{
		size_t sep = name.find(':');
		if (sep == string::npos) {
			return Json::objectValue;
		}
		string filename = name.substr(0, sep);
		string style = name.substr(sep+1);
		if (!ofFile(filename).exists()) {
			ofLogError("ofxJsonUI") << "no such styles file: '"<<filename<<"'";
			return Json::objectValue;
		}
		ofxJSONElement file;
		file.openLocal(filename);
		if (!file.isMember(style)) {
			ofLogError("ofxJsonUI") << "no such style: '"<<style<<"' in "<<filename;
			return Json::objectValue;
		}
		return file[style];
	}
}
