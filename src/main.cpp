#include <cstddef>
#include <cstdlib>
#include <string>
#include <map>
#include "TreeMap.h"
#include "/mnt/c/Users/parve/Desktop/MAPY/src/TreeMap.h"
#include "/mnt/c/Users/parve/Desktop/MAPY/src/HashMap.h"

using namespace std;


namespace
{

template <typename K, typename V>
using Map = aisdi::TreeMap<K, V>;

void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}

} // namespace
 
int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
