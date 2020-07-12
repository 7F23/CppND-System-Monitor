#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH, MM, SS;
  HH = seconds / 3600;
  MM = (seconds % 3600) / 60;
  SS = seconds % 60;
  string Hour = std::to_string(HH);
  string Minute = std::to_string(MM);
  string Second = std::to_string(SS);
  if (Hour.length() == 1) {
    Hour = "0" + Hour;
  }
  if (Minute.length() == 1) {
    Minute = "0" + Minute;
  }
  if (Second.length() == 1) {
    Second = "0" + Second;
  }
  string Utime = Hour + ":" + Minute + ":" + Second;
  return Utime;
}