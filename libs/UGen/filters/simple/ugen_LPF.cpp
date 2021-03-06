// $Id: ugen_LPF.cpp 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/filters/simple/ugen_LPF.cpp $

/*
 ==============================================================================
 
 This file is part of the UGEN++ library
 Copyright 2008-11 The University of the West of England.
 by Martin Robinson
 
 ------------------------------------------------------------------------------
 
 UGEN++ can be redistributed and/or modified under the terms of the
 GNU General Public License, as published by the Free Software Foundation;
 either version 2 of the License, or (at your option) any later version.
 
 UGEN++ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with UGEN++; if not, visit www.gnu.org/licenses or write to the
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA 02111-1307 USA
 
 The idea for this project and code in the UGen implementations is
 derived from SuperCollider which is also released under the 
 GNU General Public License:
 
 SuperCollider real time audio synthesis system
 Copyright (c) 2002 James McCartney. All rights reserved.
 http://www.audiosynth.com
 
 ==============================================================================
 */

#ifndef UGEN_NOEXTGPL

#include "../../core/ugen_StandardHeader.h"

BEGIN_UGEN_NAMESPACE

#include "ugen_LPF.h"
#include "../../core/ugen_Constants.h"
#include "../../basics/ugen_InlineUnaryOps.h"
#include "../../basics/ugen_InlineBinaryOps.h"


LPFUGenInternal::LPFUGenInternal(UGen const& input, UGen const& freq) throw()
:	UGenInternal(NumInputs),
	y1(0.f), y2(0.f), a0(0.f), b1(0.f), b2(0.f),
	currentFreq(0.f)
{
	inputs[Input] = input;
	inputs[Freq] = freq;
}

UGenInternal* LPFUGenInternal::getChannel(const int channel) throw()
{
	return new LPFUGenInternal(inputs[Input].getChannel(channel), 
							   inputs[Freq].getChannel(channel));
}

void LPFUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw()
{
	double piOverSampleRate = UGen::getReciprocalSampleRate() * pi;
	int numSamplesToProcess = uGenOutput.getBlockSize();
	float* outputSamples = uGenOutput.getSampleData();
	float* inputSamples = inputs[Input].processBlock(shouldDelete, blockID, channel);
	float* freqSamples = inputs[Freq].processBlock(shouldDelete, blockID, channel);
	float y0;
	float newFreq = *freqSamples;
		
	if(newFreq != currentFreq)
	{
		float slope = 1.f / numSamplesToProcess;
		float pfreq = max(0.01f, newFreq) * piOverSampleRate;
		
		float C = 1.f / tan(pfreq);
		float C2 = C * C;
		float sqrt2C = C * sqrt2;
		
		float next_a0 =   1.f / (1.f + sqrt2C + C2);
		float next_b1 =  -2.f * (1.f - C2) * next_a0 ;
		float next_b2 = -(1.f - sqrt2C + C2) * next_a0;
			
		float a0_slope = (next_a0 - a0) * slope;
		float b1_slope = (next_b1 - b1) * slope;
		float b2_slope = (next_b2 - b2) * slope;
		
		while(numSamplesToProcess--)
		{
			y0 = *inputSamples++ + b1 * y1 + b2 * y2; 
			*outputSamples++ = a0 * (y0 + 2.f * y1 + y2);
			y2 = y1; 
			y1 = y0;			
		
			a0 += a0_slope;
			b1 += b1_slope;
			b2 += b2_slope;
		}		
	}
	else
	{
		while(numSamplesToProcess--)
		{
			y0 = *inputSamples++ + b1 * y1 + b2 * y2; 
			*outputSamples++ = a0 * (y0 + 2.f * y1 + y2);
			y2 = y1; 
			y1 = y0;			
		}
	}
	
	y1 = zap(y1);
	y2 = zap(y2);
	
	currentFreq = newFreq;
}


LPF::LPF(UGen const& input, UGen const& freq) throw()
{
	int numChannels = 1;
	numChannels = input.getNumChannels() > numChannels ? input.getNumChannels()	: numChannels;
	numChannels = freq.getNumChannels()	 > numChannels ? freq.getNumChannels()	: numChannels;
	
	initInternal(numChannels);
	
	for(unsigned int i = 0; i < numInternalUGens; i++)
	{
		internalUGens[i] = new LPFUGenInternal(input, freq);
	}
}

END_UGEN_NAMESPACE

#endif // gpl