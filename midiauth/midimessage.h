#ifndef RTMIDI_H
#define RTMIDI_H

#ifdef MIDIAUTH_EXPORTS
#define MIDIAUTH_API __declspec(dllexport)
#else
#define MIDIAUTH_API __declspec(dllimport)
#endif

/*!
An optional port number greater than 0 can be specified.
Otherwise, the default or first port found is opened.  An
exception is thrown if an error occurs while attempting to make
the port connection.
*/
void MIDIAUTH_API __stdcall openPort( unsigned int portNumber, LPCSTR portName);

//! Close an open MIDI connection (if one exists).
void MIDIAUTH_API __stdcall  closePort();

//! Return the number of available MIDI output ports.
unsigned int MIDIAUTH_API  __stdcall getPortCount();

//! Immediately send a single message out an open MIDI output port.
/*!
An exception is thrown if an error occurs during output or an
output connection was not previously established.
*/
void MIDIAUTH_API __stdcall  sendMessage( unsigned char b0, unsigned char b1, unsigned char b2, int size );

void MIDIAUTH_API __stdcall initialize( LPCSTR clientName );

#endif
