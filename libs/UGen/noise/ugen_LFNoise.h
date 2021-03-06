// $Id: ugen_LFNoise.h 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/noise/ugen_LFNoise.h $

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

#ifndef _UGEN_ugen_LFNoise_H_
#define _UGEN_ugen_LFNoise_H_

#include "../core/ugen_UGen.h"
#include "../basics/ugen_MulAdd.h"
#include "../core/ugen_Random.h"

/** @ingroup UGenInternals */
class LFNoise0UGenInternal : public UGenInternal
{
public:
	LFNoise0UGenInternal(UGen const& freq) throw();
	UGenInternal* getChannel(const int channel) throw();
	void processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw();
	
	enum Inputs { Freq, NumInputs };
	
protected:
	Ran088 random;
	float currentValue;
	int counter;
};

/** @ingroup UGenInternals */
class LFNoise1UGenInternal : public UGenInternal
{
public:
	LFNoise1UGenInternal(UGen const& freq) throw();
	UGenInternal* getChannel(const int channel) throw();
	void processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw();
	
	enum Inputs { Freq, NumInputs };
	
protected:
	Ran088 random;
	float currentValue, slope;
	int counter;
};

/** @ingroup UGenInternals */
class LFNoise2UGenInternal : public UGenInternal
{
public:
	LFNoise2UGenInternal(UGen const& freq) throw();
	UGenInternal* getChannel(const int channel) throw();
	void processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw();
	
	enum Inputs { Freq, NumInputs };
	
protected:
	Ran088 random;
	float currentValue, nextValue, nextMidPoint, curve, slope;
	int counter;
};

#define LFNoise_Docs	@param freq The approximate rate at which to generate random values.

/** Generates random values at a specified rate. 
 This is given by the nearest integer division of the sample rate 
 by the freq argument. 
 @see LFNoise1, LFNoise2
 @ingroup AllUGens NoiseUGens */
ExpandingMulAddUGenDeclaration(LFNoise0, 
							   (freq), 
							   (freq, MulAdd_ArgsCall), 
							   (UGen const& freq = 500.f), 
							   (UGen const& freq = 500.f, MulAdd_ArgsDeclare), 
							   COMMON_UGEN_DOCS LFNoise_Docs MulAddArgs_Docs);

/** Generates linearly interpolated random values at a specified rate. 
 This is given by the nearest integer division of the sample rate 
 by the freq argument. 
 @see LFNoise0, LFNoise2
 @ingroup AllUGens NoiseUGens */
ExpandingMulAddUGenDeclaration(LFNoise1, 
							   (freq), 
							   (freq, MulAdd_ArgsCall), 
							   (UGen const& freq = 500.f), 
							   (UGen const& freq = 500.f, MulAdd_ArgsDeclare), 
							   COMMON_UGEN_DOCS LFNoise_Docs MulAddArgs_Docs);

/** Generates quadratically interpolated random values at a specified rate. 
 This is given by the nearest integer division of the sample rate 
 by the freq argument. 
 @see LFNoise0, LFNoise1
 @ingroup AllUGens NoiseUGens */
ExpandingMulAddUGenDeclaration(LFNoise2, 
							   (freq), 
							   (freq, MulAdd_ArgsCall), 
							   (UGen const& freq = 500.f), 
							   (UGen const& freq = 500.f, MulAdd_ArgsDeclare), 
							   COMMON_UGEN_DOCS LFNoise_Docs MulAddArgs_Docs);



#endif // _UGEN_ugen_LFNoise_H_
