#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"

using namespace std;

#define MAX_JSON_FILE_SIZE 8192

int main(const int argc, const char* argv[]) {
  if (argc != 2) {
    cout << "Usage: json <json filepath>\n";
    return 0;
  }

  ifstream in(argv[1]);
  if (in.eof())
    return -2;

  char buffer[MAX_JSON_FILE_SIZE] = "";
  in.read(buffer, MAX_JSON_FILE_SIZE);
  rapidjson::StringStream ss(buffer);
  rapidjson::Document jsondoc;
  jsondoc.ParseStream(ss);
  if (jsondoc.HasParseError()) {
    cout << "Error at " << jsondoc.GetErrorOffset() << endl;
    cout << buffer + jsondoc.GetErrorOffset() << endl;
    return -3;
  }

  return 0;
}

