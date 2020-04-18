#ifndef RECORD_HPP
#define RECORD_HPP

class Record
{
    public:

        std::string voucher_id;
        std::string description;
        std::string item_number;
        std::string item_name;
        double lend;
        double borrow;
};

class AccountingVoucher
{
    public:

        std::string voucher_id;
        std::vector<Record> record_vec;

        double total_lend = 0;
        double total_borrow = 0;

        void push_back(Record& record)
        {
            record_vec.push_back(record);

            total_lend += record.lend;
            total_borrow += record.borrow;
        }

        int size()
        {
            return record_vec.size();
        }

        Record& operator[](int idx)
        {
            return record_vec[idx];
        }
};

#endif
