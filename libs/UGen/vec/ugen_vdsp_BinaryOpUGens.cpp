// $Id: ugen_vdsp_BinaryOpUGens.cpp 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/vec/ugen_vdsp_BinaryOpUGens.cpp $

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

#include "../core/ugen_StandardHeader.h"

#ifdef UGEN_VDSP

BEGIN_UGEN_NAMESPACE

#include "../basics/ugen_BinaryOpUGens.h"


void BinaryAddUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw() 
{ 
	const int numSamplesToProcess = uGenOutput.getBlockSize(); 
	float* const outputSamples = uGenOutput.getSampleData(); 
	const float* const leftOperandSamples = inputs[LeftOperand].processBlock(shouldDelete, blockID, channel); 
	const float* const rightOperandSamples = inputs[RightOperand].processBlock(shouldDelete, blockID, channel); 
	vDSP_vadd(leftOperandSamples, 1, rightOperandSamples, 1, outputSamples, 1, numSamplesToProcess);
}

 // apple bug
static void Workaround_vsub(const float *a, int aStride, const float *b, int bStride, float *c, int cStride, int size )
{
    const float minusOne = -1.0f;
    vDSP_vsmul(a, aStride, &minusOne, c, cStride, size);
    vDSP_vadd( c, cStride, b, bStride, c, cStride, size );	
}

void BinarySubtractUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw() 
{ 
	const int numSamplesToProcess = uGenOutput.getBlockSize(); 
	float*  const  outputSamples = uGenOutput.getSampleData(); 
	const float* const leftOperandSamples = inputs[LeftOperand].processBlock(shouldDelete, blockID, channel); 
	const float* const rightOperandSamples = inputs[RightOperand].processBlock(shouldDelete, blockID, channel); 
	//vDSP_vsub(leftOperandSamples, 1, rightOperandSamples, 1, outputSamples, 1, numSamplesToProcess);
	Workaround_vsub(rightOperandSamples, 1, leftOperandSamples, 1, outputSamples, 1, numSamplesToProcess);
	//vDSP_vsub(rightOperandSamples, 1, leftOperandSamples, 1, outputSamples, 1, numSamplesToProcess);
}

void BinaryMultiplyUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw() 
{ 
	const int numSamplesToProcess = uGenOutput.getBlockSize(); 
	float* const outputSamples = uGenOutput.getSampleData(); 
	const float* const leftOperandSamples = inputs[LeftOperand].processBlock(shouldDelete, blockID, channel); 
	const float* const rightOperandSamples = inputs[RightOperand].processBlock(shouldDelete, blockID, channel); 
	vDSP_vmul(leftOperandSamples, 1, rightOperandSamples, 1, outputSamples, 1, numSamplesToProcess);
}

void BinaryDivideUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw() 
{ 
	const int numSamplesToProcess = uGenOutput.getBlockSize(); 
	float* const outputSamples = uGenOutput.getSampleData(); 
	const float* const leftOperandSamples = inputs[LeftOperand].processBlock(shouldDelete, blockID, channel); 
	const float* const rightOperandSamples = inputs[RightOperand].processBlock(shouldDelete, blockID, channel); 
	
#ifdef UGEN_IPHONE
	for(int i = 0; i < numSamplesToProcess; i++)
	{
		outputSamples[i] = leftOperandSamples[i] / rightOperandSamples[i];
	}
#else	
	vvdivf(outputSamples, leftOperandSamples, rightOperandSamples, &numSamplesToProcess);
#endif
}





END_UGEN_NAMESPACE

#endif
