#pragma once

#ifndef COMMON_H
#define COMMON_H

// Common used headers
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <fstream>
#include <map>

// Whatever this stuff is O.o
constexpr char* HTTP_VERSION        = (char*)"HTTP/1.1";
constexpr char* SERVER_STRING       = (char*)"Server: WebXO/1.6.5";
constexpr char* AUTHOR_STRING       = (char*)"MelissaXO<melissa@melissaxo.xyz>";

constexpr char* ERROR_PAGE_DIR      = (char*)"/var/www/ErrorPages";

constexpr int MAX_MSG_BUFFER        = 50;
constexpr int REQUEST_BUFFER        = 2048;

// Typedefs to save time
typedef std::pair<char*, int> CompBuffer;

#endif //COMMON_H
