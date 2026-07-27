// ======================================================
// GetTotalFeesMapper.h - 患者费用合计 - 结果集映射器
// Mapper 的工作流程：
// 1. DAO 执行 SQL → 产生 ResultSet（N行）
// 2. DAO 调用 sqlSession->executeQuery<GetTotalFeesDO>(sql, GetTotalFeesMapper(), params)
// 3. 框架对 ResultSet 每行调用 mapper(resultSet) → 返回一个 GetTotalFeesDO
// 4. 所有 DO 收集到 std::list<GetTotalFeesDO> 返回给 DAO
// ======================================================
#pragma once
#include "Mapper.h"
#include "../../domain/do/GetTotalFeesDO.h"

// 值语义Mapper - 用于列表查询，返回 std::list<GetTotalFeesDO>
class GetTotalFeesMapper : public Mapper<GetTotalFeesDO>
{
public:
	GetTotalFeesDO mapper(ResultSet* resultSet) const override
	{
		GetTotalFeesDO data;
		data.setTotal_hospital_fee(resultSet->getDouble("total_hospital_fee"));
		data.setPersonal_pay_fee(resultSet->getDouble("personal_pay_fee"));
		data.setPrepay_total_fee(resultSet->getDouble("prepay_total_fee"));
		data.setFinal_balance_fee(resultSet->getDouble("final_balance_fee"));
		return data;
	}
};

// 指针语义Mapper – 用于需要 shared_ptr 的场景
class PtrGetTotalFeesMapper : public Mapper<PtrGetTotalFeesDO>
{
public:
	PtrGetTotalFeesDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<GetTotalFeesDO>();
		data->setTotal_hospital_fee(resultSet->getDouble("total_hospital_fee"));
		data->setPersonal_pay_fee(resultSet->getDouble("personal_pay_fee"));
		data->setPrepay_total_fee(resultSet->getDouble("prepay_total_fee"));
		data->setFinal_balance_fee(resultSet->getDouble("final_balance_fee"));
		return data;
	}
};
