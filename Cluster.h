//
// Created by Jeff on 2019/1/6 0006.
//

#ifndef ISODATA_CLUSTER_H
#define ISODATA_CLUSTER_H

#include <vector>
#include <unordered_set>

using namespace std;

struct Cluster{
    double innerMeanDis; // 类内平均距离
    vector<double> sigma; // 每个聚类的标准差
    static double allMeanDis; // 总体平均距离
    vector<double> center; // 聚类中心位置的
    unordered_set<unsigned> ids; // 从属于此聚类的样本的id，位于isodata的data中的
    Cluster():
        center{}, innerMeanDis(0), sigma(vector<double>{}){}
    explicit Cluster(vector<double> &c):
        center(c), innerMeanDis(0), sigma(vector<double>(c.size(), 0)) {}
    void add_point(int p_index);
    void clear_ids();
};


#endif //ISODATA_CLUSTER_H
