#pragma once
#ifndef _PAT_SERVICE_H_
#define _PAT_SERVICE_H_

#include "../../domain/query/pat/PatSelectPageQuery.h"
#include "../../domain/query/pat/PatPageQuery.h"
#include "../../domain/dto/pat/PatSelectDTO.h"
#include "../../domain/dto/pat/PatDTO.h"
#include "domain/dto/PageDTO.h"
#include "domain/dto/PayloadDTO.h"

/**
 * Patient business service.
 */
class PatService
{
public:
	/**
	 * Paginated query for select-patient popup.
	 */
	PageDTO<PatSelectDTO::Wrapper>::Wrapper querySelectPatPage(const PatSelectPageQuery::Wrapper& query);

	/**
	 * Paginated query for patient management list (GET /pat/page).
	 */
	PageDTO<PatDTO::Wrapper>::Wrapper queryPatPage(const PatPageQuery::Wrapper& query);

	// ============================================================
	// 软删除患者 — 设置is_deleted=1，不物理删除
	// 参数: id=患者ID, payload=JWT认证信息（用于记录操作人）
	// 返回: true=删除成功，false=记录不存在或已删除
	// 调用: PatientDAO::softDeletePatient
	// ============================================================
	bool removePatient(const std::string& id, const PayloadDTO& payload);

private:
	/** Convert gender_enum (MALE/FEMALE) to display text. */
	static std::string toGenderLabel(const std::string& genderEnum);
	/** Convert gender_enum (MALE/FEMALE) to numeric code (MALE->1, FEMALE->2, unknown->0). */
	static int toGenderCode(const std::string& genderEnum);
	/** Calculate age from birth_date (yyyy-MM-dd). Empty/invalid returns 0. */
	static int calcAge(const std::string& birthDate);
};

#endif
