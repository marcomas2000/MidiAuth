#include "stdafx.h"
#include <mmsystem.h>
#include <sstream>

#include "midimessage.h"

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct WinMidiData
{
	  HMIDIOUT outHandle;  // Handle to Midi Output Device
};

enum
{
		WARNING,           /*!< A non-critical error. */
		DEBUG_WARNING,     /*!< A non-critical error which might be useful for debugging. */
		UNSPECIFIED,       /*!< The default, unspecified error type. */
		NO_DEVICES_FOUND,  /*!< No devices found on system. */
		INVALID_DEVICE,    /*!< An invalid device ID was specified. */
		INVALID_STREAM,    /*!< An invalid stream ID was specified. */
		MEMORY_ERROR,      /*!< An error occured during memory allocation. */
		INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
		DRIVER_ERROR,      /*!< A system driver error occured. */
		SYSTEM_ERROR,      /*!< A system error occured. */
		THREAD_ERROR       /*!< A thread error occured. */
} Type;

WinMidiData * apiData_;
bool connected_;

unsigned int __stdcall getPortCount()
{
  return midiOutGetNumDevs();
}

void __stdcall initialize( LPCSTR clientName )
{
	int error; 
	// We'll issue a warning here if no devices are available but not
	// throw an error since the user can plug something in later.
	unsigned int nDevices = midiOutGetNumDevs();
	if ( nDevices == 0 ) 
	{
		error = WARNING;
	}

	// Save our api-specific connection information.
	apiData_ = new WinMidiData;
}

void __stdcall openPort( unsigned int portNumber, LPCSTR portName )
{
	int error; 
	if ( connected_ ) 
	{
		error = WARNING;
		return;
	}

	unsigned int nDevices = midiOutGetNumDevs();
	if (nDevices < 1) 
	{
		error = WARNING;
	}

	std::ostringstream ost;
	if ( portNumber >= nDevices ) 
	{
		error = WARNING;
	}
	MMRESULT result = midiOutOpen( &apiData_->outHandle,
                                 portNumber,
                                 (DWORD)NULL,
                                 (DWORD)NULL,
                                 CALLBACK_NULL );
	if ( result != MMSYSERR_NOERROR ) 
	{
		error = WARNING;
	}
	connected_ = true;
}

void __stdcall closePort( void )
{
	if ( connected_ ) 
	{
	 midiOutReset( apiData_->outHandle );
	 midiOutClose( apiData_->outHandle );
	 connected_ = false;
	}
}

void __stdcall sendMessage( BYTE b0, BYTE b1, BYTE b2, int size )
{
	int error; 
	unsigned int nBytes = size;
	if ( nBytes == 0 )
	{
		error = WARNING;
		return;
	}
	BYTE message[3];
	message[0] = b0;
	message[1] = b1;
	message[2] = b2;
	MMRESULT result;
	if ( message[0] == 0xF0 ) 
	{ // Sysex message

		// Allocate buffer for sysex data.
		char *buffer = (char *) malloc( nBytes );
		if ( buffer == NULL ) 
		{
			error = WARNING;
		}

		// Copy data to buffer.
		for ( unsigned int i=0; i<nBytes; i++ ) buffer[i] = message[i];

		// Create and prepare MIDIHDR structure.
		MIDIHDR sysex;
		sysex.lpData = (LPSTR) buffer;
		sysex.dwBufferLength = nBytes;
		sysex.dwFlags = 0;
		result = midiOutPrepareHeader( apiData_->outHandle,  &sysex, sizeof(MIDIHDR) ); 
		if ( result != MMSYSERR_NOERROR ) 
		{
			free( buffer );
			error = WARNING;
		}

		// Send the message.
		result = midiOutLongMsg( apiData_->outHandle, &sysex, sizeof(MIDIHDR) );
		if ( result != MMSYSERR_NOERROR )
		{
			free( buffer );
			error = WARNING;
		}

		// Unprepare the buffer and MIDIHDR.
		while ( MIDIERR_STILLPLAYING == midiOutUnprepareHeader( apiData_->outHandle, &sysex, sizeof (MIDIHDR) ) )
		{
			Sleep( 1 );
		}
		free( buffer );
	}
	else 
	{ // Channel or system message.

		// Make sure the message size isn't too big.
		if ( nBytes > 3 ) 
		{
			error = WARNING;
			return;
		}

		// Pack MIDI bytes into double word.
		DWORD packet;
		unsigned char *ptr = (unsigned char *) &packet;
		for ( unsigned int i=0; i<nBytes; i++ ) 
		{
			*ptr = message[i];
			ptr++;
		}

		// Send the message immediately.
		result = midiOutShortMsg( apiData_->outHandle, packet );
		if ( result != MMSYSERR_NOERROR ) 
		{
			error = WARNING;
		}
	}
}