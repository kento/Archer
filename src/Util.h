#include <string.h>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

template <typename T> std::string NumberToString ( T Number )
{
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

template <typename T> T StringToNumber ( std::string str )
{
  std::istringstream ss(str);
  T val;
  ss >> val;
  return val;
}

void split(std::vector<std::string> *tokens, char *str, std::string split_value);
