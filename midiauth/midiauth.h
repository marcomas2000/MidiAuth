// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MIDIAUTH_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MIDIAUTH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MIDIAUTH_EXPORTS
#define MIDIAUTH_API __declspec(dllexport)
#else
#define MIDIAUTH_API __declspec(dllimport)
#endif

const int _MAXTRACKCHUNKSIZE=8000;
const int _MAXNAMELENGTH=180+1;

/**
* Function: createFile
* Param: filename
* Description: creates and open a file
*/
void MIDIAUTH_API __stdcall createFile(LPCSTR fileName);
/**
* Function: insertHeader
* Param: tickperbeat
* Description: writes the header in a MIDI file. including the "Pulses Per Quarter Note"
*/
void MIDIAUTH_API __stdcall insertHeader(int ticksperbeat);
/**
* Function: closeFile
* Param: 
* Description: closes the MIDI file
*/
void MIDIAUTH_API __stdcall closeFile();
/**
* Function: insertMidiTextEvent
* Param: text
* Description: inserts a Text Midi Event in the File
*/
void MIDIAUTH_API __stdcall insertMidiTextEvent(LPCSTR text);
/**
* Function: insertTempoEvent
* Param: tempo (in beats per minute
* Description: inserts a Tempo Midi Event in the File
*/
void MIDIAUTH_API __stdcall insertTempoEvent(int tempo);
/**
* Function: insertChannelVoice
* Param: channel
* Param: voice
* Description: associates a specific voice to a channel
*/
void MIDIAUTH_API __stdcall insertChannelVoice(char channel, char voice);
/**
* Function: insertVolume
* Param: channel
* Param: volume
* Description: insert the channel volume event
*/
void MIDIAUTH_API __stdcall insertVolume(char channel, int volume);
/**
* Function: insertControllerEvent
* Param: channel
* Param: type
* Param: value
* Description: insert the a type of event, with a particular value, in the channel
*/
void MIDIAUTH_API __stdcall insertControllerEvent(unsigned char channel, unsigned char type, unsigned char value);
/**
* Function: insertNoteOn
* Param: deltatime
* Param: channel
* Param: number
* Param: velocity
* Description: start a note (number), in a channel, with a certain velocity, at a specific time
*/
void MIDIAUTH_API __stdcall insertNoteOn(unsigned int deltatime, unsigned char channel, unsigned char number, unsigned char velocity);
/**
* Function: insertNoteOff
* Param: deltatime
* Param: channel
* Param: number
* Param: velocity
* Description: stop a note (number), in a channel, with a certain velocity, at a specific time
*/
void MIDIAUTH_API __stdcall insertNoteOff(unsigned int deltatime, unsigned char channel, unsigned char number, unsigned char velocity);
/**
* Function: closeTrackChunk
* Description: close a MIDI chunk in the MIDI file
*/
void MIDIAUTH_API __stdcall closeTrackChunk();
/**
* Function: playNoteOn
* Param: deltatime
* Param: channel
* Param: number
* Param: velocity
* Description: start a note (number), in a channel, with a certain velocity, at a specific time
*/
void MIDIAUTH_API __stdcall playNoteOn(int deltatime, unsigned char channel, unsigned char number, unsigned char velocity);
/**
* Function: playNoteOff
* Param: deltatime
* Param: channel
* Param: number
* Param: velocity
* Description: stop a note (number), in a channel, with a certain velocity, after duration
*/
void MIDIAUTH_API __stdcall playNoteOff(int deltatime, unsigned char channel, unsigned char number, unsigned char velocity);

//**********	private:

/**
* Function: insertEventInTrackChunk
* Description: inserts an event in the rappresentation of the MIDI chunk in memory
*/
void __stdcall insertEventInTrackChunk(PUCHAR midievent, int length);
/**
* Function: variableLength2Bytes
* Description: converts a value to the variable length used in MIDI files
*/
void __stdcall variableLength2Bytes(int initialValue, PUCHAR value1, PUCHAR value2);