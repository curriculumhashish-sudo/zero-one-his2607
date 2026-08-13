/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/12 00:00:00

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
#include "NurseStationAccessDAO.h"

namespace
{
	class AccessStringMapper : public Mapper<std::string>
	{
	public:
		std::string mapper(ResultSet* resultSet) const override
		{
			return resultSet->getString(1);
		}
	};
}

bool NurseStationAccessDAO::canAccessEncounter(
	const std::string& userId, const std::string& encounterId)
{
	return sqlSession->executeQueryNumerical(R"SQL(
		SELECT COUNT(DISTINCT e.id)
		FROM adm_encounter e
		LEFT JOIN adm_encounter_location el
			ON el.encounter_id=e.id AND el.form_enum='BED'
			AND el.status_enum='ACTIVE' AND el.is_deleted=0
		LEFT JOIN adm_location bed ON bed.id=el.location_id AND bed.is_deleted=0
		LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0
		LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0
		JOIN adm_pr_assign a
			ON (a.location_id IN (bed.id,house.id,ward.id) OR a.org_id=e.organization_id)
			AND a.is_deleted=0 AND a.active_flag=1
		JOIN adm_practitioner p
			ON p.id=a.practitioner_id AND p.is_deleted=0 AND p.active_flag=1
		WHERE e.id=? AND e.class_enum='IMP' AND e.is_deleted=0 AND p.user_id=?
	)SQL", "%s%s", encounterId, userId) > 0;
}

bool NurseStationAccessDAO::canAccessBed(const std::string& userId, const std::string& bedId)
{
	return sqlSession->executeQueryNumerical(R"SQL(
		SELECT COUNT(DISTINCT bed.id)
		FROM adm_location bed
		LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0
		LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0
		JOIN adm_pr_assign a
			ON a.location_id IN (bed.id,house.id,ward.id)
			AND a.is_deleted=0 AND a.active_flag=1
		JOIN adm_practitioner p
			ON p.id=a.practitioner_id AND p.is_deleted=0 AND p.active_flag=1
		WHERE bed.id=? AND bed.form_enum='BED' AND bed.is_deleted=0 AND p.user_id=?
	)SQL", "%s%s", bedId, userId) > 0;
}

bool NurseStationAccessDAO::canAccessEncounterPatient(
	const std::string& userId,const std::string& encounterId,
	const std::string& patientId)
{
	return canAccessEncounter(userId,encounterId)
		&& sqlSession->executeQueryNumerical(
			"SELECT COUNT(*) FROM adm_encounter WHERE id=? AND patient_id=? "
			"AND class_enum='IMP' AND is_deleted=0",
			"%s%s",encounterId,patientId)==1;
}

bool NurseStationAccessDAO::canAccessMedicationSummary(
	const std::string& userId, const std::string& summaryNo)
{
	auto encounterIds = sqlSession->executeQuery<std::string>(R"SQL(
		SELECT DISTINCT encounter_id
		FROM med_medication_dispense
		WHERE summary_no=? AND is_deleted=0
	)SQL", AccessStringMapper(), "%s", summaryNo);
	if (encounterIds.empty()) return false;
	for (const auto& encounterId : encounterIds)
		if (!canAccessEncounter(userId, encounterId)) return false;
	return true;
}

bool NurseStationAccessDAO::ownsMedicationIdempotency(
	const std::string& userId, const std::string& idempotencyKey,
	const std::string& operation)
{
	return sqlSession->executeQueryNumerical(R"SQL(
		SELECT COUNT(*) FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_MEDICATION_COLLECTION'
			AND entity_id=? AND operation_type=? AND user_id=?
	)SQL", "%s%s%s", idempotencyKey, operation, userId) > 0;
}
