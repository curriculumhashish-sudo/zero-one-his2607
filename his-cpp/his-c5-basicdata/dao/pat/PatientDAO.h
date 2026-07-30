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
