// Copyright -> Scott Bishel

#include "MidiPrivatePCH.h"
#include "MidiEvent.h"

#include "ChannelAftertouch.h"
#include "ChannelEvent.h"
#include "CController.h"
#include "NoteAftertouch.h"
#include "NoteOff.h"
#include "NoteOn.h"
#include "PitchBend.h"
#include "ProgramChange.h"
#include "SystemExclusiveEvent.h"

#include "Meta/MetaEvent.h"
#include <sstream>

MidiEvent::MidiEvent(long tick, long delta) : mDelta(NULL), mType(-1)  {
	mTick = tick;
	mDelta = new VariableLengthInt((int)delta);
}
MidiEvent::~MidiEvent()
{
	if (mDelta != NULL)
		delete mDelta;
	mDelta = NULL;
}

int MidiEvent::getType() {
	return mType;
}

long MidiEvent::getTick() {
	return mTick;
}
long MidiEvent::getDelta() {
	return mDelta->getValue();
}

void MidiEvent::setDelta(long d) {
	mDelta->setValue((int)d);
}

int MidiEvent::getSize() {
	return getEventSize() + mDelta->getByteCount();
}

bool MidiEvent::requiresStatusByte(MidiEvent * prevEvent) {
	if (prevEvent == NULL) {
		return true;
	}

	if (typeid(this) == typeid(MetaEvent)) {
		return true;
	}

	if (this->getType() == prevEvent->getType()) {
		return false;
	}
	return true;
}

void MidiEvent::writeToFile(FMemoryWriter & output, bool writeType){
	output.Serialize(mDelta->getBytes(), mDelta->getByteCount());
}

int MidiEvent::sId = -1;
int MidiEvent::sType = -1;
int MidiEvent::sChannel = -1;

MidiEvent * MidiEvent::parseEvent(long tick, long delta, FBufferReader & input){

//	int64 pos = input.Tell();
	bool reset = false;
	
	int id = 0;
	input.Serialize(&id, 1);
	if (!verifyIdentifier(id)) {
		// move back one bytes
		input.Seek(input.Tell() - 1);
		reset = true;
	}

	if (sType >= 0x8 && sType <= 0xE) {

		return ChannelEvent::parseChannelEvent(tick, delta, sType, sChannel, input);
	}
	else if (sId == 0xFF) {

		return MetaEvent::parseMetaEvent(tick, delta, input);
	}
	else if (sId == 0xF0 || sId == 0xF7) {

		VariableLengthInt size(input);
		char * data = new char[size.getValue()];
		input.Serialize(data, size.getValue());
		return new SystemExclusiveEvent(sId, tick, delta, data);
	}
	else {
//		OutputDebugStringA("Unknown Status Type: " + sId);
		UE_LOG(LogTemp, Warning, TEXT("Unknown Status Type: %d"), sId);

		if (reset) {
			input.Seek(input.Tell() + 1);
		}
	}

	return NULL;
}

bool MidiEvent::verifyIdentifier(int id) {

	int type = id >> 4;
	int channel = id & 0x0F;

	if (type >= 0x8 && type <= 0xE) {
		sId = id;
		sType = type;
		sChannel = channel;
	}
	else if (id == 0xFF) {
		sId = id;
		sType = -1;
		sChannel = -1;
	}
	else if (type == 0xF) {
		sId = id;
		sType = type;
		sChannel = -1;
	}
	else {
		return false;
	}
	return true;
}

int MidiEvent::CompareTo(MidiEvent *other)
{
	if (mTick != other->getTick()) {
		return mTick < other->getTick() ? -1 : 1;
	}
	if (mDelta->getValue() != other->getDelta()) {
		return mDelta->getValue() < other->getDelta() ? 1 : -1;
	}

	if (!(other->getType() == this->getType())) {
		return 1;
	}

	return 0;
}

string MidiEvent::ToString()
{
//	FString::Printf(TEXT("%d (%d): %s"), mTick, mDelta->getValue(), typeid(this).name());
	std::stringstream ss;
	ss << mTick << " (" << mDelta->getValue() << "): " << typeid(this).name();
	return ss.str();
}