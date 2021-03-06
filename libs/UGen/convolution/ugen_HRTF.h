// $Id: ugen_HRTF.h 350 2011-07-22 09:07:56Z 0x4d52 $
// $HeadURL: http://ugen.googlecode.com/svn/trunk/UGen/convolution/ugen_HRTF.h $

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
 

 HRTF data from the work at MIT by Bill Gardner and Keith Martin
 http://sound.media.mit.edu/resources/KEMAR.html
 
 ==============================================================================
 */

#ifndef _UGEN_ugen_HRTF_H_
#define _UGEN_ugen_HRTF_H_

#include "../core/ugen_Collections.h"
#include "../buffers/ugen_Buffer.h"

/** Encapsulates a collection of HRTFs.
 In fact this stores HRIRs (Head Related Impulse Responses) rather than HRTFs
 (Head Related Transfer Functions) but since the latter is a more well-known term
 the name HRTF was chosen for the class. This gives access to the 1994 work by
 Gardner and Martin by returning an HRIR in a stereo Buffer from a given azimuth
 and elevation. This can then be used via either TimeConvolve or PartConvolve to
 perform convolution and thus apply the HRTF to a real-time signal.
 
 See http://sound.media.mit.edu/resources/KEMAR.html
 
 For example:
 @code
	Buffer hrir = HRTF::getClosestResponseDegrees(30, 10); // 30 degrees to the right, 10 degrees elevated
	UGen conv = PartConvolve::AR(input, hrir); // convolve with a signal 'input'
 @endcode */
class HRTF
{
public:
	static HRTF& getInstance();
	
	static Buffer getClosestResponse(const float azimuth, const float elevation = 0.f) throw();
	static Buffer getClosestResponseDegrees(const int azimuth, const int elevation = 0) throw();
//	static Buffer getResponse(const float azimuth, const float elevation = 0.f) throw();
//	static Buffer getResponseDegrees(const int azimuth, const int elevation = 0) throw();	
	static int closestElevation(const int elevationDegrees) throw();
	static int closestAzimuth(const int elevationDegrees, const int azimuthDegrees) throw();
	
	
private:
	HRTF() throw();
	void addResponse(Text const& key, const short* left, const short* right) throw();

	Dictionary<Buffer> database;
};

#endif // _UGEN_ugen_HRTF_H_