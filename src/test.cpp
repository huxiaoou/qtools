#include "QCalendar.h"
#include "QWidgets.h"

int main()
{
    const char *CALENDAR_PATH = "/mnt/data/trade/calendar/cne_calendar.csv";

    QUtility::test_timepoint();
    QUtility::test_section();
    QUtility::test_calendar(CALENDAR_PATH);
    QUtility::test_widgets();
};
