#pragma once

#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

namespace QUtility
{
    class QTimePoint
    {
    private:
        time_t _ts;
        tm _tm;

    public:
        QTimePoint();
        QTimePoint(const time_t ts);
        QTimePoint(const char *datetime, const char *format);
        tm *getTm() { return &_tm; }
        time_t *getTs() { return &_ts; }
        time_t getTsVal() const { return _ts; }
        void setTm(tm *tm) { _tm = *tm; }
        void setTs(time_t ts) { _ts = ts; }
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

    std::ostream &operator<<(std::ostream &os, const QTimePoint &timepoint);

    class QSection
    {
    private:
        QTimePoint *_bgn;
        QTimePoint *_end;
        char _type;
        char _trade_date[12];
        char _sec_lbl_ngt_0[12];
        char _sec_lbl_ngt_1[12];
        char _sec_lbl_day[12];

    public:
        QSection(const char *this_date, const char *prev_date, const char type);
        ~QSection();
        const char *GetTradeDate() const { return _trade_date; }
        const char *GetSecLblNgt0() const { return _sec_lbl_ngt_0; }
        const char *GetSecLblNgt1() const { return _sec_lbl_ngt_1; }
        const char *GetSecLblDay() const { return _sec_lbl_day; }
        char GetType() const { return _type; }
        const QTimePoint *GetBgn() const { return _bgn; }
        const QTimePoint *GetEnd() const { return _end; }
    };

    std::ostream &operator<<(std::ostream &os, const QSection &section);

    void match_trade_date(
        const time_t *test_datetime,
        char *this_date, char *prev_date,
        const char *calendarPath);

    // FOR TEST
    void test_section();
    void test_calendar(const char *calendarPath);
}