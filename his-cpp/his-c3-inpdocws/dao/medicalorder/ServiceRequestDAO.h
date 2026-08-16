#pragma once
#ifndef _SERVICE_REQUEST_DAO_
#define _SERVICE_REQUEST_DAO_

#include "BaseDAO.h"
#include "../../domain/do/medicalorder/ServiceRequestDO.h"

/**
 * 服务医嘱数据访问对象
 * 操作wkf_service_request表（服务医嘱表）
 */
class ServiceRequestDAO : public BaseDAO
{
public:
	// 按主键查询单条服务医嘱，未找到返回null
	PtrServiceRequestDO selectById(std::string id);

	// 按医嘱ID和租户、就诊、患者范围查询，避免跨患者或跨就诊操作。
    PtrServiceRequestDO selectByScope(const std::string& id, const std::string& tenantId, const std::string& encounterId, const std::string& patientId);
	// 按原状态更新医嘱，只有实际更新一条记录才算操作成功。
    int updateStatus(const std::string& id, const std::string& tenantId, const std::string& encounterId, const std::string& patientId, const std::string& expectedStatus, const std::string& targetStatus, const std::string& operatorId);
};

#endif // !_SERVICE_REQUEST_DAO_
