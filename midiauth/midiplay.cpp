#include "stdafx.h"

#include "midiplay.h"


HMIDISTRM m_lphStream;
UINT m_puDeviceID;
unsigned long * m_Phrase;
unsigned long m_phrasePointer;

void __stdcall createStream(UINT deviceID, int size, int tempo, int timediv, int volume)
{
	MMRESULT result;
    //MIDIPROPTEMPO proptmp;
	MIDIPROPTIMEDIV propdiv;

	m_puDeviceID = deviceID;
	//Open MIDI Stream
	result = midiStreamOpen(&m_lphStream, &m_puDeviceID, 1, 0, 0, CALLBACK_NULL);
	// Set PPQN (Pulses Per Quarter Note)
	propdiv.cbStruct = sizeof(MIDIPROPTIMEDIV);
	propdiv.dwTimeDiv = timediv;
	midiStreamProperty(m_lphStream, (LPBYTE)&propdiv, MIDIPROP_SET|MIDIPROP_TIMEDIV);
	m_Phrase = new unsigned long[size];
	m_phrasePointer = 0;
	// Insert MIDI message to set Volume
	m_Phrase[m_phrasePointer] = 0;
	m_phrasePointer++;
	m_Phrase[m_phrasePointer] = 0;
	m_phrasePointer++;
	m_Phrase[m_phrasePointer] = ((unsigned long)volume<<16) | 0x07B0;
	m_phrasePointer++;
	// Insert MIDI message to set Tempo
	m_Phrase[m_phrasePointer] = 0;
	m_phrasePointer++;
	m_Phrase[m_phrasePointer] = 0;
	m_phrasePointer++;
	m_Phrase[m_phrasePointer] = ((unsigned long)MEVT_TEMPO<<24) | (60000000/tempo);
	m_phrasePointer++;
}

void __stdcall playTune()
{
	MIDIHDR midiHdr;
	MMRESULT err;

	/* Store pointer to our stream (ie, buffer) of messages in MIDIHDR */
	midiHdr.lpData = (LPSTR)&m_Phrase[0];

	/* Store its size in the MIDIHDR */
	midiHdr.dwBufferLength = midiHdr.dwBytesRecorded = m_phrasePointer*sizeof(unsigned long);

	/* Flags must be set to 0 */
	midiHdr.dwFlags = 0;

	/* Prepare the buffer and MIDIHDR */
	err = midiOutPrepareHeader((HMIDIOUT)m_lphStream,  &midiHdr, sizeof(MIDIHDR));
	/* Queue the Stream of messages. Output doesn't actually start
		until we later call midiStreamRestart().
	*/
	err = midiStreamOut(m_lphStream, &midiHdr, sizeof(MIDIHDR));
	/* Start outputting the Stream of messages. This should return immediately
		as the stream device will time out and output the messages on its own in
		the background.
	*/
	err = midiStreamRestart(m_lphStream);
	while (MMSYSERR_NOERROR != midiOutUnprepareHeader((HMIDIOUT)m_lphStream, &midiHdr, sizeof(MIDIHDR)))
	{
		Sleep(200);
	}
}

void __stdcall eventNoteOn(int start, unsigned char channel, unsigned char note, unsigned char velocity)
{
	union pos
	{
		int midibyte;
		unsigned char component[4];
	} position;
	position.component[3] = 0x00;
	position.component[2] = velocity;
	position.component[1] = note;
	position.component[0] = 0x90+channel;
	m_Phrase[m_phrasePointer++] = start;
	m_Phrase[m_phrasePointer++] = 0;
	m_Phrase[m_phrasePointer++] = position.midibyte;
}

void __stdcall eventNoteOff(int duration, unsigned char channel, unsigned char note, unsigned char velocity)
{
	union pos
	{
		int midibyte;
		unsigned char component[4];
	} position;
	position.component[3] = 0x00;
	position.component[2] = velocity;
	position.component[1] = note;
	position.component[0] = 0x80+channel;
	m_Phrase[m_phrasePointer++] = duration;
	m_Phrase[m_phrasePointer++] = 0;
	m_Phrase[m_phrasePointer++] = position.midibyte;
}

void __stdcall closeStream()
{
	midiStreamClose(m_lphStream);
        if (m_Phrase != 0)
        {
            delete [] m_Phrase;
            m_Phrase = 0;
        }
}
