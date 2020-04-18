#include <iostream>
#include <unordered_map>
#include <set>
#include "utilities.hpp"
#include "record.hpp"
#include "xlsxwriter.h"

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

    //test excel write
    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("../files/demo.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a format. */
    lxw_format *format = workbook_add_format(workbook);

    /* Set the bold property for the format */
    format_set_bold(format);

    /* Change the column width for clarity. */
    worksheet_set_column(worksheet, 0, 0, 20, NULL);

    /* Write some simple text. */
    worksheet_write_string(worksheet, 0, 0, "记账凭证", NULL);

    /* Write some simple text. */
    worksheet_write_string(worksheet, 1, 0, "单位：广西新昊物流有限责任公司", NULL);
    worksheet_write_string(worksheet, 1, 1, "日期：2020-01-31", NULL);
    worksheet_write_string(worksheet, 1, 2, "附单据数：1", NULL);

    worksheet_write_string(worksheet, 2, 0, "记账凭证号", NULL);
    worksheet_write_string(worksheet, 2, 1, "摘要", NULL);
    worksheet_write_string(worksheet, 2, 2, "科目号", NULL);
    worksheet_write_string(worksheet, 2, 3, "科目名称", NULL);
    worksheet_write_string(worksheet, 2, 4, "借方", NULL);
    worksheet_write_string(worksheet, 2, 5, "贷方", NULL);

    AccountingVoucher temp_voucher = id_voucher_map[id_vec[0]];
    for(int i = 0 ; i < temp_voucher.size() ; ++i)
    {
        Record record = temp_voucher[i];
        worksheet_write_string(worksheet, 3+i, 0, record.voucher_id.c_str(), NULL);
        worksheet_write_string(worksheet, 3+i, 1, record.description.c_str(), NULL);
        worksheet_write_string(worksheet, 3+i, 2, record.item_number.c_str(), NULL);
        worksheet_write_string(worksheet, 3+i, 3, record.item_name.c_str(), NULL);
        worksheet_write_number(worksheet, 3+i, 4, record.lend, NULL);
        worksheet_write_number(worksheet, 3+i, 5, record.borrow, NULL);
    }

    worksheet_write_string(worksheet, 2+temp_voucher.size() + 1, 0, "合计：", NULL);
    worksheet_write_number(worksheet, 2+temp_voucher.size() + 1, 1, temp_voucher.total_lend, NULL);
    worksheet_write_number(worksheet, 2+temp_voucher.size() + 1, 4, temp_voucher.total_borrow, NULL);
    worksheet_write_number(worksheet, 2+temp_voucher.size() + 1, 5, temp_voucher.total_borrow, NULL);

    ///* Text with formatting. */
    //worksheet_write_string(worksheet, 1, 0, "World", format);

    ///* Write some numbers. */
    //worksheet_write_number(worksheet, 2, 0, 123,     NULL);
    //worksheet_write_number(worksheet, 3, 0, 123.456, NULL);

    ///* Insert an image. */
    //worksheet_insert_image(worksheet, 1, 2, "logo.png");

    workbook_close(workbook);

    return 0;
}
