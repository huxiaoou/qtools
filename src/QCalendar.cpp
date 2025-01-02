#include "QCalendar.h"

namespace QUtility
{
    QSection::QSection(const char *this_date, const char *prev_date, const char type)
    {
        const char *format = "%Y%m%d %H:%M:%S";
        strcpy(_trade_date, this_date);
        _type = type;
        if (_type == 'D')
        {
            char _end_time[] = "YYYYMMDD 16:00:00";
            strncpy(_end_time, this_date, 8);
            strptime(_end_time, format, _end.getTm());
            _end.SyncTsFrmTm();
            _bgn.setTs(_end.getTs() - 60 * 60 * 8);
            _bgn.SyncTmFrmTs();
            strcpy(_sec_lbl_ngt_0, "");
            strcpy(_sec_lbl_ngt_1, "");
            _end.print_date(_sec_lbl_day, NULL);
        }
        else if (_type == 'N')
        {
            char _bgn_time[] = "YYYYMMDD 20:00:00";
            strncpy(_bgn_time, prev_date, 8);
            strptime(_bgn_time, format, _bgn.getTm());
            _bgn.SyncTsFrmTm();
            _end.setTs(_bgn.getTs() + 60 * 60 * 8);
            _end.SyncTmFrmTs();
            _bgn.print_date(_sec_lbl_ngt_0, NULL);
            _end.print_date(_sec_lbl_ngt_1, NULL);
            strcpy(_sec_lbl_day, "");
        }
        else
        {
            std::cout << "Invalid type = '" << _type << "', please check again" << std::endl;
            exit(-1);
        }
    }

    void QSection::Display() const
    {
        char _bgn_string[32];
        char _end_string[32];
        _bgn.print_datetime(_bgn_string, NULL);
        _end.print_datetime(_end_string, NULL);
        std::cout << "QSection(\n"
                  << "  bgnTS=" << _bgn.getTs() << ",\n"
                  << "  endTS=" << _end.getTs() << ",\n"
                  << "  bgn='" << _bgn_string << "',\n"
                  << "  end='" << _end_string << "',\n"
                  << "  type='" << _type << "',\n"
                  << "  trade_date='" << _trade_date << "',\n"
                  << "  sec_lbl_ngt_0='" << _sec_lbl_ngt_0 << "',\n"
                  << "  sec_lbl_ngt_1='" << _sec_lbl_ngt_1 << "',\n"
                  << "  sec_lbl_day='" << _sec_lbl_day << "',\n"
                  << ")" << std::endl;
    }

    void test_section()
    {
        char prev_date[] = "20240101";
        char this_date[] = "20240103";
        QSection *sd = new QSection(this_date, prev_date, 'D');
        sd->Display();

        std::cout << "\n";
        QSection *sn = new QSection(this_date, prev_date, 'N');
        sn->Display();

        std::cout << "\n";
        QSection *se = new QSection(this_date, prev_date, 'e');
        se->Display();
    }
}
