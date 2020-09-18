#ifndef COMMON_H
#define COMMON_H

// Common used headers
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <fstream>

// Whatever this stuff is O.o
constexpr char* HTTP_VERSION    = (char*)"HTTP/1.1";
constexpr char* SERVER_STRING   = (char*)"Server: WebXO/1.2.1";

constexpr char* ERROR_PAGE_DIR   = (char*)"/var/www/ErrorPages";

// Typedefs to save time
typedef std::pair<char*, int> CompBuffer;

#endif //COMMON_H