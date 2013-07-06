#pragma once

#include "ofMain.h"

#ifdef B0
#undef B0
#endif

namespace ofxUGen
{
	enum
	{
		A0 = 21,
		As0 = 22,
		Bb0 = 22,
		B0 = 23,
		C1 = 24,
		Cs1 = 25,
		Db1 = 25,
		D1 = 26,
		Ds1 = 27,
		Eb1 = 27,
		E1 = 28,
		F1 = 29,
		Fs1 = 30,
		Gb1 = 30,
		G1 = 31,
		Gs1 = 32,
		Ab1 = 32,
		A1 = 21,
		As1 = 22,
		Bb1 = 22,
		B1 = 23,
		C2 = 36,
		Cs2 = 37,
		Db2 = 37,
		D2 = 38,
		Ds2 = 39,
		Eb2 = 39,
		E2 = 40,
		F2 = 41,
		Fs2 = 42,
		Gb2 = 42,
		G2 = 43,
		Gs2 = 44,
		Ab2 = 44,
		A2 = 33,
		As2 = 34,
		Bb2 = 34,
		B2 = 35,
		C3 = 48,
		Cs3 = 49,
		Db3 = 49,
		D3 = 50,
		Ds3 = 51,
		Eb3 = 51,
		E3 = 52,
		F3 = 53,
		Fs3 = 54,
		Gb3 = 54,
		G3 = 55,
		Gs3 = 56,
		Ab3 = 56,
		A3 = 45,
		As3 = 46,
		Bb3 = 46,
		B3 = 47,
		C4 = 60,
		Cs4 = 61,
		Db4 = 61,
		D4 = 62,
		Ds4 = 63,
		Eb4 = 63,
		E4 = 64,
		F4 = 65,
		Fs4 = 66,
		Gb4 = 66,
		G4 = 67,
		Gs4 = 68,
		Ab4 = 68,
		A4 = 57,
		As4 = 58,
		Bb4 = 58,
		B4 = 59,
		C5 = 72,
		Cs5 = 73,
		Db5 = 73,
		D5 = 74,
		Ds5 = 75,
		Eb5 = 75,
		E5 = 76,
		F5 = 77,
		Fs5 = 78,
		Gb5 = 78,
		G5 = 79,
		Gs5 = 80,
		Ab5 = 80,
		A5 = 69,
		As5 = 70,
		Bb5 = 70,
		B5 = 71,
		C6 = 84,
		Cs6 = 85,
		Db6 = 85,
		D6 = 86,
		Ds6 = 87,
		Eb6 = 87,
		E6 = 88,
		F6 = 89,
		Fs6 = 90,
		Gb6 = 90,
		G6 = 91,
		Gs6 = 92,
		Ab6 = 92,
		A6 = 81,
		As6 = 82,
		Bb6 = 82,
		B6 = 83,
		C7 = 96,
		Cs7 = 97,
		Db7 = 97,
		D7 = 98,
		Ds7 = 99,
		Eb7 = 99,
		E7 = 100,
		F7 = 101,
		Fs7 = 102,
		Gb7 = 102,
		G7 = 103,
		Gs7 = 104,
		Ab7 = 104,
		A7 = 93,
		As7 = 94,
		Bb7 = 94,
		B7 = 95,
		C8 = 108
	};

	inline float ftom(float freq)
	{
		return (12 * log2f(freq / 440.)) + 69;
	}

	inline float mtof(float note)
	{
		float freq;
		note -= 69;
		freq = powf(2, note / 12.) * 440;
		return freq;
	}

	struct Chord
	{
		enum
		{
			ROOT = 0,
			THIRD = 4,
			FORTH = 5,
			FIFTH = 7,
			SEVENTH = 11
		};

		static vector<float> make_chord(float note, float *tone, size_t num)
		{
			vector<float> notes;
			for (int i = 0; i < num; i++)
				notes.push_back(note + tone[i]);
			return notes;
		}

		static vector<float> major(float note)
		{
			float tone[] = {ROOT, THIRD, FIFTH};
			return make_chord(note, tone, 3);
		}

		static vector<float> minor(float note)
		{
			float tone[] = {ROOT, THIRD-1, FIFTH};
			return make_chord(note, tone, 3);
		}

		static vector<float> major7th(float note)
		{
			float tone[] = {ROOT, THIRD, FIFTH, SEVENTH};
			return make_chord(note, tone, 4);
		}

		static vector<float> minor7th(float note)
		{
			float tone[] = {ROOT, THIRD-1, FIFTH, SEVENTH-1};
			return make_chord(note, tone, 4);
		}

		static vector<float> _7th(float note)
		{
			float tone[] = {ROOT, THIRD, FIFTH, SEVENTH-1};
			return make_chord(note, tone, 4);
		}

		static vector<float> sus4(float note)
		{
			float tone[] = {ROOT, FORTH, FIFTH};
			return make_chord(note, tone, 3);
		}

		static vector<float> dim(float note)
		{
			float tone[] = {ROOT, THIRD-1, FIFTH-1};
			return make_chord(note, tone, 3);
		}

		static vector<float> aug(float note)
		{
			float tone[] = {ROOT, THIRD, FIFTH+1};
			return make_chord(note, tone, 3);
		}

	};

}

