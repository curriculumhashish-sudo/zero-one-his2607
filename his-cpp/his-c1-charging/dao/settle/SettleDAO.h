#pragma once

#ifndef _SETTLE_DAO_H_
#define _SETTLE_DAO_H_

#include <vector>
#include "BaseDAO.h"
#include "domain/do/settle/SettleDO.h"

// 住院结算数据访问对象
// 写操作保持原子化，后续业务层可通过共享 SqlSession 将这些操作纳入同一事务
class SettleDAO : public BaseDAO
{
public:
	// 锁定并查询本次结算使用的患者账户
	PtrSettlementAccountDO selectAccountForUpdate(
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& accountId);

	// 锁定并查询本次结算的收费项目
	std::list<PtrSettlementChargeItemDO> selectChargeItemsForUpdate(
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& accountId,
		const std::vector<std::string>& chargeItemIds);

	// 新增结算支付流水
	int insertPayment(const SettlementPaymentDO& data);

	// 新增结算支付明细
	int insertPaymentDetails(const std::list<SettlementPaymentDetailDO>& dataList);

	// 新增结算发票
	int insertInvoice(const SettlementInvoiceDO& data);

	// 新增医保预结算记录
	int insertPreSettle(const InpatientPreSettleDO& data);

	// 新增医保正式结算记录
	int insertYbSettlementRecord(const InpatientYbSettlementRecordDO& data);

	// 新增支付流水与医保正式结算记录的关联
	int insertYbLink(const SettlementYbLinkDO& data);

	// 将指定收费项目更新为已结算状态
	int updateChargeItemsToSettled(
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& accountId,
		const std::vector<std::string>& chargeItemIds,
		const std::string& updatedAt,
		const std::string& updatedBy);

	// 使用乐观锁更新账户结账状态
	int updateAccountBillingStatus(
		const std::string& accountId,
		const std::string& patientId,
		const std::string& encounterId,
		const std::string& billingStatusEnum,
		const std::string& updatedAt,
		const std::string& updatedBy,
		int expectedVersion);
};

#endif
