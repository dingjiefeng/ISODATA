//
// Created by Jeff on 2019/1/6 0006.
//

#include "Cluster.h"
#include "common.h"

double Cluster::allMeanDis = 0;

/**
 * 向聚类中添加点
 * @param p_index 点的id
 */
void Cluster::add_point(int p_index) {
    if (ids.find(static_cast<const unsigned int &>(p_index)) != ids.end())
    {
        cout << WARN_POINT_REPEAT << endl;
        return;
    }
    ids.emplace(p_index);
}




/**
 * 清除聚类中的所有点
 */
void Cluster::clear_ids() {
    ids.clear();
}
