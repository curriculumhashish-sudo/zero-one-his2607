#pragma once
#include "Mapper.h"
#include "../../domain/do/settle/GetTotalFeesDO.h"

// 值语义Mapper
class GetTotalFeesMapper : public Mapper<GetTotalFeesDO>
{
public:
    GetTotalFeesDO mapper(ResultSet* resultSet) const override
    {
        GetTotalFeesDO data;
        data.setEncounter_id(resultSet->getString("encounter_id"));
        data.setTotal_hospital_fee(resultSet->getDouble("total_hospital_fee"));
        data.setInsurance_pay_fee(resultSet->getDouble("insurance_pay_fee"));
        data.setPersonal_pay_fee(resultSet->getDouble("personal_pay_fee"));
        data.setPrepay_total_fee(resultSet->getDouble("prepay_total_fee"));
        data.setFinal_balance_fee(resultSet->getDouble("final_balance_fee"));
        data.setAccount_balance(resultSet->getDouble("account_balance"));
        return data;
    }
};

// 指针语义Mapper
class PtrGetTotalFeesMapper : public Mapper<PtrGetTotalFeesDO>
{
public:
    PtrGetTotalFeesDO mapper(ResultSet* resultSet) const override
    {
        auto data = std::make_shared<GetTotalFeesDO>();
        data->setEncounter_id(resultSet->getString("encounter_id"));
        data->setTotal_hospital_fee(resultSet->getDouble("total_hospital_fee"));
        data->setInsurance_pay_fee(resultSet->getDouble("insurance_pay_fee"));
        data->setPersonal_pay_fee(resultSet->getDouble("personal_pay_fee"));
        data->setPrepay_total_fee(resultSet->getDouble("prepay_total_fee"));
        data->setFinal_balance_fee(resultSet->getDouble("final_balance_fee"));
        data->setAccount_balance(resultSet->getDouble("account_balance"));
        return data;
    }
};

