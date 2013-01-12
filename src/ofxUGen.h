#pragma once

#include "ofMain.h"

#include "UGen.h"

#define OFXUGEN_SCOPED_LOCK ofxUGen::ScopedLock __lock__(ofxUGen::instance().mutex)

class ofxUGen : public ofBaseSoundInput, public ofBaseSoundOutput
{
	class BufferBlock;
	
	static ofxUGen *_instance;
	
	ofxUGen();
	~ofxUGen();
	
	ofxUGen(const ofxUGen &);
	ofxUGen& operator=(const ofxUGen&);
	
public:
	
	static ofxUGen& instance();
	
	class SynthDef;
	friend class SynthDef;
	
	typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
	
	void setup(int num_output = 2, int num_input = 0, float sample_rate = 44100,  int buffer_size = 512);
	void close();
	
	void audioIn(float *input, int bufferSize, int nChannels);
	void audioOut(float *output, int bufferSize, int nChannels);
	
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }

	void registerSyhth(UGen &ugen)
	{
		OFXUGEN_SCOPED_LOCK;
		array.add(ugen);
	}
	
	void unregisterSyhth(UGen &ugen)
	{
		OFXUGEN_SCOPED_LOCK;
		array.removeItem(ugen);
	}

protected:
	
	ofSoundStream stream;
	BufferBlock *output_buffer;
	
	UGenArray array;
	Mix out;
	
	ofMutex mutex;
};

class ofxUGen::SynthDef : public DoneActionReceiver
{
public:
	
	SynthDef()
	{
		ofxUGen::instance();
	}
	
	virtual ~SynthDef()
	{
		stop();
	}
	
	void play()
	{
		ofxUGen::instance().registerSyhth(out);
	}
	
	void stop()
	{
		ofxUGen::instance().unregisterSyhth(out);
	}
	
	void release()
	{
		OFXUGEN_SCOPED_LOCK;
		out.release();
	}
	
	void handleDone(const int senderUserData)
	{
		out = UGen::getNull();
	}
	
	bool isAlive()
	{
		return !Out().isNull();
	}

protected:
	
	void Out(const UGen &ugen)
	{
		OFXUGEN_SCOPED_LOCK;
		out = ugen;
	}
	
	const UGen& Out() { return out; }
	
private:
	
	UGen out;
};
