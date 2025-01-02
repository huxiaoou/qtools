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
            _bgn = new QTimePoint(_end->getTsVal() - 60 * 60 * 8);
            strcpy(_sec_lbl_ngt_0, "");
            strcpy(_sec_lbl_ngt_1, "");
            _end->print_date(_sec_lbl_day, NULL);
        }
        else if (_type == 'N')
        {
            char _bgn_time[] = "YYYYMMDD 20:00:00";
            strncpy(_bgn_time, prev_date, 8);
            _bgn = new QTimePoint(_bgn_time, NULL);
            _end = new QTimePoint(_bgn->getTsVal() + 60 * 60 * 8);
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
           << "  bgnTS=" << section.GetBgn()->getTsVal() << ",\n"
           << "  endTS=" << section.GetEnd()->getTsVal() << ",\n"
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

    bool match_trade_date(
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
                        return true;
                }
            }
            return false;
        }
    }

    void test_section()
    {
        char prev_date[] = "20241231";
        char this_date[] = "20250102";
        QSection *sd = new QSection(this_date, prev_date, 'D');
        QSection *sn = new QSection(this_date, prev_date, 'N');
        std::cout << *sd << "\n" << *sn;
    }

    void test_calendar(const char *calendarPath)
    {
        const char *SEP = "-------------------------------------";
        char this_date[12] = "";
        char prev_date[12] = "";

        std::cout << SEP << std::endl;
        QTimePoint *now = new QTimePoint();
        if (match_trade_date(now->getTs(), this_date, prev_date, calendarPath))
        {
            std::cout << "Prev date = " << prev_date << std::endl;
            std::cout << "Now = " << *now << std::endl;
            std::cout << "This date = " << this_date << std::endl;
        }
        delete now;

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 14:59:00", NULL);
        if (match_trade_date(now->getTs(), this_date, prev_date, calendarPath))
        {
            std::cout << "Prev date = " << prev_date << std::endl;
            std::cout << "Now = " << *now << std::endl;
            std::cout << "This date = " << this_date << std::endl;
        }

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 15:30:00", NULL);
        if (match_trade_date(now->getTs(), this_date, prev_date, calendarPath))
        {
            std::cout << "Prev date = " << prev_date << std::endl;
            std::cout << "Now = " << *now << std::endl;
            std::cout << "This date = " << this_date << std::endl;
        }

        std::cout << SEP << std::endl;
        now = new QTimePoint("20241231 16:30:00", NULL);
        if (match_trade_date(now->getTs(), this_date, prev_date, calendarPath))
        {
            std::cout << "Prev date = " << prev_date << std::endl;
            std::cout << "Now = " << *now << std::endl;
            std::cout << "This date = " << this_date << std::endl;
        }

        std::cout << SEP << std::endl;
    }
}
