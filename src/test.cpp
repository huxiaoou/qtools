#include "QCalendar.h"

int main()
{
    const char * CALENDAR_PATH = "/mnt/data/trade/calendar/cne_calendar.csv";
    QUtility::test_section();
    QUtility::test_calendar(CALENDAR_PATH);
};
