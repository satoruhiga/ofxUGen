#pragma once

#include "ofMain.h"

#include "UGen.h"

#include "ofxUGenUtils.h"

#define OFXUGEN_SCOPED_LOCK ofxUGen::ScopedLock __lock__(Server::get().mutex)

namespace ofxUGen
{
	class Server;
	class SynthDef;
	
	typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
}

class ofxUGen::Server : public ofBaseSoundInput, public ofBaseSoundOutput
{
	friend class SynthDef;
	
	class BufferBlock;
	static Server *_instance;
	
	Server();
	~Server();
	
	Server(const Server &);
	Server& operator=(const Server&);
	
public:
	
	static Server& get();
	inline static Server& instance() { return get(); }
	
	class SynthDef;
	friend class SynthDef;
	
	void setup(int num_output = 2, int num_input = 0, float sample_rate = 44100,  int buffer_size = 512);
	void close();
	
	void audioIn(float *input, int bufferSize, int nChannels);
	void audioOut(float *output, int bufferSize, int nChannels);
	
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }

	void play(UGen &ugen)
	{
		OFXUGEN_SCOPED_LOCK;
		array.add(ugen);
	}
	
	void stop(UGen &ugen)
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
		Server::get();
	}
	
	virtual ~SynthDef()
	{
		stop();
	}
	
	void play()
	{
		Server::get().play(out);
	}
	
	void stop()
	{
		Server::get().stop(out);
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

namespace ofxUGen
{
	inline Server& s() { return Server::get(); }
}