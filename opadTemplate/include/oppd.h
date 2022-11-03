
#ifndef OPPD_H
#define OPPD_H

#include "json.hpp"
using json = nlohmann::json;

#include <atlbase.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#include <WinUser.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include "bass.h"

void consoleGotoxy(int x, int y);
void clearScreen();
void setUnicodeConsole();

void day01(int argc, char** argv);
void day02();
void day03();

#endif
