#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <fstream>
#include <unordered_map>

#include "/home/zhenyu/smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"
#include "record.hpp"


inline void appendToMap(std::unordered_map<std::string, AccountingVoucher > &map, const std::string &key, Record& elem)
{
    AccountingVoucher voucher;
    if(map.find(key) != map.end())
    {
        //find the key
        voucher = map[key];
    }
    voucher.push_back(elem);
    map[key] = voucher;
}

    template <class T_key, class T_elem >
inline void appendToMap(std::unordered_map<T_key, std::vector<T_elem> > &map_key_elem_vec, const T_key &key, T_elem elem)
{
    std::vector<T_elem> elem_vec;
    if(map_key_elem_vec.find(key) != map_key_elem_vec.end())
    {
        //find the key
        elem_vec = map_key_elem_vec[key];
    }
    elem_vec.emplace_back(elem);
    map_key_elem_vec[key] = elem_vec;
}

inline std::vector<std::string> splitString(std::string str, std::string delimiter)
{
    //reference: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> tokens;
    //printv(str);
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        //std::cout << token << std::endl;
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    //printv(str.size());
    if(str.size()>0)
    {
        tokens.push_back(str);
    }
    return tokens;
}

inline std::vector<std::vector<std::string>> readCSV(std::string file_path, int header_line_skip, std::string delimiter)
{
    std::vector<std::vector<std::string>> data_csv;

    std::cout<<"openning "<<file_path<<" ..."<<std::endl;
    // Open an existing file
    std::ifstream fin(file_path, std::ios::in);
    if(fin.is_open())
    {
        std::string line, word, temp;
        int count = 0;
        for(int i = 0 ; i < header_line_skip ; ++i)
        {
            getline(fin, line); //read the header line
            std::cout<<"Skipped header line: "<<line<<std::endl;
        }
        while (getline(fin, line))
        {
            //printvP(line);
            std::vector<std::string> data_line = splitString(line, delimiter);
            //printvP(data_line.size());

            //// used for breaking words
            //std::stringstream s(line);

            //// read every column data of a row and
            //// store it in a string variable, 'word'
            //while (getline(s, word, ','))
            //{
            //    //printv(word);
            //    // add all the column data
            //    // of a row to a vector
            //    data_line.push_back(word);
            //}
            data_csv.push_back(data_line);
        }
    }

    return data_csv;
}

#endif
