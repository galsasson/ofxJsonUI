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

		if (type == "container") {
			Container* c = new Container();
			c->setName(ofxJsonParser::parseString(config["name"],"container"));
			c->setContent(config);
			return c;
		}
		else if (type == "text") {
			Text* t = new Text();
			t->setName(ofxJsonParser::parseString(config["name"],"text"));
			t->setStyle(config);
			t->setText(ofxJsonParser::parseString(config["text"]));
			return t;
		}
		else if (type == "image") {
			Image* i = new Image();
			i->setName(ofxJsonParser::parseString(config["name"],"image"));
			i->setStyle(config);
			return i;
		}
		else if (type == "video") {
			Video* v = new Video();
			v->setName(ofxJsonParser::parseString(config["name"],"video"));
			v->setStyle(config);
			return v;
		}
		else if (type == "svg") {
			SVG* s = new SVG();
			s->setName(ofxJsonParser::parseString(config["name"],"svg"));
			s->setStyle(config);
			return s;
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
