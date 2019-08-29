#include "Logger.h"

int main() {
  Log::LoggingLevel() = Log::LevelFromString("DEBUG");
  Log::OnOffFile() = true;
  Log::LogPath() = "/tmp/lwtest.log";
  Log::OnOffStdout() = true;

  LOGGER(logDEBUG) << "Hello, world";
  LOGGER(logINFO) << "This line goes everywhere";

  Log::OnOffFile() = false;
  Log::OnOffStdout() = true;
  LOGGER(logWARNING) << "This line goes to stdout only";

  Log::OnOffFile() = true;
  Log::OnOffStdout() = false;
  LOGGER(logERROR) << "This line goes to file only";

  Log::OnOffFile() = false;
  Log::OnOffStdout() = false;
  LOGGER(logERROR) << "This line goes to nowhere";

  return 0;
}