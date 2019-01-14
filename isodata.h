//
// Created by Jeff on 2019/1/2 0002.
//

#ifndef ISODATA_ISODATA_H
#define ISODATA_ISODATA_H

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <time.h>
#include <random>
#include "error.h"
#include "Cluster.h"
#include <deque>
#include <functional>
#include "common.h"

using namespace std;
// 实现ISODATA聚类算法

class isodata {
private:
    typedef function<vector<vector<double>>(void)> READFUNC;
    // 直接初始化的数据
    // 为了简单，不预留更改设置的接口，只在初始化时设置
    unsigned _c; // 预期的聚类个数
    unsigned _nc; // 初始聚类中心数目
    unsigned _tn; // 每一类中允许的样本最少数目
    double _te; // 类内相对标准差上限
    double _tc; // 聚类中心点之间的最小距离
    unsigned _nt; // 每次迭代中最多可以合并的次数
    unsigned _ns; // 最多迭代次数
    // 不直接在构造函数中初始化
    unsigned row; // 数据的行数，也就是样本个数
    unsigned col; // 数据的列数，也就是特征个数
    vector<vector<double>> data; // 待分类的数据
    deque<Cluster> clusters; // 聚类
    READFUNC read_func; // 读取数据的函数，可以自定义
    double alpha; // 分裂系数
public:
    /**
     * 构造函数
     * @param _c 预期的聚类个数
     * @param _nc 初始聚类中心数目
     * @param _tn 每一类中允许的样本最少数目
     * @param _te 类内相对标准差上限
     * @param _tc 聚类中心点之间的最小距离
     * @param _nt 每次迭代中最多可以合并的次数
     * @param _ns 最多迭代次数
     * @param func 读取数据的函数，无输入，返回二维double vector
     */
    explicit isodata(unsigned int c, unsigned int _nc, unsigned int _tn,
                     double _te, double _tc, unsigned int _nt,
                     unsigned int _ns, READFUNC func) :
                     _c(c), _nc(_nc), _tn(_tn),
                     _te(_te), _tc(_tc), _nt(_nt),
                     _ns(_ns), row(0), col(0),
                     clusters(), read_func(std::move(func)), alpha(0.3) {
    }



    void run()
    {
        setData();
        init_clusters();
        for (int i = 0; i < _ns; ++i) {
            re_assign();
            check_tn();
            update_centers();
            update_meandis();
            switch_method(i);
        }
        output();
    }

private:
    void setData();
    void init_clusters();
    pair<int, double> get_nearest_cluster(int p_index, int ignore);
    pair<int, double> get_nearest_cluster(int p_index, unordered_set<unsigned>&);
    void re_assign();
    void check_tn();
    void update_centers();
    void update_center(Cluster& cluster);
    void update_sigmas();
    void update_sigma(Cluster& cluster);
    void update_meandis();
    void check_split();
    void split(const int& c_index);
    void check_merge();
    void merge(const int& id1, const int& id2);
    void switch_method(const int& index);
    void output() const;
};


#endif //ISODATA_ISODATA_H

