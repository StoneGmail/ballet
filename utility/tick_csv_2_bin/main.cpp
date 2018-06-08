#include "utility_global.h"
#include "tick_bin.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cout << "Usage tc2b <csv data filepath> <bin data filepath> <cdi filepath>\n";
    exit(0);
  }

  int ret = tick_csv_to_bin(argv[1], argv[2]);
  if (ret) {
    printf("Error when tick csv to bin\n");
  } else {
    ret = gen_cdi(argv[2], argv[3]);
  }

  return ret;
}
