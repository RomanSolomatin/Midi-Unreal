// Copyright -> Scott Bishel

#pragma once

#include "SoundUtils.generated.h"

/**
 * 
 */
UCLASS()
class MIDI_API USoundUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static USoundWave* CreateSoundWave(const TArray<int>& buffer);
	static USoundWave* CreateSoundWave(uint8* buffer, int32 size);

	/**
	* Creates a sin wave that can be used by the audio compoonent
	* @param amplitude - Intensity [Similar to volume]
	* @param frequency - Pitch[Hz]
	* @param seconds - Duration
	*/
	UFUNCTION(BlueprintCallable, Category = "Midi|Audio")
	static USoundWave* AudioNote(float amplitude = 16384.0f, float frequency = 440.0f, float seconds = 1.0f);

	/**
	* Creates a continuous sin wave that can be used by the audio compoonent
	* @param amplitude - Intensity [Similar to volume]
	* @param frequency - Pitch[Hz]
	*/
	UFUNCTION(BlueprintCallable, Category = "Midi|Audio")
	static USoundWave* ConstantNote(float amplitude = 8192.0f, float frequency = 440.0f);
};