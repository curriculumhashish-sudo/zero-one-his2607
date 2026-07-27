#pragma once
#ifndef _GET_TOTAL_FEES_DO_
#define _GET_TOTAL_FEES_DO_
#include "../DoInclude.h"


class GetTotalFeesDO : public BaseDO
{
    MYSQL_SYNTHESIZE(long long, encounter_id, Encounter_id);
    
    MYSQL_SYNTHESIZE(double, total_hospital_fee, Total_hospital_fee);
    
    MYSQL_SYNTHESIZE(double, personal_pay_fee, Personal_pay_fee);
    
    MYSQL_SYNTHESIZE(double, prepay_total_fee, Prepay_total_fee);
    
    MYSQL_SYNTHESIZE(double, final_balance_fee, Final_balance_fee);

public:
    GetTotalFeesDO() : BaseDO("GetTotalFees")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "ll", encounter_id);
        MYSQL_ADD_FIELD("total_hospital_fee", "d", total_hospital_fee);
        MYSQL_ADD_FIELD("personal_pay_fee", "d", personal_pay_fee);
        MYSQL_ADD_FIELD("prepay_total_fee", "d", prepay_total_fee);
        MYSQL_ADD_FIELD("final_balance_fee", "d", final_balance_fee);
    }
};

typedef std::shared_ptr<GetTotalFeesDO> PtrGetTotalFeesDO;

#endif // !_GET_TOTAL_FEES_DO_