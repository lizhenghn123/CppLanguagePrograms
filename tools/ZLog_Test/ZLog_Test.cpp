 #include "base/ZLog.h"
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
    zl_log_instance_create("../log", "testlog");
    test_log();
    zl_log_instance_destroy();

    system("pause");
    return 0;
}