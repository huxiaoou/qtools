#include <time.h>
#include <iostream>
#include <ctime>
#include <iomanip>

int main()
{
    std::time_t current_time;
    time(&current_time);
    std::cout << "current_time in 'time_t': " << current_time << std::endl;
    std::tm *tm0 = localtime(&current_time);
    std::cout << "current time in 'tm'    : " << std::put_time(tm0, "%Y-%m-%d %H:%M:%S") << std::endl;

    const char *datetime_str = "2023-10-05 14:30:00";
    const char *format = "%Y-%m-%d %H:%M:%S";
    std::tm tm1;
 
    // 使用 strptime 解析日期时间字符串
    char *rest = strptime(datetime_str, format, &tm1);
    printf("Parsed date and time: %d-%02d-%02d %02d:%02d:%02d\n",
           tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
           tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
};
