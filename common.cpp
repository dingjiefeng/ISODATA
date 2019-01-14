//
// Created by Jeff on 2019/1/6 0006.
//
#include "common.h"
#include <iostream>
#include <sstream>
#include <fstream>

/**
 * 读取数据
 * 数据存放在txt文件中，一个样本占用一行，
 * 一行内，不同的特征之间用'，'间隔，行尾无特殊字符
 * @return
 */
vector<vector<double>> read_data()
{
    const string path = R"(E:\CPP\Clion\ISODATA\data.txt)";
    ifstream f(path);
    if (!f.is_open())
    {
        cout << WARN_FILE_OPEN_FAIL << endl;
    }
    vector<vector<double>> data;
    int id(0);
    while (!f.eof())
    {
        string s;
        getline(f, s);
        stringstream ss(s);
        vector<double> line;
        double d;
        char c;
        while (!ss.eof() && ss >> d)
        {
            line.emplace_back(d);
            ss >> c;

        }
        if (!line.empty())
            data.emplace_back(line);
    }
    data.shrink_to_fit();
    return data;
}






