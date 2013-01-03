#include "testApp.h"

#include "ofxUGen.h"

class MySynth : public ofxUGen::SynthDef
{
public:
	
	ofVec2f pos;
	
	UGen envgen;
	UGen amp;

	MySynth()
	{
		pos.x = ofGetMouseX();
		pos.y = ofGetMouseY();
		
		float freq = ofMap(pos.x, 0, ofGetHeight(), 0, 2000);
		float pan = ofMap(pos.y, 0, ofGetWidth(), -1, 1);

		Env env = Env::perc(0.5, 1.5, 0.3, EnvCurve::Sine);
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
		amp = SinOsc::AR(ofRandom(4.0), 0, 0.5, 0.5) * envgen;
		
		Out(
			Pan2::AR(SinOsc::AR(freq) * amp, pan)
		);
	}
	
	void draw()
	{
		ofFill();
		ofSetColor(255, amp.getValue() * 255);
		ofCircle(pos.x, pos.y, amp.getValue() * 100);
		
		ofNoFill();
		ofSetColor(255);
		ofCircle(pos.x, pos.y, amp.getValue() * 80);
	}
	
	bool isAlive()
	{
		return !Out().isNull();
	}
};

vector<MySynth*> synths;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	
	ofxUGen::instance().setup();
}

//--------------------------------------------------------------
void testApp::update()
{
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofEnableAlphaBlending();
	
	vector<MySynth*>::iterator it = synths.begin();
	while (it != synths.end())
	{
		MySynth *s = *it;
		s->draw();
		
		if (!s->isAlive())
		{
			it = synths.erase(it);
			delete s;
		}
		else
			it++;
	}
	
	ofDrawBitmapString("num synth: " + ofToString(synths.size(), 0), 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	for (int i = 0; i < synths.size(); i++)
		synths[i]->release();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	synths.push_back(new MySynth);
	synths.back()->play();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
