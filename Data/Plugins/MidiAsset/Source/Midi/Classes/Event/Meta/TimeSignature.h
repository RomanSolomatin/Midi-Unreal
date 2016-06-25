// Copyright -> Scott Bishel

#pragma once

#include "MetaEvent.h"

/**
 * 
 */
class MIDI_API TimeSignature : public MetaEvent
{
public:
	static const int METER_EIGHTH = 12;
	static const int METER_QUARTER = 24;
	static const int METER_HALF = 48;
	static const int METER_WHOLE = 96;

	static const int DEFAULT_METER = METER_QUARTER;
	static const int DEFAULT_DIVISION = 8;

private:
	int mNumerator;
	int mDenominator;
	int mMeter;
	int mDivision;

public:
	TimeSignature();
	TimeSignature(long tick, long delta, int num, int den, int meter, int div);

	void setTimeSignature(int num, int den, int meter, int div);

	int getNumerator();
	int getDenominatorValue();
	int getRealDenominator();
	int getMeter();
	int getDivision();

protected:
	int getEventSize();

public:
	void writeToFile(FMemoryWriter & output);

	static TimeSignature * parseTimeSignature(long tick, long delta, FBufferReader & input);

private:
	int log2(int den);

public:
	int CompareTo(MidiEvent *other);
	string ToString();
};
