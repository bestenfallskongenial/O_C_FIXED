
#include <circle/util.h>
#include <stdio.h>
#include <string.h>

CLogger*        CLogger::s_pThis = 0;
TLoggerRawSink* CLogger::s_pSink = 0;

CLogger::CLogger(unsigned nLogLevel, CTimer* pTimer, boolean bOverwriteOldest)
:               m_nLogLevel(nLogLevel),
                m_pPanicHandler(0)
{
                s_pThis = this;
}

CLogger::~CLogger(void)
{
                if (s_pThis == this)
                    {
                    s_pThis = 0;
                    }
}

boolean CLogger::Initialize(CDevice* pTarget)
{
                return TRUE;
}

void CLogger::SetNewTarget(CDevice* pTarget)
{
}

CLogger* CLogger::Get(void)
{
                if (s_pThis == 0)
                    {
                    new CLogger(LogPanic);
                    }

                return s_pThis;
}

void CLogger::SetRawSink(TLoggerRawSink* pSink)
{
                s_pSink = pSink;
}

void CLogger::RegisterPanicHandler(TLogPanicHandler* pHandler)
{
                m_pPanicHandler = pHandler;
}

void CLogger::WriteRaw(const char* pText, unsigned nLength)
{
                if (pText == 0) return;
                if (nLength == 0) return;
                if (s_pSink == 0) return;

                s_pSink(pText, nLength);
}

void CLogger::WriteCString(const char* pText)
{
                if (pText == 0) return;

                WriteRaw(pText, strlen(pText));
}

void CLogger::Write(const char* pSource,
                    TLogSeverity Severity,
                    const char* pMessage,
                    ...)
{
                va_list Args;
                va_start(Args, pMessage);

                WriteV(pSource, Severity, pMessage, Args);

                va_end(Args);
}
/*
void CLogger::WriteV(const char* pSource,
                     TLogSeverity Severity,
                     const char* pMessage,
                     va_list Args)
{
                if (Severity > m_nLogLevel) return;

                char Message[256];

                vsnprintf(Message, sizeof Message, pMessage, Args);
                Message[sizeof Message - 1] = '\0';

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(Message);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}
*/
void CLogger::WriteV(const char* pSource,
                     TLogSeverity Severity,
                     const char* pMessage,
                     va_list Args)
{
                (void) Args;

                if (Severity > m_nLogLevel) return;
                if (pMessage == 0) return;

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(pMessage);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}

void CLogger::WriteNoAlloc(const char* pSource,
                           TLogSeverity Severity,
                           const char* pMessage)
{
                if (Severity > m_nLogLevel) return;

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(pMessage);
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0)
                    {
                    (*m_pPanicHandler)();
                    }
}