// midiauth.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include "midiauth.h"

/**
* File descriptor
*/
FILE * m_midifd;

/**
* File name
*/
char m_midifilename[_MAXNAMELENGTH];

/**
* Structure holding the chunk in memory
*/
unsigned char m_holdingchunk[_MAXTRACKCHUNKSIZE];

/**
* index pointing to the first available position in the vector holding the chunk
*/
int m_holdingchunkpointer;

void __stdcall createFile(LPCSTR fileName)
{
	strncpy_s(m_midifilename, fileName, _MAXNAMELENGTH);
	fopen_s(&m_midifd, m_midifilename, "wb");
}

void __stdcall insertHeader(int ticksperbeat)
{
	// write MIDI file header chunk
	if (m_midifd != 0)
	{
		unsigned char firstbyte = ticksperbeat & 0x00ff;
		unsigned char secondbyte = (ticksperbeat & 0xff00) >> 8;
		fprintf(m_midifd, "MThd%c%c%c%c%c%c%c%c%c%c", 0x0, 0x0, 0x0, 0x6, 0x0, 0x1, 0x0, 0x2,
					secondbyte, firstbyte);
	}
}

void __stdcall insertEventInTrackChunk(PUCHAR midievent, int length)
{
	int i;
	for(i=0; i < length; i++)
	{
		m_holdingchunk[m_holdingchunkpointer+i] = midievent[i];
	}
	m_holdingchunkpointer+=i;
}

void __stdcall closeTrackChunk()
{
	// write ID and size. Size comes from the current value of m_holdingchunkpointer
	// plus the 4 bytes for the end-of-chunk event (00 FF 2F 00)

	union tr
	{
		int trackLength;
		char length[4];
	}transfer;

	transfer.trackLength = m_holdingchunkpointer +4;
// Takes care on endianness on different platforms ad MIDI requires data in little endian form
	fprintf(m_midifd, "MTrk%c%c%c%c", transfer.length[3], transfer.length[2],
										transfer.length[1], transfer.length[0]);
    for(int i=0; i<m_holdingchunkpointer; i++)
	{
		fprintf(m_midifd, "%c", m_holdingchunk[i]);
		fflush(m_midifd);
	}
	fprintf(m_midifd, "%c%c%c%c", 0x00, 0xFF, 0x2F, 0x00);
	// reset pointer in the buffer
	m_holdingchunkpointer = 0;
}

void __stdcall closeFile()
{
	fclose(m_midifd);
}

void __stdcall insertMidiTextEvent(LPCSTR text)
{
	unsigned char midievent[200];
        unsigned int event_ind;

	midievent[0] = 0x00;
	midievent[1] = 0xFF;
	midievent[2] = 0x01;
	midievent[3] = strlen((LPCSTR)text);
	for (event_ind=0; event_ind < strlen((LPCSTR)text); event_ind++)
	{
		midievent[4+event_ind] = text[event_ind];
	}
	insertEventInTrackChunk(midievent, event_ind+4);
}

void __stdcall insertTempoEvent(int tempo)
{
	unsigned char midievent[200];

	midievent[0] = 0x00;
	midievent[1] = 0xFF;
	midievent[2] = 0x51;
	midievent[3] = 0x03;
	int microsendsperquarternote = 60000000/tempo;
	midievent[4] = (microsendsperquarternote & 0x00ff0000) >> 16;
	midievent[5] = (microsendsperquarternote & 0x0000ff00) >> 8;
	midievent[6] = microsendsperquarternote & 0x000000ff;
	insertEventInTrackChunk(midievent, 7);
}

void __stdcall insertVolume(char channel, int volume)
{
	unsigned char midievent[200];

	midievent[0] = 0x00;
	midievent[1] = 0xB0+channel;
	midievent[2] = 0x07;
	unsigned char volch = (volume & 0x00FF);
	midievent[3] = volch;
	insertEventInTrackChunk(midievent, 4);
}

void __stdcall insertChannelVoice(char channel, char voice)
{
	unsigned char midievent[200];

	midievent[0] = 0x00;
	midievent[1] = 0xC0+channel;
	midievent[2] = voice;
	insertEventInTrackChunk(midievent, 3);
}

void __stdcall insertControllerEvent(unsigned char channel, unsigned char type, unsigned char value)
{
	unsigned char midievent[200];

	midievent[0] = 0x00;
	midievent[1] = 0xB0+channel;
	midievent[2] = type;
	midievent[3] = value;
	insertEventInTrackChunk(midievent, 4);
}

void __stdcall insertNoteOn(unsigned int deltatime, unsigned char channel, unsigned char number, unsigned char velocity)
{
	unsigned char midievent[200];
	
	if ((deltatime > 0x7fffffff) && (deltatime < 0xffffffff))
	{
	}
	else if (deltatime > 0x7fffff)
	{
	}
	else if (deltatime > 0x7fff)
	{
	}
	else if (deltatime > 0x7f)
	{
		unsigned char value1, value2;

		variableLength2Bytes(deltatime,&value1, &value2);
		midievent[0] = value1;
		midievent[1] = value2;
		midievent[2] = 0x90+channel;
		midievent[3] = number;
		midievent[4] = velocity;
		insertEventInTrackChunk(midievent, 5);
	}
	else
	{
		if (deltatime == 0)
		{
			midievent[0] = 0;
			midievent[1] = 0x90+channel;
			midievent[2] = number;
			midievent[3] = velocity;
			insertEventInTrackChunk(midievent, 4);
		}
		else
		{
			midievent[0] = 3;
			midievent[1] = 0x90+channel;
			midievent[2] = number;
			midievent[3] = velocity;
			insertEventInTrackChunk(midievent, 4);
		}
	}
}

void __stdcall insertNoteOff(unsigned int deltatime, unsigned char channel, unsigned char number, unsigned char velocity)
{
	unsigned char midievent[200];

	if ((deltatime > 0x7fffffff) && (deltatime < 0xffffffff))
	{
	}
	else if (deltatime > 0x7fffff)
	{
	}
	else if (deltatime > 0x7fff)
	{
	}
	else if (deltatime > 0x7f)
	{
		unsigned char value1, value2;

		variableLength2Bytes(deltatime,&value1, &value2);
		midievent[0] = value1;
		midievent[1] = value2;
		midievent[2] = 0x80+channel;
		midievent[3] = number;
		midievent[4] = velocity;
		insertEventInTrackChunk(midievent, 5);
	}
	else
	{
		midievent[0] = deltatime;
		midievent[1] = 0x80+channel;
		midievent[2] = number;
		midievent[3] = velocity;
		insertEventInTrackChunk(midievent, 4);
	}
}

void __stdcall variableLength2Bytes(int initialValue, PUCHAR value1, PUCHAR value2)
{
	*value2 = initialValue & 0x7f;
	initialValue = initialValue >> 7;
	*value1 = initialValue + 0x80;
}

/************************************************************************************
void playNoteOn(int start, unsigned char channel, unsigned char number, unsigned char velocity)
{
	eventNoteOn(start, channel, number, velocity);
}

void playNoteOff(int duration, unsigned char channel, unsigned char number, unsigned char velocity)
{
	eventNoteOff(duration, channel, number, velocity);
}
***************************************************************************************/