#pragma once
#ifndef _GET_TOTAL_FEES_DO_
#define _GET_TOTAL_FEES_DO_
#include "../DoInclude.h"

/**
 * 患者费用合计DO（多表汇总查询结果实体）
 * 关联表：fin_cashier_settlement、fin_payment_rec_detail
 */
class GetTotalFeesDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);
    // 住院总费用
    MYSQL_SYNTHESIZE(double, total_hospital_fee, Total_hospital_fee);
    // 医保支付金额
    MYSQL_SYNTHESIZE(double, insurance_pay_fee, Insurance_pay_fee);
    // 个人自付金额
    MYSQL_SYNTHESIZE(double, personal_pay_fee, Personal_pay_fee);
    // 预交押金总额
    MYSQL_SYNTHESIZE(double, prepay_total_fee, Prepay_total_fee);
    // 应补/应退金额
    MYSQL_SYNTHESIZE(double, final_balance_fee, Final_balance_fee);
    // 账户余额
    MYSQL_SYNTHESIZE(double, account_balance, Account_balance);

public:
    // 汇总查询DO，非单表实体，构造参数传空字符串
    GetTotalFeesDO() : BaseDO("")
    {
        MYSQL_ADD_FIELD_PK("encounter_id", "s", encounter_id);
        MYSQL_ADD_FIELD("total_hospital_fee", "d", total_hospital_fee);
        MYSQL_ADD_FIELD("insurance_pay_fee", "d", insurance_pay_fee);
        MYSQL_ADD_FIELD("personal_pay_fee", "d", personal_pay_fee);
        MYSQL_ADD_FIELD("prepay_total_fee", "d", prepay_total_fee);
        MYSQL_ADD_FIELD("final_balance_fee", "d", final_balance_fee);
        MYSQL_ADD_FIELD("account_balance", "d", account_balance);
    }
};

typedef std::shared_ptr<GetTotalFeesDO> PtrGetTotalFeesDO;

#endif // !_GET_TOTAL_FEES_DO_
