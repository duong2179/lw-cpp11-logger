#ifndef LOGGER_H_
#define LOGGER_H_

#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std::chrono;

#define MSECS_PER_SEC 1000

enum TLogLevel { logERROR, logWARNING, logINFO, logDEBUG };

class Log {
 public:
  Log();
  virtual ~Log();
  std::ostringstream& Get(const std::string& func_name,
                          TLogLevel level = logINFO);

 public:
  static TLogLevel& LogLevel();
  static bool& OnOffStdout();
  static bool& OnOffFile();
  static std::string& LogPath();
  static std::string LevelToString(TLogLevel level);
  static TLogLevel LevelFromString(const std::string& level);

  static int64_t CurrentEpochInMsec();
  static std::string TimeStringFromEpoch(int64_t ts64);

 private:
  Log(const Log&);
  Log& operator=(const Log&);
  FILE*& LogFilePtr();
  std::ostringstream os;

  static std::mutex mtx;
};

#ifndef FILE_LOG_MAX_LEVEL
#define FILE_LOG_MAX_LEVEL logDEBUG
#endif

#define LOGGER(level)               \
  if (level > FILE_LOG_MAX_LEVEL)    \
    ;                               \
  else if (level > Log::LogLevel()) \
    ;                               \
  else                              \
    Log().Get(__FUNCTION__, level)

#endif  // LOGGER_H_
