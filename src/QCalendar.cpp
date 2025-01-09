#include "QCalendar.h"

namespace QUtility
{
    QTimestamp::QTimestamp()
    {
        _ts = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    QTimestamp::QTimestamp(const int64_t ts)
    {
        _ts = ts;
    }

    QTimestamp::QTimestamp(const char *datetime, const char *format)
    {
        std::tm *_tm = new ::std::tm;
        std::memset(_tm, 0, sizeof(_tm));
        if (format == NULL)
            format = "%Y%m%d %H:%M:%S";
        char *endptr = strptime(datetime, format, _tm);
        if (*endptr == '.')
            endptr++; // Skip the '.'
        SyncTpFrmTm(_tm, unsigned(atoi(endptr)));
    }

    void QTimestamp::reSync()
    {
        _ts = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    void QTimestamp::SyncTpFrmTm(std::tm *tm, unsigned int millisec)
    {
        _ts = std::mktime(tm) * 1000 + millisec;
    }

    const std::tm *QTimestamp::GetTm() const
    {
        std::time_t t = Seconds();
        std::tm *_tm = std::localtime(&t);
        return _tm;
    }

    void QTimestamp::PrintDate(char *dest, const char *format) const
    {
        const std::tm *_tm = GetTm();
        if (format == NULL)
            format = "%04d%02d%02d";
        sprintf(dest, format, _tm->tm_year + 1900, _tm->tm_mon + 1, _tm->tm_mday);
    }

    void QTimestamp::PrintDateTime(char *dest, const char *format) const
    {
        const std::tm *_tm = GetTm();
        if (format == NULL)
            format = "%04d%02d%02d %02d:%02d:%02d.%03d";
        sprintf(dest, format,
                _tm->tm_year + 1900, _tm->tm_mon + 1, _tm->tm_mday,
                _tm->tm_hour, _tm->tm_min, _tm->tm_sec,
                MilliSeconds());
    }

    std::ostream &operator<<(std::ostream &os, const QTimestamp &tp)
    {
        char output[32];
        tp.PrintDateTime(output, NULL);
        os << output;
        return os;
    }

    QSection::QSection(const char *this_date, const char *prev_date, const char type)
    {
        std::strcpy(_trade_date, this_date);
        _type = type;
        if (_type == 'D')
        {
            char _end_time[] = "YYYYMMDD 16:00:00.000";
            std::strncpy(_end_time, this_date, 8);
            _end = new QTimestamp(_end_time, NULL);
            _bgn = _end->shift_hours(-8);
            std::strcpy(_sec_lbl_ngt_0, "");
            std::strcpy(_sec_lbl_ngt_1, "");
            _end->PrintDate(_sec_lbl_day, NULL);
        }
        else if (_type == 'N')
        {
            char _bgn_time[] = "YYYYMMDD 20:00:00.000";
            std::strncpy(_bgn_time, prev_date, 8);
            _bgn = new QTimestamp(_bgn_time, NULL);
            _end = _bgn->shift_hours(8);
            _bgn->PrintDate(_sec_lbl_ngt_0, NULL);
            _end->PrintDate(_sec_lbl_ngt_1, NULL);
            std::strcpy(_sec_lbl_day, "");
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
           << "  bgnTS=" << section.GetBgn()->getTs() << ",\n"
           << "  endTS=" << section.GetEnd()->getTs() << ",\n"
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
        const QTimestamp &test_timestamp,
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
            char _end_time[] = "YYYYMMDD 16:00:00.000";

            std::strcpy(prev_date, "");
            std::strcpy(this_date, "");
            while (fgets(line, sizeof(line), file))
            {
                if (std::sscanf(line, "%[0-9]\r\n", (char *)trade_date) == 1)
                {
                    std::strcpy(prev_date, this_date);
                    std::strcpy(this_date, trade_date);
                    std::strncpy(_end_time, this_date, 8);
                    QTimestamp this_timestamp(_end_time, NULL);
                    if (this_timestamp >= test_timestamp)
                        return;
                }
            }
            exit(EXIT_FAILURE);
        }
    }

    void test_timepoint()
    {
        QTimestamp tp0;
        QTimestamp tp1(1);
        QTimestamp tp2("20240512 09:00:00.123", NULL);
        QTimestamp tp3 = tp2 + 2000;
        QTimestamp tp4 = tp3 - 5124;
        std::cout << tp0 << std::endl;
        std::cout << tp1 << std::endl;
        std::cout << tp2 << std::endl;
        std::cout << tp3 << std::endl;
        std::cout << tp4 << std::endl;

        QTimestamp t1 = tp2;
        QTimestamp t2 = tp3;
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

        QTimestamp *tp = new QTimestamp("20250101 09:00:00.000", NULL);
        std::cout << "tp=" << *tp << std::endl;
        std::cout << "Night section has the tp? " << (sn->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        std::cout << "Day   section has the tp? " << (sd->hasTimepoint(tp) ? 'Y' : 'N') << std::endl;
        delete tp;

        tp = new QTimestamp("20250102 09:00:00.000", NULL);
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
        QTimestamp now;
        match_trade_date(now, this_date, prev_date, calendarPath);
        std::cout << "Now = " << now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;

        std::cout << SEP << std::endl;
        now = QTimestamp("20241231 14:59:00.000", NULL);
        match_trade_date(now, this_date, prev_date, calendarPath);
        std::cout << "Now = " << now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;

        std::cout << SEP << std::endl;
        now = QTimestamp("20241231 15:30:00.000", NULL);
        match_trade_date(now, this_date, prev_date, calendarPath);
        std::cout << "Now = " << now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;

        std::cout << SEP << std::endl;
        now = QTimestamp("20241231 16:30:00.000", NULL);
        match_trade_date(now, this_date, prev_date, calendarPath);
        std::cout << "Now = " << now << std::endl;
        std::cout << "Prev date = " << prev_date << std::endl;
        std::cout << "This date = " << this_date << std::endl;

        std::cout << SEP << std::endl;
        QTimestamp *t1 = new QTimestamp();
        QTimestamp *t2 = new QTimestamp();
        std::cout << "(t1 = " << *t1 << ")" << ((t1 > t2) ? " > " : " <= ") << "(t2 = " << *t2 << ")" << std::endl;

        unsigned repeat_times = 100000;
        for (unsigned i = 0; i < repeat_times; i++)
            t1->reSync();
        std::cout << "After " << repeat_times << " times of resync, t1 = " << *t1 << std::endl;
        delete t1;
        delete t2;
    }
}
