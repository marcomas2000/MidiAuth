// miditest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "midiauth.h"
#include "midimessage.h"
#include "midiplay.h"


const int TICKSPERBEAT = 480;

int _tmain(int argc, _TCHAR* argv[])
{
	/*******************************************
	createFile("test.midi");
	insertHeader(180);
	insertTempoEvent(120);
	insertMidiTextEvent("Land of Art");
	insertMidiTextEvent("");
	closeTrackChunk();
	insertChannelVoice( 0x0, 0x0);
	insertVolume(0x0, 100);
	insertControllerEvent(0x0, 0x5B, 0);
	insertControllerEvent(0x0, 0x5D, 0);
	insertControllerEvent(0x0, 0x0a, 0x40);
	insertNoteOn(0, 0, 60, 0x40);
	insertNoteOff(180, 0, 60, 0x40);
	insertNoteOn(0, 0, 64, 0x40);
	insertNoteOff(180, 0, 64, 0x40);
	insertNoteOn(0, 0, 67, 0x40);
	insertNoteOff(180, 0, 67, 0x40);
	insertNoteOn(0, 0, 72, 0x40);
	insertNoteOff(180, 0, 72, 0x40);
	insertNoteOn(0, 0, 67, 0x40);
	insertNoteOff(180, 0, 67, 0x40);
	insertNoteOn(0, 0, 64, 0x40);
	insertNoteOff(180, 0, 64, 0x40);
	insertNoteOn(0, 0, 60, 0x40);
	insertNoteOff(360, 0, 60, 0x40);
	closeTrackChunk();
	closeFile();

	unsigned char message[4];

	initialize("RTClient");

	openPort(0, "RtMidi");

	message[0] = 0xB0;
	message[1] = 0x07;
    message[2] = 0x7f;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 60;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 60;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 64;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 64;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 67;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 67;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 72;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 72;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 67;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 67;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 64;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 64;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	message[0] = 0x90;
    message[1] = 60;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );

    message[0] = 0x80;
    message[1] = 60;
    message[2] = 0x64;
    sendMessage( message[0], message[1], message[2], 3 );
	Sleep(300);

	closePort();
//*************************************************************/
    createStream(0, 50, 120, TICKSPERBEAT, 100);

	eventNoteOn(0, 0, 60, 0x64);
	eventNoteOff(480, 0, 60, 0x64);
	eventNoteOn(0, 0, 64, 0x64);
	eventNoteOff(480, 0, 64, 0x64);
	eventNoteOn(0, 0, 67, 0x64);
	eventNoteOff(480, 0, 67, 0x64);
	eventNoteOn(0, 0, 72, 0x64);
	eventNoteOff(480, 0, 72, 0x64);
	eventNoteOn(0, 0, 67, 0x64);
	eventNoteOff(480, 0, 67, 0x64);
	eventNoteOn(0, 0, 64, 0x64);
	eventNoteOff(480, 0, 64, 0x64);
	eventNoteOn(0, 0, 60, 0x64);
	eventNoteOff(480, 0, 60, 0x64);

	playTune();
	closeStream();

	return 0;
}

