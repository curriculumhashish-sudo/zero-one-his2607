#pragma once
#ifndef _PATIENT_DAO_H_
#define _PATIENT_DAO_H_

#include "BaseDAO.h"
#include "../../domain/do/pat/PatientDO.h"
#include "domain/query/pat/PatSelectPageQuery.h"
#include "domain/query/pat/PatPageQuery.h"
#include <list>

/**
 * Patient data access.
 */
class PatientDAO : public BaseDAO
{
public:
	/**
	 * Query paginated patient rows for select popup.
	 */
	std::list<PtrPatientDO> selectPatList(const PatSelectPageQuery::Wrapper& query);

	/**
	 * Count patients matching select popup filters.
	 */
	uint64_t countPatList(const PatSelectPageQuery::Wrapper& query);

	/**
	 * Query paginated patient rows for management list (GET /pat/page).
	 */
	std::list<PtrPatientDO> queryPatPage(const PatPageQuery::Wrapper& query);

	/**
	 * Count patients matching management list filters.
	 */
	uint64_t countPatPage(const PatPageQuery::Wrapper& query);

	// ============================================================
	// 软删除患者 — 设置is_deleted=1，记录删除人和删除时间
	// SQL: UPDATE adm_patient SET is_deleted=1, deleted_at=?, deleted_by=?, updated_at=?, updated_by=? WHERE id=? AND is_deleted=0
	// 返回: 受影响行数（1=成功, 0=记录不存在或已被删除）
	// ============================================================
	int softDeletePatient(const std::string& id,
		const std::string& deletedAt,
		const std::string& deletedBy);

private:
	/**
	 * Build WHERE clause and bind params for select popup keyword search.
	 */
	std::string buildSelectCondition(const PatSelectPageQuery::Wrapper& query, SqlParams& params);

	/**
	 * Build WHERE clause and bind params for management list (name / id card fuzzy match).
	 */
	std::string buildPatPageCondition(const PatPageQuery::Wrapper& query, SqlParams& params);
};

#endif
