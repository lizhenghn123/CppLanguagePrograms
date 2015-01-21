#include <iostream>
#include "base/ZLog.h"
#include "base/Timestamp.h"
using namespace zl::base;

void test_log()
{
    LOG_DEBUG("Hello World [%d]", 1000);
    LOG_INFO("Hello World [%s][%d]", "ddddert", 1000);
    LOG_INFO("Hello World [%d][%s]", 1000, "ddddert");
    LOG_NOTICE("this is a notice");
    LOG_INFO("Hello World");
}

int main()
{
    //zl::base::zl_log_instance_create("../log", "testlog");
    //zl::base::zl_log_set_priority(zl::base::ZL_LOG_PRIO_ALERT);
    LOG_CONSOLE_OUTPUT(false);

    test_log();

    LOG_DISABLE_ALL;

    test_log();
    //zl::base::zl_log_instance_destroy();

    system("pause");
    return 0;
}