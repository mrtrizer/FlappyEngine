#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "test_gobj.h"

int main(int argc, char** argv)
{
    int status = 1;
    const auto RUN_TEST = [&status, argc, argv] (QObject * obj) {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
    };

    RUN_TEST(new Test_GObj());

    return status;
}
