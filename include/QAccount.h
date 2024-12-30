#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include "ThostFtdcUserApiDataType.h"

namespace QUtility
{
    typedef char UTypeLinkMode[12];
    typedef char UTypeFrontAddress[60];
    typedef char UTypePath[240];
    typedef char UTypeAppId[60];
    typedef char UTypeAuthenticCode[60];

    class Account
    {
    private:
        unsigned active;
        UTypeLinkMode _linkMode;
        UTypeFrontAddress _frntAddr;
        TThostFtdcBrokerIDType _brokerId;
        TThostFtdcInvestorIDType _invstrId;
        TThostFtdcPasswordType _password;
        UTypeAppId _appId;
        UTypeAuthenticCode _authenticCode;
        UTypePath _consPath;

        bool Match(char *text_line);

    public:
        Account(const char *account_file_path);
        void Display() const;
        char *GetLinkMode() { return _linkMode; }
        char *GetFrntAddr() { return _frntAddr; }
        char *GetBrokerID() { return _brokerId; }
        char *GetInvstrID() { return _invstrId; }
        char *GetPassword() { return _password; }
        char *GetAppID() { return _appId; }
        char *GetAuthCode() { return _authenticCode; }
        char *GetConsPath() { return _consPath; }
    };
}
