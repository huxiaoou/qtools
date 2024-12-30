#include "qaccount.h"

namespace QUtility
{
    CAccount::CAccount(const char *account_file_path)
    {
        char text_line[240];
        std::ifstream fin;
        fin.open(account_file_path, std::ios_base::in);
        while (fin.peek() != EOF)
        {
            fin.getline(text_line, sizeof(text_line));
            if (Match(text_line))
                break;
        }
        fin.close();
    }

    bool CAccount::Match(char *text_line)
    {
        sscanf(
            text_line, "%u,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",
            &active,
            _linkMode,
            _frntAddr,
            _brokerId,
            _invstrId,
            _password,
            _appId,
            _authenticCode,
            _consPath);
        return active == 1;
    }

    void CAccount::Display() const
    {
        std::cout << "ACCOUNT SUMMARY:" << std::endl;
        std::cout << "------LINK MODE:" << _linkMode << std::endl;
        std::cout << "------FRNT ADDR:" << _frntAddr << std::endl;
        std::cout << "------BROKER ID:" << _brokerId << std::endl;
        std::cout << "------INVSTR ID:" << _invstrId << std::endl;
        std::cout << "------PASSWORD :" << _password << std::endl;
        std::cout << "------APP ID   :" << _appId << std::endl;
        std::cout << "------AUTH CODE:" << _authenticCode << std::endl;
        std::cout << "------CONS PATH:" << _consPath << std::endl;
    }
}
