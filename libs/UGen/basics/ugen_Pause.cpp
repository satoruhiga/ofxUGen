// $Id: ugen_Pause.cpp 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/basics/ugen_Pause.cpp $

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


// check gpl

#include "../core/ugen_StandardHeader.h"

BEGIN_UGEN_NAMESPACE

#include "ugen_Pause.h"
#include "ugen_InlineUnaryOps.h"



PauseUGenInternal::PauseUGenInternal(Pause_InputsWithTypesOnly) throw()
:	UGenInternal(NumInputs),
	prevLevel(0.f)
{
	inputs[Input] = input;
	inputs[Level] = level;
}

void PauseUGenInternal::initValue(const float value) throw()
{
	UGenInternal::initValue(value);
	prevLevel = value;
}

UGenInternal* PauseUGenInternal::getChannel(const int channel) throw()
{
	PauseUGenInternal* internal = new PauseUGenInternal(inputs[Input].getChannel(channel),
														inputs[Level].getChannel(channel));
	internal->prevLevel = prevLevel;
	return internal;
}

void PauseUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw()
{
	int numSamplesToProcess = uGenOutput.getBlockSize();
	float* outputSamples = uGenOutput.getSampleData();
	
	float currentLevel = *(inputs[Level].processBlock(shouldDelete, blockID, channel));
	
	if(currentLevel == 0.f && prevLevel == 0.f)
	{
#ifdef UGEN_VDSP
		vDSP_vclr(outputSamples, 1, numSamplesToProcess);
#else
		memset(outputSamples, 0, numSamplesToProcess * sizeof(float));
#endif
	}
	else
	{
		float* inputSamples = inputs[Input].processBlock(shouldDelete, blockID, channel);
		
		if(currentLevel != prevLevel)
		{
			float inc = (currentLevel - prevLevel) / (float)numSamplesToProcess;
			float level = prevLevel;
			while(numSamplesToProcess--)
			{
				*outputSamples++ = *inputSamples++ * level;
				level += inc;
			}
		}
		else
		{
#ifdef UGEN_VDSP
			float zero = 0.f;
			vDSP_vsmsa(inputSamples, 1, &currentLevel, &zero, outputSamples, 1, numSamplesToProcess);
#else
			while(numSamplesToProcess--)
			{
				*outputSamples++ = *inputSamples++ * currentLevel;
			}
#endif
		}
	}
	
	prevLevel = currentLevel;
}

Pause::Pause(Pause_InputsWithTypesOnly) throw()
{
	UGen inputs[] = { Pause_InputsNoTypes };
	const int numInputChannels = findMaxInputChannels(numElementsInArray(inputs), inputs);
	initInternal(numInputChannels);
	for(unsigned int i = 0; i < numInternalUGens; i++)
	{
		internalUGens[i] = new PauseUGenInternal(Pause_InputsNoTypes);
		internalUGens[i]->initValue(input.getValue(i) * level.getValue(i));
	}
}

END_UGEN_NAMESPACE
