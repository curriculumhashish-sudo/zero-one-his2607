#pragma once
#ifndef GETCATEGORYDETAILSDO_H
#define GETCATEGORYDETAILSDO_H
#include "../DOInclude.h"

class GetCategoryDetailsDO : public BaseDO
{
private:
   
    MYSQL_SYNTHESIZE(int, sort_num, Sort_num);
    MYSQL_SYNTHESIZE(std::string, charge_type_name, Charge_type_name);
    MYSQL_SYNTHESIZE(double, type_total_fee, Type_total_fee);


public:
    GetCategoryDetailsDO() : BaseDO("")
    {
      
        MYSQL_ADD_FIELD("sort_num", "i", sort_num);
        MYSQL_ADD_FIELD("charge_type_name", "s", charge_type_name);
        MYSQL_ADD_FIELD("type_total_fee", "d", type_total_fee);
    }
};
#endif