//
//  AnimatableNode.h
//  maccabi-wall
//
//  Created by Gal Sasson on 7/24/17.
//
//

#ifndef AnimatableNode_h
#define AnimatableNode_h

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxAnimatable.h"

using namespace ofxInterface;

class AnimatableNode : public Node
{
public:
	class Curve
	{
	public:
		AnimCurve type;
		float params[4];
		Curve(AnimCurve t = EASE_IN_EASE_OUT,float p0=0, float p1=0, float p2=0, float p3=0) {
			type = t;
			params[0] = p0;
			params[1] = p1;
			params[2] = p2;
			params[3] = p3;
		}
		Curve(float p[4]) : Curve(CUBIC_BEZIER_PARAM, p[0], p[1], p[2], p[3]) {}
		float calcAt(float t) {
			return ofxAnimatable::calcCurveAt(t, type, params[0], params[1], params[2], params[3]);
		}
	};

    AnimatableNode();
    void animatePositionTo(const ofVec3f& p, float duration, Curve curve = Curve(EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
	void stopPositionAnimation();
    void animateColor(ofColor* c, const ofColor& target, float duration, Curve curve = Curve(EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
	void animateFloat(float* val, float target, float duration, Curve curve=Curve(EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
    virtual void update(float dt) override;
    
    ofColor nodeColor;
    ofColor nodeBGColor;
    
private:
    struct posanim_t {
        ofVec3f source;
        ofVec3f target;
        float duration;
        Curve curve;
        float delay;
        float progress;
        bool bActive;
		std::function<void(void)> onEnd;
    };
    
    struct coloranim_t {
        ofColor* color;
        ofColor source;
        ofColor target;
        float duration;
        Curve curve;
        float delay;
        float progress;
		std::function<void(void)> onEnd;
    };

    struct floatanim_t {
        float* value;
        float source;
        float target;
        float duration;
        Curve curve;
        float delay;
        float progress;
		std::function<void(void)> onEnd;
    };

    posanim_t nodePositionAnimation;
    vector<coloranim_t> nodeColorAnimations;
    vector<floatanim_t> nodeFloatAnimations;
};

#endif /* AnimatableNode_h */
