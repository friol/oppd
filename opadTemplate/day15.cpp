
//
// cal+ (or minus?)
// friol's oppd 2k22
//

#include "include/oppd.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXDAYPLACES 40

int monthFromString(std::string month)
{
    std::vector<std::string> monthArrShort = {
        "jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"
    };
    std::vector<std::string> monthArr = {
        "january","february","march","april","may","june","july","august","september","october","november","december"
    };

    for (int m = 0;m < monthArrShort.size();m++)
    {
        if (monthArrShort[m] == month)
        {
            return m;
        }
    }

    for (int m = 0;m < monthArr.size();m++)
    {
        if (monthArr[m] == month)
        {
            return m;
        }
    }

    return -1;
}

void buildMonthArray(int firstDayWday, int* marr, int curmonth,int year)
{
    for (int m = 0;m < MAXDAYPLACES;m++)
    {
        marr[m] = -1;
    }

    int numDaysPerMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int ndpm = numDaysPerMonth[curmonth];
    if (curmonth == 1)
    {
        if (year % 4 == 0) 
        {
            if (year % 100 == 0) 
            {
                if (year % 400 == 0) 
                {
                    ndpm++;
                }
            }
            else 
            {
                ndpm++;
            }
        }
    }

    int day = 1;
    for (int d = 0;d < ndpm+ firstDayWday;d++)
    {
        if (d < firstDayWday)
        {
            marr[d] = -1;
        }
        else
        {
            marr[d] = day;
            day++;
        }
    }
}

void printCalendarOfMonth(int curmonth, int year, int* marr, int thisDay)
{
    std::vector<std::string> monthArr = {
        "January","February","March","April","May","June","July","August","September","October","November","December"
    };

    std::string header = "Su Mo Tu We Th Fr Sa";
    std::string monthYear = monthArr[curmonth] + " " + std::to_string(year);
    int paddingAmount = (int)(header.size() - monthYear.size()) / 2;
    for (int c = 0;c < paddingAmount;c++)
    {
        std::cout << " ";
    }

    std::cout << monthYear << std::endl; // TODO center it
    std::cout << header << std::endl;

    int dw = 0;
    for (int p = 0;p < MAXDAYPLACES;p++)
    {
        if (marr[p] != -1)
        {
            if (marr[p] == thisDay)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
            }
            else if (((p%7)==0)||((p%7)==6))
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
            }
            std::cout << std::setw(2) << marr[p];

            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 7);

            std::cout << " ";
        }
        else
        {
            std::cout << "   ";
        }
        dw++;
        if (dw >= 7)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 7);
            std::cout << std::endl;
            dw = 0;
        }
    }

    std::cout << std::endl;
}

void day15usage(char** argv)
{
	std::cout << "Usage:" << std::endl << argv[0] << " [month year]" << std::endl;
}

void day15(int argc, char** argv)
{
    typedef std::chrono::system_clock Clock;

    auto now = Clock::now();
    std::time_t now_c = Clock::to_time_t(now);
    struct tm newtime;
    localtime_s(&newtime,&now_c);

    int curyear=1900+newtime.tm_year;
    int curmonth=newtime.tm_mon;
    int curday = newtime.tm_mday;

    bool isCurrentMonth = true;

    if (argc > 1)
    {
        if (argc != 3)
        {
            day15usage(argv);
            return;
        }

        std::string month = argv[1];
        std::string year = argv[2];

        std::transform(month.begin(), month.end(), month.begin(), [](unsigned char c) { return std::tolower(c); });

        int newYear = std::stoi(year);
        int newMonth = monthFromString(month);

        if (newMonth == -1)
        {
            std::cout << "Invalid month." << std::endl;
            return;
        }

        if ((newYear == curyear) && (newMonth == curmonth))
        {
            isCurrentMonth = true;
        }
        else
        {
            curyear = newYear;
            curmonth = newMonth;
            isCurrentMonth = false;
        }
    }

    // find day of week of 1st day

    std::tm time_in = { 0, 0, 0, 1, curmonth, curyear-1900 }; // 1-based day, 0-based month, year since 1900
    std::time_t time_temp = std::mktime(&time_in);
    struct tm firstDay;
    localtime_s(&firstDay, &time_temp);

    int theMonthArray[MAXDAYPLACES];
    buildMonthArray(firstDay.tm_wday, theMonthArray,curmonth,curyear);

    if (!isCurrentMonth)
    {
        printCalendarOfMonth(curmonth, curyear, theMonthArray, -1);
    }
    else
    {
        printCalendarOfMonth(curmonth, curyear, theMonthArray, curday);
    }
}
