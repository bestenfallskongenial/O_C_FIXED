#include "CLogger.h"

#include <stdio.h>
#include <string.h>

CLogger*        CLogger::s_pThis = 0;
TLoggerRawSink* CLogger::s_pSink = 0;
void*           CLogger::s_pSinkContext = 0;

CLogger::CLogger(unsigned nLogLevel, CTimer* pTimer, boolean bOverwriteOldest)
:               m_nLogLevel(nLogLevel),
                m_pPanicHandler(0)
{
                s_pThis = this;
}

CLogger::~CLogger(void)
{
                if (s_pThis == this) s_pThis = 0;
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
                if (s_pThis == 0) new CLogger(LogPanic);

                return s_pThis;
}

void CLogger::SetRawSink(TLoggerRawSink* pSink, void* pContext)
{
                s_pSink        = pSink;
                s_pSinkContext = pContext;
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

                s_pSink(s_pSinkContext, pText, nLength);
}

void CLogger::WriteCString(const char* pText)
{
                if (pText == 0) return;

                WriteRaw(pText, strlen(pText));
}

void CLogger::FormatChar(char* buf, size_t size, size_t& pos, char c)
{
                if (size == 0) return;

                if (pos < size - 1) buf[pos] = c;

                pos++;
}

void CLogger::FormatString(char* buf, size_t size, size_t& pos, const char* s)
{
                if (s == 0) s = "(null)";

                while (*s)
                    {
                    FormatChar(buf, size, pos, *s++);
                    }
}

void CLogger::FormatUIntDec(char* buf, size_t size, size_t& pos, unsigned value)
{
                char tmp[10];
                unsigned n = 0;

                if (value == 0)
                    {
                    FormatChar(buf, size, pos, '0');
                    return;
                    }

                while (value != 0 && n < sizeof(tmp))
                    {
                    tmp[n++] = '0' + (value % 10);
                    value /= 10;
                    }

                while (n > 0)
                    {
                    FormatChar(buf, size, pos, tmp[--n]);
                    }
}

void CLogger::FormatIntDec(char* buf, size_t size, size_t& pos, int value)
{
                if (value < 0)
                    {
                    FormatChar(buf, size, pos, '-');
                    FormatUIntDec(buf, size, pos, (unsigned)(-value));
                    }
                else
                    {
                    FormatUIntDec(buf, size, pos, (unsigned)value);
                    }
}

void CLogger::FormatUIntHex(char* buf, size_t size, size_t& pos, unsigned value, boolean upper)
{
                const char* digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";

                char tmp[8];
                unsigned n = 0;

                if (value == 0)
                    {
                    FormatChar(buf, size, pos, '0');
                    return;
                    }

                while (value != 0 && n < sizeof(tmp))
                    {
                    tmp[n++] = digits[value & 0xF];
                    value >>= 4;
                    }

                while (n > 0)
                    {
                    FormatChar(buf, size, pos, tmp[--n]);
                    }
}

void CLogger::FormatULongDec(char* buf, size_t size, size_t& pos, unsigned long value)
{
                char tmp[20];
                unsigned n = 0;

                if (value == 0)
                    {
                    FormatChar(buf, size, pos, '0');
                    return;
                    }

                while (value != 0 && n < sizeof(tmp))
                    {
                    tmp[n++] = '0' + (value % 10);
                    value /= 10;
                    }

                while (n > 0)
                    {
                    FormatChar(buf, size, pos, tmp[--n]);
                    }
}

int CLogger::FormatV(char* buf, size_t size, const char* fmt, va_list args)
{
                size_t pos = 0;

                if (fmt == 0)
                    {
                    if (size > 0) buf[0] = '\0';
                    return 0;
                    }

                while (*fmt)
                    {
                    if (*fmt != '%')
                        {
                        FormatChar(buf, size, pos, *fmt++);
                        continue;
                        }

                    fmt++;

                    switch (*fmt)
                        {
                        case '%':
                            FormatChar(buf, size, pos, '%');
                            break;

                        case 'u':
                            FormatUIntDec(buf, size, pos, va_arg(args, unsigned));
                            break;

                        case 'd':
                        case 'i':
                            FormatIntDec(buf, size, pos, va_arg(args, int));
                            break;

                        case 'x':
                            FormatUIntHex(buf, size, pos, va_arg(args, unsigned), FALSE);
                            break;

                        case 'X':
                            FormatUIntHex(buf, size, pos, va_arg(args, unsigned), TRUE);
                            break;

                        case 's':
                            FormatString(buf, size, pos, va_arg(args, const char*));
                            break;

                        case 'c':
                            FormatChar(buf, size, pos, (char)va_arg(args, int));
                            break;

                        case 'l':
                            fmt++;

                            if (*fmt == 'u')
                                {
                                FormatULongDec(buf, size, pos, va_arg(args, unsigned long));
                                }
                            else
                                {
                                FormatChar(buf, size, pos, '%');
                                FormatChar(buf, size, pos, 'l');

                                if (*fmt != '\0')
                                    {
                                    FormatChar(buf, size, pos, *fmt);
                                    }
                                else
                                    {
                                    fmt--;
                                    }
                                }
                            break;
                                                        
                        case '\0':
                            fmt--;
                            break;

                        default:
                            FormatChar(buf, size, pos, '%');
                            FormatChar(buf, size, pos, *fmt);
                            break;
                        }

                    fmt++;
                    }

                if (size > 0)
                    {
                    if (pos < size)
                        {
                        buf[pos] = '\0';
                        }
                    else
                        {
                        buf[size - 1] = '\0';
                        }
                    }

                return (int)pos;
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

void CLogger::WriteV(const char* pSource,
                     TLogSeverity Severity,
                     const char* pMessage,
                     va_list Args)
{
            //  (void) Args;

                if (Severity > m_nLogLevel) return;

                char Message[LOG_MAX_MESSAGE];

            //  char Message[256];

            //  vsnprintf(Message, sizeof Message, pMessage, Args);
            //  Message[sizeof Message - 1] = '\0';

                if (pMessage == 0) return;  // new

                FormatV(Message, sizeof Message, pMessage, Args);

                WriteCString(pSource);
                WriteCString(": ");
                WriteCString(Message); // prints without vsprintf conversion
                WriteCString("\n");

                if (Severity == LogPanic && m_pPanicHandler != 0) (*m_pPanicHandler)();
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

                if (Severity == LogPanic && m_pPanicHandler != 0) (*m_pPanicHandler)();
}