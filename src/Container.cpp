//
//  Container.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 8/8/17.
//
//

#include "Container.h"

namespace ofxJsonUI
{
	Container::Container()
	{
		bHaveContent = false;
		pad = 128;
		nodeColor = ofColor(255);
		root = new Node();
		root->setVisible(false);
		addChild(root);
	}

	void Container::setContent(Json::Value& cont)
	{
		ofVec2f size = ofxJsonParser::parseVector(cont["size"]);
		allocateFbo(size);
		setSize(size);

		Json::Value& elements = cont["elements"];
		for (Json::Value& elem: elements) {
			Node* n = ofxJsonUI::create(elem);
			if (n!=NULL) {
				root->addChild(n);
				n->setPosition(ofxJsonParser::parseVector(elem["position"]));
			}
		}
	}

	void Container::allocateFbo(const ofVec2f& size)
	{
		ofFbo::Settings set;
		set.width = size.x+2*pad;
		set.height = size.y+2*pad;
		set.internalformat = GL_RGBA;
		fbo.allocate(set);
	}

	void Container::renderContents()
	{
		fbo.begin();
		ofClear(255,0);
		ofxNanoVG::one().beginFrame(fbo.getWidth(), fbo.getHeight(), 1);
		ofPushMatrix();
		ofTranslate(pad, pad);

		root->setVisible(true);
#ifdef GLM_SWIZZLE //this version of OF is using GLM for math ops
		ofMultMatrix(glm::inverse(root->getGlobalTransformMatrix()));
#else
		ofMultMatrix(root->getGlobalTransformMatrix().getInverse());
#endif
		root->render();
		root->setVisible(false);

		ofPopMatrix();
		ofxNanoVG::one().endFrame();
		fbo.end();
	}

	void Container::update(float dt)
	{
		AnimatableNode::update(dt);
		root->updateSubtreePostOrder(dt);
		renderContents();
	}

	void Container::draw()
	{
		ofSetColor(nodeColor);
		fbo.draw(-pad, -pad);
	}
}
