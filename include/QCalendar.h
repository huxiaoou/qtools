#include <ctime>
#include <string.h>
#include <iostream>

namespace QUtility
{
    class QTimePoint
    {
    private:
        std::time_t _ts;
        std::tm _tm;

    public:
        std::tm *getTm() { return &_tm; }
        std::time_t getTs() const { return _ts; }
        void setTm(std::tm *tm) { _tm = *tm; }
        void setTs(std::time_t ts) { _ts = ts; }
        void SyncTsFrmTm() { _ts = mktime(&_tm); }
        void SyncTmFrmTs() { _tm = *localtime(&_ts); }
        void print_datetime(char *dest, const char *format) const
        {
            if (format == NULL)
                format = "%04d%02d%02d %02d:%02d:%02d";
            sprintf(dest, format,
                    _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
                    _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
        }
        void print_date(char *dest, const char *format) const
        {
            if (format == NULL)
                format = "%04d%02d%02d";
            sprintf(dest, format,
                    _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday);
        }
    };

    class QSection
    {
    private:
        QTimePoint _bgn;
        QTimePoint _end;
        char _type;
        char _trade_date[12];
        char _sec_lbl_ngt_0[12];
        char _sec_lbl_ngt_1[12];
        char _sec_lbl_day[12];

    public:
        QSection(const char *this_date, const char *prev_date, const char type);
        char *GetTradeDate() { return _trade_date; }
        char *GetSecLblNgt0() { return _sec_lbl_ngt_0; }
        char *GetSecLblNgt1() { return _sec_lbl_ngt_1; }
        char *GetSecLblDay() { return _sec_lbl_day; }
        void Display() const;
    };

    void test_section();
}