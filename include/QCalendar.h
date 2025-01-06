#pragma once

#include <stdlib.h>
#include <cstring>
#include <ctime>
#include <iostream>
#include <chrono>

namespace QUtility
{
    using namespace std::chrono;
    class QTimestamp
    {
    private:
        int64_t _ts;

    public:
        QTimestamp();
        QTimestamp(const int64_t ts);
        QTimestamp(const char *datetime, const char *format);

        const unsigned MilliSeconds() const { return _ts % 1000; };
        const time_t Seconds() const { return _ts / 1000; }
        int64_t getTs() const { return _ts; }

        void SyncTpFrmTm(std::tm *tm, unsigned int millisec);
        const std::tm *GetTm() const;
        void PrintDate(char *dest, const char *format) const;
        void PrintDateTime(char *dest, const char *format) const;

        bool operator>(const QTimestamp &other) const { return _ts > other._ts; }
        bool operator>=(const QTimestamp &other) const { return _ts >= other._ts; }
        bool operator<(const QTimestamp &other) const { return _ts < other._ts; }
        bool operator<=(const QTimestamp &other) const { return _ts <= other._ts; }
        bool operator==(const QTimestamp &other) const { return _ts == other._ts; }
        QTimestamp operator+(const int64_t delta) const { return QTimestamp(_ts + delta); }
        QTimestamp operator-(const int64_t delta) const { return QTimestamp(_ts - delta); }
        QTimestamp *shift_milliSeconds(int64_t delta) const { return new QTimestamp(_ts + delta); }
        QTimestamp *shift_seconds(int64_t delta) const { return new QTimestamp(_ts + delta * 1000); }
        QTimestamp *shift_minutes(int64_t delta) const { return new QTimestamp(_ts + delta * 60 * 1000); }
        QTimestamp *shift_hours(int64_t delta) const { return new QTimestamp(_ts + delta * 60 * 60 * 1000); }
    };

    std::ostream &operator<<(std::ostream &os, const QTimestamp &tp);

    class QSection
    {
    private:
        QTimestamp *_bgn;
        QTimestamp *_end;
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
        const QTimestamp *GetBgn() const { return _bgn; }
        const QTimestamp *GetEnd() const { return _end; }
        bool hasTimepoint(const QTimestamp *tp) const { return (*_bgn <= *tp) && (*tp <= *_end); }
    };

    std::ostream &operator<<(std::ostream &os, const QSection &section);

    void match_trade_date(
        const QTimestamp &test_timestamp,
        char *this_date, char *prev_date,
        const char *calendarPath);

    // FOR TEST
    void test_timepoint();
    void test_section();
    void test_calendar(const char *calendarPath);
}