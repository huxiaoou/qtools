#include "QWidgets.h"

namespace QUtility
{
    bool checkDirExistence(const char *dirPath)
    {
        struct stat s;
        return stat(dirPath, &s) == 0 && S_ISDIR(s.st_mode);
    }

    void checkAndMkdir(const char *dirPath)
    {
        if (!checkDirExistence(dirPath))
            mkdir(dirPath, 0744); 
        // 1 = execute
        // 2 = write
        // 4 = read
    }

    void test_widgets()
    {
        const char *p0 = "/mnt/data/trade";
        if (checkDirExistence(p0))
            std::cout << "Path = " << p0 << " exist." << std::endl;
        else
            std::cout << "Path = " << p0 << " DOES NOT exist." << std::endl;

        const char *p1 = "/mnt/data/another_dir";
        if (checkDirExistence(p1))
            std::cout << "Path = " << p1 << " exist." << std::endl;
        else
            std::cout << "Path = " << p1 << " DOES NOT exist." << std::endl;

        const char *p2 = "/home/ubuntu/tmp/test_dir";
        checkAndMkdir(p2);
    }
}