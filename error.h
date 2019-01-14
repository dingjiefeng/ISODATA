#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Jeff on 2019/1/4 0004.
//

#ifndef ISODATA_ERROR_H
#define ISODATA_ERROR_H

#include <string>
using std::string;

// 为了省事 所有都是警告 并且只提示 不中断运行
const string WARN_DATA_SIZE("Data size error");
const string WARN_VECTOR_SIZE("Vector size error");
const string WARN_FILE_OPEN_FAIL("Fail to open file");
const string WARN_POINT_REPEAT("Index repeat");
const string WARN_CLUSTER_SIZE_SMALL("Cluster size too small");
#endif //ISODATA_ERROR_H

#pragma clang diagnostic pop