#include <iostream>
#include "utility_global.h"
#include "tick_bin.h"

#include "def.h"

using namespace std;

int main(int argc, char * argv[])
{
  if (argc != 3) {
    std::cout << "Usage savecdi <data path> <instrument>888\n";
    exit(0);
  }

  char binfile[MAX_PATH];
  char cdifile[MAX_PATH];
  snprintf(binfile, MAX_PATH, "%s/%s888_tick.bin", argv[1], argv[2]);
  snprintf(cdifile, MAX_PATH, "%s/%s888_tick.cdi", argv[1], argv[2]);

  printf("%s\n%s\n", binfile, cdifile);
  return gen_cdi(binfile, cdifile);
}
