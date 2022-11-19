
#ifndef OPPD_H
#define OPPD_H

#include "json.hpp"
using json = nlohmann::json;

#include <winsock2.h>
#include <Ws2tcpip.h>

#include <atlbase.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#include <WinUser.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include <cctype>
#include <locale>

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include "bass.h"

void consoleGotoxy(int x, int y);
void clearScreen();
void setUnicodeConsole();
void consolePrintReversedChar(char ch,int col);
void resetConsole();
COORD GetConsoleCursorPosition();
void consoleGetDimensions(int& dimx, int& dimy);
void consoleShowCursor();
void consoleHideCursor();
char consoleGetCharAt(int x, int y);

void day01(int argc, char** argv);
void day02();
void day03();
void day04(int argc, char** argv);
void day05();
void day06();
void day07();
void day08(int argc, char** argv);
void day09(int argc, char** argv);
void day09bis(int argc, char** argv);
void day10(int argc, char** argv);
void day11();
void day12();
void day13();
void day14(int argc,char** argv);
void day15(int argc,char** argv);
void day16(int argc,char** argv);
void day17();
void day18(int argc, char** argv);
void day19();

#endif
