
#include <util/Socket.hxx>

#include <cassert>
#include <iostream>
#include <stdio.h>
#include <util/Data.hxx>

#ifndef WIN32
#include <sys/time.h>

#include <sys/types.h>
#include <time.h>

#include <util/Log.hxx>
#include <util/Lock.hxx>

using namespace Vocal2;
using namespace std;

Log::Level Log::_level = Log::DEBUG;
Log::Type Log::_type = COUT;
Data Log::_appName;
Data Log::_hostname;


const char
Log::_descriptions[][32] = {"EMERG", "ALERT", "CRIT", "ERR", "WARNING", "NOTICE", "INFO", "DEBUG", "DEBUG_STACK", ""}; 

Mutex Log::_mutex;

void 
Log::initialize(Type type, Level level, const Data& appName)
{
   _type = type;
   _level = level;
   _appName = appName.substr(appName.find_last_of("/")+1);
   
 
#ifdef WIN32
   _hostname = buffer;
   _pid = getpid();

}

void
Log::setLevel(Level level)
{
   Lock lock(_mutex);
   _level = level; 
}

Data
Log::toString(Level l)
{
   return Data("LOG_") + _descriptions[l];
}

Log::Level
Log::toLevel(const Data& l)
{
   Data pri = l;
   if (pri.find("LOG_", 0) == 0)
   {
      pri.erase(0, 4);
   }
   
   int i=0;
   while (Data(_descriptions[i]).length())
   {
      if (pri == Data(_descriptions[i])) 
      {
         return Level(i);
      }
      i++;
   }

   cerr << "Choosing Debug level since string was not understood: " << l << endl;
   return Log::DEBUG;
}


ostream&
Log::tags(Log::Level level, const Subsystem& subsystem, ostream& strm) 
{
#ifdef WIN32
	strm << _descriptions[level] << DELIM
        << timestamp() << DELIM
        << subsystem DELIM ;
#else
        << timestamp() << DELIM  
        << _hostname << DELIM  
        << _appName << DELIM
        << subsystem << DELIM 
        << _pid << DELIM
}

Data
Log::timestamp() 
{
   const unsigned int DATEBUF_SIZE=256;
   char datebuf[DATEBUF_SIZE];
   
#ifdef WIN32
   if (result == -1)
   {
      /* If we can't get the time of day, don't print a timestamp.
        Under Unix, this will never happen:  gettimeofday can fail only
        if the timezone is invalid which it can't be, since it is
        uninitialized]or if tv or tz are invalid pointers. */
        datebuf [0] = 0;
    }
    else
    {
       /* The tv_sec field represents the number of seconds passed since
          the Epoch, which is exactly the argument gettimeofday needs. */
       const time_t timeInSeconds = (time_t) tv.tv_sec;
       strftime (datebuf,
                 DATEBUF_SIZE,
                 "%Y%m%d-%H%M%S", /* guaranteed to fit in 256 chars,
                                     hence don't check return code */
                 localtime (&timeInSeconds));
    }
   
   char msbuf[5];
   /* Dividing (without remainder) by 1000 rounds the microseconds
      measure to the nearest millisecond. */
   sprintf(msbuf, ".%3.3ld", (tv.tv_usec / 1000));
   
   int datebufCharsRemaining = DATEBUF_SIZE - strlen (datebuf);
   strncat (datebuf, msbuf, datebufCharsRemaining - 1);
   datebuf[DATEBUF_SIZE - 1] = '\0'; /* Just in case strncat truncated msbuf,
                                        thereby leaving its last character at
                                        the end, instead of a null terminator */

   return datebuf;
}
   