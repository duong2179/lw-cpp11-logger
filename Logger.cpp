#include "Logger.h"
#include <cstring>
#include <ctime>

std::mutex Log::mtx;

Log::Log() {}

int64_t Log::CurrentEpochInMsec() {
  milliseconds msecs =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  return msecs.count();
}

std::string Log::TimeStringFromEpoch(int64_t ts64) {
  time_t secs = ts64 / MSECS_PER_SEC;
  int32_t msecs = ts64 % MSECS_PER_SEC;

  // TODO: localtime is NOT thread-safe
  struct tm timeinfo = *std::localtime(&secs);

  char str[32];
  memset(str, 0, 32);
  char buff[32];
  memset(buff, 0, 32);
  strftime(buff, 32, "%Y-%m-%d %H:%M:%S", &timeinfo);
  std::sprintf(str, "%s.%03u", buff, msecs);

  return str;
}

std::ostringstream& Log::Get(const std::string& func_name, TLogLevel level) {
  int64_t ts64 = Log::CurrentEpochInMsec();

  os << Log::TimeStringFromEpoch(ts64) << " " << Log::LevelToString(level)
     << " [" << func_name << "]: ";
  os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
  return os;
}

Log::~Log() {
  os << std::endl;

  // Write to output streams. Use lock to avoid interleaves
  std::lock_guard<std::mutex> lck(Log::mtx);

  if (OnOffStdout()) {
    FILE* fp = stdout;
    if (!fp) {
      return;
    }
    fprintf(fp, "%s", os.str().c_str());
    fflush(fp);
  }

  if (OnOffFile()) {
    FILE* fp = LogFilePtr();
    if (!fp) {
      return;
    }
    fprintf(fp, "%s", os.str().c_str());
    fflush(fp);
  }
}

TLogLevel& Log::LogLevel() {
  static TLogLevel log_level = logDEBUG;
  return log_level;
}

bool& Log::OnOffStdout() {
  static bool on_off_stdout = true;
  return on_off_stdout;
}

bool& Log::OnOffFile() {
  static bool on_off_file = true;
  return on_off_file;
}

std::string& Log::LogPath() {
  static std::string log_path;
  return log_path;
}

FILE*& Log::LogFilePtr() {
  static FILE* fp = nullptr;
  if (fp == nullptr && !Log::LogPath().empty()) {
    fp = fopen(Log::LogPath().c_str(), "a");
  }
  return fp;
}

std::string Log::LevelToString(TLogLevel level) {
  static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
  return buffer[level];
}

TLogLevel Log::LevelFromString(const std::string& level) {
  if (level == "DEBUG" || level == "debug")
    return logDEBUG;
  if (level == "INFO" || level == "info")
    return logINFO;
  if (level == "WARNING" || level == "warning")
    return logWARNING;
  if (level == "ERROR" || level == "error")
    return logERROR;
  Log().Get(__FUNCTION__, logWARNING) << "Unknown logging level '" << level
                                      << "'. Using INFO level as default.";
  return logINFO;
}
