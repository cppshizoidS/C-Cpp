#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct datetime
{
    int hour;
    int minute;
    int second;

    int day;
    int month;
    int year;

    void Output();
    void System_time(datetime& system);
    void Get_next_date(datetime& next);
    void Get_prev_date(datetime& prev);
};

void datetime::Output()
{
    printf("%i:%i:%i %i/%i/%i", hour, minute, second, day, month, year);
}

void datetime::System_time(datetime& system)
{
    time_t rawtime;
    tm timeinfo;
    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);
    system.hour = timeinfo.tm_hour;
    system.minute = timeinfo.tm_min;
    system.second = timeinfo.tm_sec;
    system.day = timeinfo.tm_mday;
    system.month = timeinfo.tm_mon + 1;
    system.year = timeinfo.tm_year + 1900;
}

time_t Get_unix_time(datetime A)
{
    tm timeinfo;
    timeinfo.tm_year = A.year - 1900;
    timeinfo.tm_mon = A.month - 1;
    timeinfo.tm_mday = A.day;
    timeinfo.tm_hour = A.hour;
    timeinfo.tm_min = A.minute;
    timeinfo.tm_sec = A.second;
    time_t unix_time = mktime(&timeinfo);
    return unix_time;
}

void Get_normal_time(time_t unix_time, datetime& A)
{
    tm timeinfo;
    localtime_s(&timeinfo, &unix_time);
    A.hour = timeinfo.tm_hour;
    A.minute = timeinfo.tm_min;
    A.second = timeinfo.tm_sec;
    A.day = timeinfo.tm_mday;
    A.month = timeinfo.tm_mon + 1;
    A.year = timeinfo.tm_year + 1900;
}

void datetime::Get_next_date(datetime& next)
{
    int min = 2419200 * 2;
    int max = 31536000;
    Get_normal_time(Get_unix_time(*this) + (min + rand() % (max - min + 1)), next);
}

void datetime::Get_prev_date(datetime& prev)
{
    int min = 2419200 * 2;
    int max = 31536000;
    Get_normal_time(Get_unix_time(*this) - (min + rand() % (max - min + 1)), prev);
}

datetime* Read_file_date(string file_name, int& n)
{
    ifstream file(file_name);
    string line;
    n = 0;
    while (getline(file, line))
    {
        n++;
    }
    datetime* Date = new datetime[n];
    int i = 0;
    file.clear();
    file.seekg(0, ios::beg);
    while (getline(file, line))
    {
        istringstream line_(line);
        line_ >> Date[i].hour >> Date[i].minute >> Date[i].second >> Date[i].day >> Date[i].month >> Date[i].year;
        i++;
    }
    file.close();
    return Date;
}

int main()
{
    srand(time(0));
    datetime date, next, prev;
    int n;
    datetime* Date = Read_file_date("data.txt", n);
    datetime* Prev = new datetime[n];
    date.System_time(date); printf("\n\nSystem time: "); date.Output();
    printf("\n\n\tData read from file:\n\n");
    for (int i = 0; i < n; i++)
    {
        Date[i].Get_next_date(next);
        Date[i].Get_prev_date(prev);
        printf("%i)\t", i + 1);
        Date[i].Output(); 
        printf("\t");
        next.Output();
        printf("\t");
        prev.Output();
        printf("\n");
        Prev[i] = prev;
    }
    printf("\n\tVariant 8:\n\n");
    for (int i = 0; i < n; i++)
    {
        if (Date[i].year == Prev[i].year)
        {
            printf("%i)\t", i + 1);
            Date[i].Output(); 
            printf("\t");
            prev.Output();
            printf("\n");
        }
    }
    delete[] Date;
    delete[] Prev;
    return 0;
}
