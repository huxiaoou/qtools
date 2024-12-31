#include "QCalendar.h"

int main()
{
    char prev_date[] = "20240101";
    char this_date[] = "20240103";
    QUtility::QSection *sd = new QUtility::QSection(this_date, prev_date, 'D');
    sd->Display();

    std::cout << "\n";

    QUtility::QSection *sn = new QUtility::QSection(this_date, prev_date, 'N');
    sn->Display();

    QUtility::QSection *se = new QUtility::QSection(this_date, prev_date, 'e');
    se->Display();
};
