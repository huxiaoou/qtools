#include "QCalendar.h"

namespace QUtility
{
    QTimePoint::QTimePoint()
    {
        time(&_ts);
        SyncTmFrmTs();
    }

    QTimePoint::QTimePoint(const time_t ts)
    {
        _ts = ts;
        SyncTmFrmTs();
    }

    QTimePoint::QTimePoint(const char *datetime, const char *format)
    {
        memset(&_tm, 0, sizeof(_tm));
        if (format == NULL)
            format = "%Y%m%d %H:%M:%S";
        strptime(datetime, format, &_tm);
        SyncTsFrmTm();
    }

    std::ostream &operator<<(std::ostream &os, const QTimePoint &timepoint)
    {
        char output[32];
        timepoint.print_datetime(output, NULL);
        os << output;
        return os;
    }

    QSection::QSection(const char *this_date, const char *prev_date, const char type)
    {
        strcpy(_trade_date, this_date);
        _type = type;
        if (_type == 'D')
        {
            char _end_time[] = "YYYYMMDD 16:00:00";
            strncpy(_end_time, this_date, 8);
            _end = new QTimePoint(_end_time, NULL);
            _bgn = new QTimePoint(*_end->getTs() - 60 * 60 * 8);
            strcpy(_sec_lbl_ngt_0, "");
            strcpy(_sec_lbl_ngt_1, "");
            _end->print_date(_sec_lbl_day, NULL);
        }
        else if (_type == 'N')
        {
            char _bgn_time[] = "YYYYMMDD 20:00:00";
            strncpy(_bgn_time, prev_date, 8);
            _bgn = new QTimePoint(_bgn_time, NULL);
            _end = new QTimePoint(*_bgn->getTs() + 60 * 60 * 8);
            _bgn->print_date(_sec_lbl_ngt_0, NULL);
            _end->print_date(_sec_lbl_ngt_1, NULL);
            strcpy(_sec_lbl_day, "");
        }
        else
        {
            std::cout << "Invalid type = '" << _type << "', please check again" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    QSection::~QSection()
    {
        delete _bgn;
        delete _end;
    }

    std::ostream &operator<<(std::ostream &os, const QSection &section)
    {
        os << "QSection(\n"
           << "  bgnTS=" << *section.GetBgn()->getTs() << ",\n"
           << "  endTS=" << *section.GetEnd()->getTs() << ",\n"
           << "  bgn='" << *section.GetBgn() << "',\n"
           << "  end='" << *section.GetEnd() << "',\n"
           << "  type='" << section.GetType() << "',\n"
           << "  trade_date='" << section.GetTradeDate() << "',\n"
           << "  sec_lbl_ngt_0='" << section.GetSecLblNgt0() << "',\n"
           << "  sec_lbl_ngt_1='" << section.GetSecLblNgt1() << "',\n"
           << "  sec_lbl_day='" << section.GetSecLblDay() << "',\n"
           << ")" << std::endl;
        return os;
    }

    void match_trade_date(
        const time_t *test_datetime,
        char *this_date, char *prev_date,
        const char *calendarPath)
    {
        FILE *file = fopen(calendarPath, "r");
        if (!file)
        {
            perror("Unable to open file");
            exit(EXIT_FAILURE);
        }
        else
        {
            char line[60];
            char trade_date[12];
            const char *format = "%Y%m%d %H:%M:%S";
            char _end_time[] = "YYYYMMDD 16:00:00";
            tm _tm;
            time_t *this_datetime = new time_t;

            strcpy(prev_date, "");
            strcpy(this_date, "");
            while (fgets(line, sizeof(line), file))
            {
                if (sscanf(line, "%[0-9]\r\n", (char *)trade_date) == 1)
                {
                    strcpy(prev_date, this_date);
                    strcpy(this_date, trade_date);
                    strncpy(_end_time, this_date, 8);
                    strptime(_end_time, format, &_tm);
                    *this_datetime = mktime(&_tm);
                    if (*this_datetime >= *test_datetime)
                        return;
                }
            }
            exit(EXIT_FAILURE);
        }
    }

    void test_timepoint()
    {
        QTimePoint t1 = QUtility::QTimePoint("20250101 09:00:00", NULL);
        QTimePoint t2 = QUtility::QTimePoint("20250102 08:00:00", NULL);

        std::cout << "t1 = " << t1 << "\n"
                  << "t2 = " << t2 << std::endl;
        if (t1 < t2)
            std::cout << "t1 < t2" << std::endl;
        if (t1 <= t2)
            std::cout << "t1 <= t2" << std::endl;
        if (t2 > t1)
            std::cout << "t2 > t1" << std::endl;
        if (t2 >= t1)
            std::cout << "t2 >= t1" << std::endl;
    }

    void test_section()
    {
        char prev_date[] = "20241231";
        char this_date[] = "20250102";
        QSection *sn = new QSection(this_date, prev_date, 'N');
        QSection *sd = new QSection(this_date, prev_date, 'D');
        std::cout << *sn << "\n"
                  << *sd << std::endl;

        QTimePoint *tp = new QTimePoint("20250101 09:00:00", NULL);
        std::cout << "tp=" << *tp << std::endl;
        std::cout << "Night section has the tp? " << (sn->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        std::cout << "Day   section has the tp? " << (sd->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        delete tp;

        tp = new QTimePoint("20250102 09:00:00", NULL);
        std::cout << "tp=" << *tp << std::endl;
        std::cout << "Night section has the tp? " << (sn->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        std::cout << "Day   section has the tp? " << (sd->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        delete tp;
    }

    void test_calendar(const char *calendarPath)
    {
        const char *SEP = "-------------------------------------";
        char this_date[12] = "";
        char prev_date[12] = "";

        std::cout << SEP << std::endl;
        QTimePoint *now = new QTimePoint();
        match_trade_date(now->getTs(), this_date, prev_date, calendarPath);
        std::cout << "Now = " << *now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;
        delete now;

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 14:59:00", NULL);
        match_trade_date(now->getTs(), this_date, prev_date, calendarPath);
        std::cout << "Now = " << *now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;
        delete now;

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 15:30:00", NULL);
        match_trade_date(now->getTs(), this_date, prev_date, calendarPath);
        std::cout << "Now = " << *now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;
        delete now;

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 16:30:00", NULL);
        match_trade_date(now->getTs(), this_date, prev_date, calendarPath);
        std::cout << "Now = " << *now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;
        delete now;

        std::cout << SEP << std::endl;
        QTimePoint *t1 = new QTimePoint();
        QTimePoint *t2 = new QTimePoint();
        std::cout << "(t1 = " << *t1 << ")" << ((t1 > t2) ? " > " : " <= ") << "(t2 = " << *t2 << ")" << std::endl;
        delete t1;
        delete t2;
    }
}
