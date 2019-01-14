//
// Created by Jeff on 2019/1/6 0006.
//

#ifndef ISODATA_COMMON_H
#define ISODATA_COMMON_H

#include <vector>
#include <iostream>
#include <cmath>
#include "error.h"

using namespace std;

vector<vector<double>> read_data();

template <typename T>
double get_distance(vector<T> &p1, vector<T> &p2);
template <typename T>
vector<T> operator+(vector<T> &left, vector<T>& right);
template <typename T>
vector<T> operator+(vector<T> left, vector<T> right);
template <typename T>
void operator+=(vector<T> &left, vector<T>& right);
template <typename T1, typename T2>
vector<double> operator/(vector<T1> &left, T2 r);
template <typename T1, typename T2>
vector<double> operator/(vector<T1> &&left, T2 r);
template <typename T>
void operator/=(vector<T> &left, double r);
template <typename T>
vector<double> mypow(vector<T> &left, double p);
template <typename T>
vector<double> mypow(vector<T> &&left, double p);
template <typename T>
vector<T> operator-(vector<T> &left, vector<T> &right);
template <typename T>
void operator+=(vector<T>& left, vector<T>&& right);
template <typename T>
vector<double> vector_sqrt(vector<T>& left);
template <typename T>
vector<double> operator*(vector<double> &left, T d);
template <typename T>
ostream& operator<<(ostream& out, vector<T>& tvec);



/**
 * 获取两个样本之间的欧式距离
 * @param p1
 * @param p2
 * @return
 */
template <typename T>
double get_distance(vector<T> &p1, vector<T> &p2)
{
    if (p1.size() != p2.size())
    {
        cout << WARN_VECTOR_SIZE << endl;
        return -1;
    }else{
        double res(0);
        for (int i = 0; i < p1.size(); ++i) {
            res += pow(p1[i]-p2[i], 2);
        }
        return sqrt(res);
    }
}

/**
 * 重载一种矢量相加的方法
 * @tparam T
 * @param left
 * @param right
 * @return
 */
template <typename T>
vector<T> operator+(vector<T> &left, vector<T>& right)
{
    auto len = left.size();
    vector<T> res(len, 0);
    if (left.size() != right.size())
        cout << WARN_VECTOR_SIZE << endl;

    for (int i = 0; i < len; ++i)
    {
        res[i] = left[i] + right[i];
    }
    return res;
}

/**
 * 重载一种矢量自加的方法
 * @tparam T
 * @param left
 * @param right
 */
template <typename T>
void operator+=(vector<T> &left, vector<T>& right)
{
    auto len = left.size();
    if (left.size() != right.size())
        cout << WARN_VECTOR_SIZE << endl;
    for (int i = 0; i < len; ++i)
    {
        left[i] += right[i];
    }

}

/**
 * 重载一种矢量被除的方法
 * @tparam T
 * @param left
 * @param r
 * @return
 */
template <typename T1, typename T2>
vector<double> operator/(vector<T1> &left, T2 r)
{
    auto len = left.size();
    vector<double> res;
    res.resize(len, 0);
    for (int i = 0; i < len; ++i)
    {
        res[i] = left[i] / static_cast<double>(r);
    }
    return res;
}
template <typename T1, typename T2>
vector<double> operator/(vector<T1> &&left, T2 r)
{
    auto len = left.size();
    vector<double> res;
    res.resize(len, 0);
    for (int i = 0; i < len; ++i)
    {
        res[i] = left[i] / static_cast<double>(r);
    }
    return res;
}

/**
 * 重载一种矢量自除的方法
 * @tparam T
 * @param left
 * @param r
 * @return
 */
template <typename T>
void operator/=(vector<T> &left, double r)
{
    auto &&res = left/r;
    swap(res, left);
}

/**
 * 定义一种矢量的幂运算，每个元素依次幂运算后返回
 * @tparam T
 * @param left 矢量
 * @param p 幂次
 * @return
 */
template <typename T>
vector<double> mypow(vector<T> &left, double p)
{
    auto len = left.size();
    vector<double> res;
    res.resize(len);
    for (int i = 0; i < len; ++i)
    {
        res[i] = pow(left[i], p);
    }
    return res;
}
template <typename T>
vector<double> mypow(vector<T> &&left, double p)
{
    auto len = left.size();
    vector<double> res;
    res.resize(len);
    for (int i = 0; i < len; ++i)
    {
        res[i] = pow(left[i], p);
    }
    return res;
}

/**
 * 矢量相减
 * @tparam T
 * @param left
 * @param right
 * @return
 */
template <typename T>
vector<T> operator-(vector<T> &left, vector<T> &right)
{
    if (left.size() != right.size())
        cout << WARN_VECTOR_SIZE << endl;
    auto len = left.size();
    vector<T> res;
    res.resize(len);
    for (int i = 0; i < len; ++i) {
        res[i] = left[i] - right[i];
    }
    return res;
}

/**
 * 一种矢量自加运算
 * @tparam T
 * @param left
 * @param right
 */
template <typename T>
void operator+=(vector<T>& left, vector<T>&& right)
{
    if (left.size() != right.size())
        cout << WARN_VECTOR_SIZE << endl;
    auto len(left.size());
    for (int i = 0; i < len; ++i) {
        left[i] += right[i];
    }
}

/**
 * 矢量开平方
 * @tparam T
 * @param left
 * @return
 */
template <typename T>
vector<double> vector_sqrt(vector<T>& left)
{
    auto len(left.size());
    vector<double> res;
    res.resize(len);
    for (int i = 0; i < len; ++i) {
        res[i] = sqrt(left[i]);
    }
    return res;
}


/**
 * 定义一个矢量乘标量运算
 * @tparam T
 * @param left
 * @param d
 * @return
 */
template<typename T>
vector<double> operator*(vector<double> &left, T d) {
    vector<double> res(left);
    for (auto &item : res) {
        item *= d;
    }
    return res;
}

template<typename T>
vector<T> operator+(vector<T> left, vector<T> right) {
    vector<T> res(left);
    for (int i = 0; i < left.size(); ++i) {
        res[i] += right[i];
    }
    return res;
}

/**
 * 矢量输出
 * @tparam T
 * @param out
 * @param tvec
 * @return
 */
template<typename T>
ostream &operator<<(ostream &out, vector<T> &tvec) {
    auto len(tvec.size());
    for (int i = 0; i < len; ++i) {
        out << tvec[i];
        if (i < len-1)
            out << ",";
    }
    return out;
}
template<typename T>
ostream &operator<<(ostream &out, const vector<T> &tvec) {
    auto len(tvec.size());
    for (int i = 0; i < len; ++i) {
        out << tvec[i];
        if (i < len-1)
            out << ",";
    }
    return out;
}
#endif //ISODATA_COMMON_H
