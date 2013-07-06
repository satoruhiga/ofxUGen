#include "ofxUGen.h"

using namespace ofxUGen;

class Server::BufferBlock
{
public:
	
	BufferBlock(size_t buffer_size, size_t num_channels) : buffer_size(buffer_size), num_channels(num_channels)
	{
		interleaved = new float[buffer_size * num_channels];
		memset(interleaved, 0, sizeof(float) * buffer_size * num_channels);
		
		separated = new float* [num_channels];
		for (int i = 0; i < num_channels; i++)
		{
			separated[i] = new float [buffer_size];
			memset(separated[i], 0, sizeof(float) * buffer_size);
		}
	}
	
	~BufferBlock()
	{
		delete [] interleaved;
		interleaved = NULL;
		
		for (int i = 0; i < num_channels; i++)
			delete [] separated[i];
		
		delete [] separated;
		separated = NULL;
	}
	
	size_t getBufferSize() { return buffer_size; }
	size_t getNumChannels() { return num_channels; }
	
	float* getInterleavedBuffer() { return interleaved; }
	float** getSeparatedBuffer() { return separated; }
	
	void updateInterlevaedBuffer()
	{
		float *dst = interleaved;
		for (int i = 0; i < buffer_size; i++)
		{
			for (int c = 0; c < num_channels; c++)
			{
				*dst++ = separated[c][i];
			}
		}
	}
	
	void updateSeparatedBuffer()
	{
		float *src = interleaved;
		for (int i = 0; i < buffer_size; i++)
		{
			for (int c = 0; c < num_channels; c++)
			{
				separated[c][i] = *src++;
			}
		}
	}
	
private:
	
	float *interleaved;
	float **separated;
	
	size_t buffer_size;
	size_t num_channels;
};


#pragma mark - Server

Server* Server::_instance = NULL;

Server& Server::get()
{
	if (_instance == NULL)
		_instance = new Server;
	return *_instance;
}

Server::Server()
{
	UGen::initialise();
}

Server::~Server()
{
	close();
	
	UGen::shutdown();
}

void Server::audioIn(float *input, int bufferSize, int nChannels)
{
}

void Server::audioOut(float *output, int bufferSize, int nChannels)
{
	if (mutex.tryLock())
	{
		if (!out.isNull())
		{
			int blockID = UGen::getNextBlockID(bufferSize);
			
			out.setOutputs(output_buffer->getSeparatedBuffer(), bufferSize, nChannels);
			out.prepareAndProcessBlock(bufferSize, blockID, -1);
		}

		mutex.unlock();
	}
	
	output_buffer->updateInterlevaedBuffer();
	
	memcpy(output, output_buffer->getInterleavedBuffer(), sizeof(float) * bufferSize * nChannels);
}

void Server::setup(int num_output, int num_input, float sample_rate, int buffer_size)
{
	if (num_input)
	{
		num_input = 0;
		ofLogNotice("ofxUGen") << "audio input not yet implemented";
		// stream.setInput(this);
	}
	
	if (num_output)
	{
		stream.setOutput(this);
		output_buffer = new BufferBlock(buffer_size, num_output);
		out = Pan2::AR(Lag::AR(0), Lag::AR(0));
	}
	
	array = UGenArray(UGen::emptyChannels(num_output));
	out = Mix(array, false);
	array.clear();

	stream.setup(num_output, num_input, sample_rate, buffer_size, 4);
}

void Server::close()
{
	stream.close();
}
