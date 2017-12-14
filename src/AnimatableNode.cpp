//
//  AnimatableNode.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 7/24/17.
//
//

#include "AnimatableNode.h"

AnimatableNode::AnimatableNode()
{
    nodePositionAnimation.bActive = false;
}

void AnimatableNode::animatePositionTo(const ofVec3f& pos, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
    nodePositionAnimation.source = getPosition();
    nodePositionAnimation.target = pos;
    nodePositionAnimation.duration = duration;
    nodePositionAnimation.curve = curve;
    nodePositionAnimation.delay = delay;
    nodePositionAnimation.progress = 0;
    nodePositionAnimation.bActive = true;
	nodePositionAnimation.onEnd = onEndFunc;
}

void AnimatableNode::stopPositionAnimation()
{
	nodePositionAnimation.bActive = false;
}

void AnimatableNode::animateColor(ofColor *c, const ofColor& target, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
    struct coloranim_t anim;
    anim.color = c;
    anim.source = *c;
    anim.target = target;
    anim.duration = duration;
    anim.curve = curve;
    anim.delay = delay;
    anim.progress = 0;
	anim.onEnd = onEndFunc;
	// remove old animations that reference the same value
	for (int i=nodeColorAnimations.size()-1; i>=0; i--) {
		if (nodeColorAnimations[i].color == c) {
			nodeColorAnimations.erase(nodeColorAnimations.begin()+i);
		}
	}
	// add this one
    nodeColorAnimations.push_back(anim);
}

void AnimatableNode::animateFloat(float* val, float target, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
    struct floatanim_t anim;
    anim.value = val;
    anim.source = *val;
    anim.target = target;
    anim.duration = duration;
    anim.curve = curve;
    anim.delay = delay;
    anim.progress = 0;
	anim.onEnd = onEndFunc;
	// remove old animations that reference the same value
	for (int i=nodeFloatAnimations.size()-1; i>=0; i--) {
		if (nodeFloatAnimations[i].value == val) {
			nodeFloatAnimations.erase(nodeFloatAnimations.begin()+i);
		}
	}
	// add this one
    nodeFloatAnimations.push_back(anim);
}

void AnimatableNode::update(float dt)
{
    // Update position animation
    if (nodePositionAnimation.bActive) {
        nodePositionAnimation.progress += dt;
        float timeProg = ofClamp(nodePositionAnimation.progress-nodePositionAnimation.delay, 0, nodePositionAnimation.duration) / nodePositionAnimation.duration;
		float prog = ofClamp(nodePositionAnimation.curve.calcAt(timeProg), 0, 1);
        setPosition(nodePositionAnimation.source.getInterpolated(nodePositionAnimation.target, prog));
        if (nodePositionAnimation.progress >= nodePositionAnimation.delay+nodePositionAnimation.duration) {
			nodePositionAnimation.bActive = false;
			nodePositionAnimation.onEnd();
        }
    }
    
    // Update color animation
    for (int i=nodeColorAnimations.size()-1; i>=0; i--) {
        coloranim_t& anim = nodeColorAnimations[i];
        anim.progress += dt;
        float timeProg = ofClamp(anim.progress-anim.delay, 0, anim.duration)/anim.duration;
		float prog = ofClamp(anim.curve.calcAt(timeProg), 0, 1);
        *anim.color = anim.source.getLerped(anim.target, prog);
        if (anim.progress >= anim.delay+anim.duration) {
			anim.onEnd();
            nodeColorAnimations.erase(nodeColorAnimations.begin()+i);
        }
    }
    
    // Update float animation
    for (int i=nodeFloatAnimations.size()-1; i>=0; i--) {
        floatanim_t& anim = nodeFloatAnimations[i];
        anim.progress += dt;
        float timeProg = ofClamp(anim.progress-anim.delay, 0, anim.duration)/anim.duration;
		float prog = ofClamp(anim.curve.calcAt(timeProg), 0, 1);
        *anim.value = ofMap(prog, 0, 1, anim.source, anim.target, true);
        if (anim.progress >= anim.delay+anim.duration) {
			anim.onEnd();
            nodeFloatAnimations.erase(nodeFloatAnimations.begin()+i);
        }
    }
}
