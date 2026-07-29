/*
 Copyright Zero One Star. All rights reserved.

 @Author: yiguang
 @Date: 2026/07/28 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#include "stdafx.h"
#include "AdmissionService.h"
#include "../../dao/enroll/AdmissionDAO.h"
#include "id/UuidFacade.h"

// Nacos客户端，用于防止符号冲突
#include "NacosClient.h"

// 住院入院登记
bool AdmissionService::insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 校验必填字段
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return false;

	// 从DTO中提取字段
	auto name = dto->name;
	auto idCard = dto->id ? oatpp::String(std::to_string(dto->id.getValue(0))) : oatpp::String("");
	auto birthTime = dto->birthtime;
	auto age = dto->age;
	auto sex = dto->sex;
	auto phone = dto->phone ? oatpp::String(std::to_string(dto->phone.getValue(0))) : oatpp::String("");
	auto admissionDept = dto->admissionDept;
	auto admissionWard = dto->admission_ward;
	auto attendingDoctor = dto->attending_doctor;
	auto admissionDiagnosis = dto->admissionDiagnosis;
	auto diagnosisDesc = dto->diagnosisDesc;
	auto patientCondition = dto->patientCondition;
	auto admissionType = dto->admissionType;
	auto admissionMode = dto->admissionMode;
	auto feeNature = dto->feeNature;
	auto admissionDate = dto->admissiondata;

	// 从payload获取认证信息（tenantId, orgId, operUserId）
	auto tenantId = oatpp::String(payload.getId());
	auto orgId = oatpp::String("1");      // 默认科室ID，可在JWT中扩展
	auto operUserId = oatpp::String(payload.getId());
	auto prepayAmount = oatpp::String("0"); // 默认预交金额
	auto payWay = oatpp::String("CASH");     // 默认支付方式

	// 输出参数
	oatpp::String outPatientId;
	oatpp::String outEncounterId;
	oatpp::String outBusNo;

	// 调用DAO插入入院登记
	AdmissionDAO dao;
	return dao.insertAdmission(
		name, idCard, birthTime, age, sex, phone,
		admissionDept, admissionWard, attendingDoctor,
		admissionDiagnosis, diagnosisDesc, patientCondition,
		admissionType, admissionMode, feeNature, admissionDate,
		prepayAmount, payWay, tenantId, orgId, operUserId,
		outPatientId, outEncounterId, outBusNo
	);
}

// 取消入院登记
bool AdmissionService::deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// 校验必填字段
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return false;

	// 从删除DTO中提取就诊ID
	// delete字段中包含要取消的就诊ID
	auto encounterId = dto->thedelete;
	auto cancelReason = oatpp::String("Manual cancellation");

	// 从payload获取认证信息
	auto tenantId = oatpp::String(payload.getId());
	auto operUserId = oatpp::String(payload.getId());

	// 调用DAO删除（取消）入院登记
	AdmissionDAO dao;
	return dao.deleteAdmission(encounterId, cancelReason, operUserId, tenantId);
}
