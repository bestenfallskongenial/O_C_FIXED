#include "kernel.h"
#include <circle/string.h>
#include <circle/debug.h>
#include <assert.h>

static const char FromKernel[] = "kernel";

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Logger (m_Options.GetLogLevel ())
{
	m_ActLED.Blink (5);	// show we are alive
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}
	
	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}
	
	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}
	
	return bOK;
}

TShutdownMode CKernel::Run (void)
{
	m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

	// show the character set on screen
	for (char chChar = ' '; chChar <= '~'; chChar++)
	{
		if (chChar % 8 == 0)
		{
			m_Screen.Write ("\n", 1);
		}

		CString Message;
		Message.Format ("%02X: \'\u001b[7m%c\u001b[0m\' ", (unsigned) chChar, chChar);
		
		m_Screen.Write ((const char *) Message, Message.GetLength ());
	}
	m_Screen.Write ("\n", 1);

#ifndef NDEBUG
	// some debugging features
	m_Logger.Write (FromKernel, LogDebug, "Dumping the start of the ATAGS");
	debug_hexdump ((void *) 0x100, 128, FromKernel);

	m_Logger.Write (FromKernel, LogNotice, "The following assertion will fail");
	assert (1 == 2);
#endif

	return ShutdownHalt;
}