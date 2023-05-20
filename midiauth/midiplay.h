#ifndef MIDIPLAY_H
#define MIDIPLAY_H

#ifdef MIDIAUTH_EXPORTS
#define MIDIAUTH_API __declspec(dllexport)
#else
#define MIDIAUTH_API __declspec(dllimport)
#endif

#include <windows.h>
#include <mmsystem.h>

	void MIDIAUTH_API __stdcall createStream(UINT deviceID, int size, int tempo, int timediv, int volume);
	void MIDIAUTH_API __stdcall eventNoteOn(int start, unsigned char channel, unsigned char note, unsigned char velocity);
	void MIDIAUTH_API __stdcall eventNoteOff(int duration, unsigned char channel, unsigned char note, unsigned char velocity);
	void MIDIAUTH_API __stdcall playTune();
	void MIDIAUTH_API __stdcall closeStream();
 

#endif // MIDIPLAY_H
