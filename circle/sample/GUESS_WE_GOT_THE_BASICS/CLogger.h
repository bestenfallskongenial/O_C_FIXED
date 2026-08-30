#ifndef _circle_logger_h
#define _circle_logger_h

#include <circle/types.h>
#include <stdarg.h>
#include <stddef.h>

class CDevice;
class CTimer;

#define LOG_MAX_SOURCE      20
#define LOG_MAX_MESSAGE     200

enum TLogSeverity
{
                LogPanic = 0,
                LogError,
                LogWarning,
                LogNotice,
                LogDebug
};

typedef void TLogPanicHandler(void);

typedef void TLoggerRawSink(void* pContext,
                            const char* pText,
                            unsigned nLength);

class CLogger
{
public:
                CLogger                 (   unsigned    nLogLevel,
                                            CTimer*     pTimer = 0,
                                            boolean     bOverwriteOldest = TRUE );

                ~CLogger                (   void );

                boolean Initialize      (   CDevice*        pTarget );

                void    SetNewTarget    (   CDevice*        pTarget );

                static  CLogger* Get    (   void );

                void    Write           (   const char*     pSource,
                                            TLogSeverity    Severity,
                                            const char*     pMessage,
                                            ... );

                void    WriteV          (   const char*     pSource,
                                            TLogSeverity    Severity,
                                            const char*     pMessage,
                                            va_list         Args );

                void    WriteNoAlloc    (   const char*     pSource,
                                            TLogSeverity    Severity,
                                            const char*     pMessage );

                void    RegisterPanicHandler
                                        (   TLogPanicHandler* pHandler );

                static  void SetRawSink (   TLoggerRawSink* pSink,
                                            void*           pContext );

private:
                void    WriteRaw        (   const char*     pText,
                                            unsigned        nLength );

                void    WriteCString    (   const char*     pText );

                int     FormatV         (   char*           buf,
                                            size_t          size,
                                            const char*     fmt,
                                            va_list         args );

                void    FormatChar      (   char*           buf,
                                            size_t          size,
                                            size_t&         pos,
                                            char            c );

                void    FormatString    (   char*           buf,
                                            size_t          size,
                                            size_t&         pos,
                                            const char*     s );

                void    FormatUIntDec   (   char*           buf,
                                            size_t          size,
                                            size_t&         pos,
                                            unsigned        value );

                void    FormatIntDec    (   char*           buf,
                                            size_t          size,
                                            size_t&         pos,
                                            int             value );

                void    FormatUIntHex   (   char*           buf,
                                            size_t          size,
                                            size_t&         pos,
                                            unsigned        value,
                                            boolean         upper );
                void    FormatULongDec  (   char*           buf, 
                                            size_t          size, 
                                            size_t&         pos, 
                                            unsigned long   value);

private:
                static CLogger*         s_pThis;
                static TLoggerRawSink*  s_pSink;
                static void*            s_pSinkContext;

                unsigned                m_nLogLevel;
                TLogPanicHandler*       m_pPanicHandler;
};

#define LOGPANIC(...)        CLogger::Get()->Write(From, LogPanic,   __VA_ARGS__)
#define LOGERR(...)          CLogger::Get()->Write(From, LogError,   __VA_ARGS__)
#define LOGWARN(...)         CLogger::Get()->Write(From, LogWarning, __VA_ARGS__)
#define LOGNOTE(...)         CLogger::Get()->Write(From, LogNotice,  __VA_ARGS__)
#define LOGDBG(...)          CLogger::Get()->Write(From, LogDebug,   __VA_ARGS__)

#endif