// $Id: ugen_Bits.h 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/core/ugen_Bits.h $

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
// most of this appears to be "common knowledge"?

#ifndef _UGEN_ugen_Bits_H_
#define _UGEN_ugen_Bits_H_

/** For bit pattern conversion between a few types. */
typedef union 
{ 
	unsigned long u;
	long i;
	float f;
} Element;

/** Various bitwise utilities.
 
 All functions are statics at present but this may develop into a bits class
 (perhaps similar to the juce::BitArray) */
class Bits
{
public:
	static inline long countLeadingZeros(long x)
	{
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return(32 - countOnes(x));
	}

	static inline long countTrailingZeros(long x) 
	{
		return 32 - countLeadingZeros(~x & (x-1));
	}
	
	static inline long countLeadingOnes(long x) 
	{
		return countLeadingZeros(~x);
	}
	
	static inline long countTrailingOnes(long x) 
	{
		return 32 - countLeadingZeros(x & (~x-1));
	}
	
	static inline long numBitsRequired(long x) 
	{
		return 32 - countLeadingZeros(x);
	}
	
	static inline long log2Ceil(long x) 
	{
		return 32 - countLeadingZeros(x - 1);
	}
	
	static inline long nextPowerOf2(long x) 
	{
		return 1L << log2Ceil(x);
	}
	
	static inline bool isPowerOf2(long x) 
	{
		return (x & (x-1)) == 0;
	}

	static inline unsigned long countOnes(unsigned long x)
	{
		x -= ((x >> 1) & 0x55555555);
        x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
        x = (((x >> 4) + x) & 0x0f0f0f0f);
        x += (x >> 8);
        x += (x >> 16);
        return(x & 0x0000003f);		
	}
	
	static inline void endianSwap(unsigned short& x)
	{
		x = (x >> 8) | 
			(x << 8);
	}
	
	static inline void endianSwap(short& x)
	{
		return endianSwap((unsigned short&)x);
	}
	
	static inline void endianSwap(unsigned int& x)
	{
		x = (x >> 24) | 
			((x << 8) & 0x00FF0000) |
			((x >> 8) & 0x0000FF00) |
			(x << 24);
		
	}
	
	static inline void endianSwap(int& x)
	{
		return endianSwap((unsigned int&)x);
	}
	
};

#endif // _UGEN_ugen_Bits_H_
