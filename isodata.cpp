//
// Created by Jeff on 2019/1/2 0002.
//
#include "isodata.h"
#include <cmath>
#include <queue>
#include <algorithm>
#include <fstream>


/**
* 设置读入数据
* @param _data 用于分类的样本数据
*/
void isodata::setData()
{
    data = std::move(read_func());
    if (data.size() < _c || data.size() < _tn)
    {
        cout << WARN_DATA_SIZE << endl;
        return;
    }

    row  = static_cast<unsigned int>(data.size());
    col = static_cast<unsigned int>(data[0].size());
    for (vector<double> & d : data)
    {
        if (d.size() != col)
        {
            cout << WARN_DATA_SIZE << endl;
            data.clear();
            return;
        }
    }
}

/**
 * 初始化 随机选取_nc个聚类中心
 */
void isodata::init_clusters() {
    // 选取随机id
    std::default_random_engine rand(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<unsigned> rnd(0, row-1);
    unordered_set<unsigned> ids;
    while (ids.size() < _nc)
    {
        auto id = rnd(rand);
        // 避免数据之间有重复的 选取出重复的中心点坐标
        bool flg = false;
        for (auto &item : ids)
        {
            if (get_distance(data[item], data[id]) == 0.0)
                flg = true;
        }
        if (!flg)
            ids.emplace(id);
    }
    // 初始化聚类
    for (auto &id : ids)
    {
        clusters.emplace_back(Cluster{this->data[id]});
    }

}

/**
 * 根据距离聚类中心的最小距离 选取当前点所属的聚类
 * @p_index 当前点在data中的序号
 * @ignore 设置忽略序号， 缺省为-1， 主要用于删除聚类时使用
 * @return 序号+距离
 */
pair<int, double> isodata::get_nearest_cluster(int p_index, int ignore = -1) {
    if (ignore != -1 && clusters.size() == 1)
        cout << WARN_CLUSTER_SIZE_SMALL << endl;
    int c_index = 0;
    double dis(get_distance(data[p_index], clusters[c_index].center));
    for (int i = 1; i < clusters.size(); ++i) {
        if (ignore != -1 && i == ignore)
            continue;
        auto d = get_distance(data[p_index], clusters[i].center);
        if (d < dis)
        {
            dis = d;
            c_index = i;
        }
    }
    return {c_index, dis};
}
/**
 * 根据距离聚类中心的最小距离 选取当前点所属的聚类
 * @param p_index
 * @param cluster_ids 一个集合，集合内的聚类中心不做考虑
 * @return 序号+距离
 */
pair<int, double> isodata::get_nearest_cluster(int p_index, unordered_set<unsigned> &cluster_ids) {
    // 选择第一个不在cluster_ids中的聚类
    int c_index = 0;
    while (cluster_ids.find(static_cast<const unsigned int &>(c_index)) != cluster_ids.end())
        ++c_index;
    //初始一个距离
    double dis(get_distance(data[p_index], clusters[c_index].center));
    for (int i = c_index+1;
    i < clusters.size() && (cluster_ids.find(static_cast<const unsigned int &>(i)) == cluster_ids.end());
    ++i)
    {
        auto d = get_distance(data[p_index], clusters[i].center);
        if (d < dis)
        {
            dis = d;
            c_index = i;
        }
    }
    return {c_index, dis};
}

/**
 * 依据距离最小原则重新分配点
 */
void isodata::re_assign() {
    for (auto &cluster : clusters) {
        cluster.clear_ids();
    }
    for (int i = 0; i < row; ++i) {
        auto&& res = get_nearest_cluster(i);
        clusters[res.first].add_point(i);
    }
}

/**
 * 检测每个聚类中的个数是否少于_tn，如果少于则取消此类别
 */
void isodata::check_tn() {
    unordered_set<unsigned> to_erase;
    for (int i = 0; i < clusters.size(); ++i) {
        if (clusters[i].ids.size() >= _tn)
            continue;
        to_erase.emplace(i);
        auto &indexs = clusters[i].ids;
        for (unsigned int index : indexs) {
            auto &&res = get_nearest_cluster(index, to_erase);
            clusters[res.first].add_point(index);
        }
        clusters[i].ids.clear();

    }
    for (auto it = clusters.begin(); it != clusters.end();)
    {
        if (it->ids.empty())
            it = clusters.erase(it);
        else
            ++it;
    }
}

/**
 * 更新各个聚类的中心坐标
 */
void isodata::update_centers() {
    for (auto &cluster : clusters)
    {
        update_center(cluster);
    }
}

/**
 * 更新某个聚类的中心坐标
 */
void isodata::update_center(Cluster& cluster) {
    vector<double> sum(col, 0);
    for (auto &index : cluster.ids)
    {
        sum += data[index];
    }
    cluster.center = sum/ static_cast<double>(cluster.ids.size());
}


/**
 * 更新各个聚类的标准差
 */
void isodata::update_sigmas() {
    for ( auto& cluster : clusters) {
        update_sigma(cluster);
    }
}

/**
 * 更新单个聚类的标准差
 */
void isodata::update_sigma(Cluster& cluster) {
    cluster.sigma.clear();
    auto &sigma = cluster.sigma;
    sigma.resize(col, 0);
    for (auto& id : cluster.ids) {
        sigma += mypow(cluster.center - data[id], 2);
    }
    auto &&res(vector_sqrt(sigma));
    swap(res, sigma);
}

/**
 * 更新平均距离
 */
void isodata::update_meandis() {
    Cluster::allMeanDis = 0;
    for (auto &&cluster : clusters) {
        double dis(0.0);
        for (auto &id : cluster.ids) {
            dis += get_distance(data[id], cluster.center);
        }
        Cluster::allMeanDis += dis;
        cluster.innerMeanDis = dis/cluster.ids.size();
    }
    Cluster::allMeanDis /= row;
}


/**
 * 检测是否需要分裂，如果需要则执行分裂操作
 */
void isodata::check_split() {
    update_sigmas();
    while (true)
    {
        bool flag = false;
        for (unsigned j = 0; j < clusters.size(); ++j) {
            auto& cluster = clusters[j];
            for (int i = 0; i < col; ++i) {
                if (cluster.innerMeanDis > Cluster::allMeanDis) {
                    if (cluster.sigma[i] > _te && (cluster.ids.size() > 2 * _tn + 1 || clusters.size() < _c / 2)) {
                        flag = true;
                        split(j);
                    }
                } else {
                    if (cluster.sigma[i] > _te && clusters.size() < _c / 2) {
                        flag = true;
                        split(j);
                    }
                }
            }
        }
        if (!flag)
            break;
        else
            update_meandis();
    }
}

/**
 * 分裂第c_index个聚类
 * @param c_index
 */
void isodata::split(const int &c_index) {
    //根据标准差选取分裂维度
    auto& cluster = clusters[c_index];
    auto iter = max_element(cluster.sigma.begin(), cluster.sigma.end());
    auto pos = distance(cluster.sigma.begin(), iter);
    Cluster newcluster(cluster.center);
    //分裂
    newcluster.center[pos] -= alpha*cluster.center[pos];
    cluster.center[pos] += alpha*cluster.center[pos];
    for (const auto &id : cluster.ids) {
        auto d1 = get_distance(data[id], cluster.center);
        auto d2 = get_distance(data[id], newcluster.center);
        if (d2 < d1)
            newcluster.ids.emplace(id);
    }
    unordered_set<unsigned> dids;
    set_difference(cluster.ids.begin(), cluster.ids.end(),
                   newcluster.ids.begin(), newcluster.ids.end(),
                   inserter(dids, dids.begin()));
    swap(dids, cluster.ids);
    // 更新参数并保存新聚类
    update_center(newcluster);
    update_sigma(newcluster);
    update_center(cluster);
    update_sigma(cluster);
    clusters.emplace_back(newcluster);
}


/**
 * 检测是否需要合并，如果需要则执行合并操作
 */
void isodata::check_merge() {
    // 自定义数据类型
    typedef pair<pair<unsigned, unsigned>, double> UNIT;
    //1 计算聚类中心两两之间的距离，保留小于_tc的，并从小到达排序
    vector<UNIT> uvec;
    for (unsigned i = 0; i < clusters.size(); ++i) {
        for (unsigned j = i+1; j < clusters.size(); ++j) {
            auto dis = get_distance(clusters[i].center, clusters[j].center);
            if (dis < _tc)
                uvec.emplace_back(UNIT({i,j}, dis));
        }
    }
    sort(uvec.begin(), uvec.end(), [](UNIT& left, UNIT& right){ return left.second < right.second;});
    //2 执行合并操作
    unordered_set<unsigned> clusterIds{};
    unsigned cnt(0);
    for (const auto &unit : uvec) {
        auto& cids = unit.first;
        auto& cdis = unit.second;
        if (clusterIds.find(cids.first) == clusterIds.end() &&
            clusterIds.find(cids.second) == clusterIds.end())
        {
            merge(cids.first, cids.second);
            clusterIds.emplace(cids.first);
            clusterIds.emplace(cids.second);
        }
        // 检测是否超过最大次数
        if (++cnt > _ns)
            break;
    }
    //3 清除被合并的聚类
    for (auto iter = clusters.begin(); iter != clusters.end();)
    {
        if (iter->ids.empty())
            iter = clusters.erase(iter);
        else
            iter++;
    }
}

/**
 * 合并操作 合并id1 id2的聚类
 * @param id1
 * @param id2
 */
void isodata::merge(const int &id1, const int &id2) {
    auto &c1 = clusters[id1];
    auto &c2 = clusters[id2];
    c1.center = (c1.center* c1.ids.size() + c2.center*c2.ids.size())/(c1.ids.size()+c2.ids.size());
    c2.ids.clear();
}

/**
 * 根据情况选择下一步操作
 */
void isodata::switch_method(const int& index) {
    if (index == _ns-1)
    {
        _tc = 0;
        check_merge();
    } else if (clusters.size() <= _c/2)
    {
        check_split();
    } else if (index % 2 == 0 || clusters.size() >= 2*_c)
    {
        check_merge();
    } else
    {
        check_split();
    }
}

/**
 * 输出聚类分析的结果
 */
void isodata::output() const {
    // 在命令行窗口打印
    cout << "Original Data Number : " << row << endl;
    cout << "Cluster Number : " << clusters.size() << endl;
    for (int i = 0; i < clusters.size(); ++i) {
        cout << "Number " << to_string(i+1) << " : " << clusters[i].ids.size() << endl;
        cout << "cluster center : " << clusters[i].center << endl;
    }
    // 输出到文件
    ofstream res(R"(E:\CPP\Clion\ISODATA\clusters.txt)");
    if (!res.is_open())
        cout << WARN_FILE_OPEN_FAIL << endl;
    res << clusters.size() << endl;
    for (int j = 0; j < clusters.size(); ++j) {
        res << j+1 << " " << clusters[j].ids.size() << endl;
        for (const auto &id : clusters[j].ids) {
            res << data[id] << endl;
        }
    }
}





