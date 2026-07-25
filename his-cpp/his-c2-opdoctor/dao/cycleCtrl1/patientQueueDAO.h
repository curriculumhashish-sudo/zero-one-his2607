#pragma once
#ifndef PATIENTQUEUEDAO_H_
#define PATIENTQUEUEDAO_H_

#include "BaseDAO.h"
#include "domain/do/cycleCtrl1/BkgScheduleTemplateDO.h"
#include "domain/do/cycleCtrl1/BkgWaitlistEntryDO.h"
#include "domain/do/cycleCtrl1/AdmPatientDO.h"
#include "domain/do/cycleCtrl1/AdmHealthcareServiceDO.h"
#include "domain/query/cycleCtrl1/patientQueueQuery.h"
#include <string>
#include <list>


/*
BkgWaitlistEntryDO:
需要字段：
	id
	patient_id 关联患者表
	schedule_id 关联排班表
	status 候诊状态编码（待诊 / 在诊 / 完诊）
	created_at 就诊时间
	queue_position 排队序号

AdmPatientDO：
需要字段：
	name 患者姓名
	gender_enum 性别编码
	birth_date 出生日期（用来计算 age）
	bus_no 病历号（页面 EN 开头编号）

BkgScheduleTemplateDO：
需要字段：
	practitioner_id 医生 ID（权限过滤条件）
	healthcare_service_id 关联医疗服务表

AdmHealthcareServiceDO:
需要字段：
	name 号类型名称（专家号 / 普通号）

*/

class PatientQueueDAO : public BaseDAO
{
public:
	//查询患者候诊队列信息
	std::list<PtrBkgWaitlistEntryDO> getPatientQueueList(const PatientQueueQuery::Wrapper& query);
	
};



#endif // !PATIENTQUEUEDAO_H_
