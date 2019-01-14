#include "isodata.h"
#include "MyTime.h"

/**
 * 在运行前，需要自定义common.cpp中的read_data函数
 * @return
 */
int main() {
    CMyTimeWrapper c;
    c.tic();
    isodata isodata1(4, 90, 10, 90, 20, 5, 500, read_data);
    isodata1.run();
    c.tocMs();
    return 0;
}


