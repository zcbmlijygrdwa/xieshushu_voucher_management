#include <iostream>
#include <unordered_map>
#include <set>
#include "utilities.hpp"
#include "record.hpp"

int main(int argc, char** argv)
{
    std::vector<Record> record_vec;
    std::vector<std::vector<std::string>> data = readCSV("../data/sample_data.txt", 0, "\t");
    //for(int i = 0 ; i < 4 ; i++)
    for(int i = 0 ; i < data.size() ; i++)
    {
        Record record;
        printv(data[i].size());
        for(int j = 0 ; j < data[i].size() ; j++)
        {
            std::cout<<"data["<<i<<"]["<<j<<"] = "<<data[i][j]<<std::endl;
        }

        if(data[i].size() == 5)
        {
            record.voucher_id = data[i][0];
            record.description = data[i][1];
            record.item_number = data[i][2];
            record.item_name = data[i][3];
            record.lend = std::stod(data[i][4]);
            record.borrow = 0;

        }
        else if(data[i].size() ==6)
        {
            record.voucher_id = data[i][0];
            record.description = data[i][1];
            record.item_number = data[i][2];
            record.item_name = data[i][3];
            if(data[i][4].size() ==0)
            {
                record.lend = 0;
            }
            else
            {
                record.lend = std::stod(data[i][4]);
            }
            record.borrow = std::stod(data[i][5]);

        }
        else
        {
            std::cout<<"Error when parse the following:"<<std::endl;
            for(int j = 0 ; j < data[i].size() ; j++)
            {
                printv(data[i][j]);
            }
            return -1;
        }

        record_vec.push_back(record);
    }

    printv(record_vec.size());


    //create set of id
    std::unordered_map<std::string, AccountingVoucher > id_voucher_map;
    std::set<std::string> id_set;
    std::vector<std::string> id_vec;
    for(int i = 0 ; i < record_vec.size() ; ++i)
    {
        if(id_set.find(record_vec[i].voucher_id) == id_set.end())
        {
            id_set.insert(record_vec[i].voucher_id);
            id_vec.push_back(record_vec[i].voucher_id);
        }
        appendToMap(id_voucher_map, record_vec[i].voucher_id, record_vec[i]);
    }

    for(int i = 0 ; i < id_vec.size() ; ++i)
    {
        //printv(id_vec[i]);
        AccountingVoucher temp_voucher = id_voucher_map[id_vec[i]];

        int file_count = 1; 

        //write to file
        for(int j = 0 ; j < temp_voucher.size() ; )
        {
            std::ofstream myfile;
            double total = 0;
            std::stringstream ss;
            if(temp_voucher.size()>5)
            {
                ss << "../files/"<<id_vec[i]<<"_"<< file_count <<".txt";
            }
            else
            {
                ss << "../files/"<<id_vec[i]<<".txt";
            }
            myfile.open (ss.str());
            myfile.precision(2);
            myfile << std::fixed;
            for(int k = 0 ; k+j < temp_voucher.size() && k < 5 ; ++k)
            {
                Record record = temp_voucher[j+k];
                myfile << record.voucher_id <<", ";
                myfile << record.description <<", ";
                myfile << record.item_number <<", ";
                myfile << record.item_name <<", ";
                myfile << record.lend <<", ";
                myfile << record.borrow <<"\n";
            }
            myfile <<"合计：" <<temp_voucher.total_lend<<" "<<temp_voucher.total_lend<<" "<<temp_voucher.total_borrow<<"\n";
            myfile.close();
            file_count++;
            j += 5;
        }
    }

    return 0;
}
