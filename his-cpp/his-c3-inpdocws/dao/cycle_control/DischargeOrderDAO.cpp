/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 14:26:52

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
#include "DischargeOrderDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

string DischargeOrderDAO::insertDischargeOrder(const CreateDischargeOrderDTO::Wrapper& dto)
{
	UuidFacade uf;
	string id = uf.genUuid();
	string activityId = uf.genUuid();
	string now = SimpleDateTimeFormat::format();

	string sql = "INSERT INTO wkf_service_request"
		" (id, tenant_id, created_at, updated_at, created_by, updated_by,"
		" is_deleted, version, status_enum, category_enum,"
		" patient_id, encounter_id, requester_id, activity_id,"
		" quantity, unit_code, authored_time, org_id, reason_text)"
		" VALUES (?, 1, ?, ?, '1', '1',"
		" 0, 0, 'ACTIVE', 'DISCHARGE',"
		" (SELECT patient_id FROM adm_encounter WHERE id = ?), ?,"
		" (SELECT id FROM whale_users LIMIT 1), ?,"
		" 1, 'ci', ?,"
		" (SELECT organization_id FROM adm_encounter WHERE id = ?), ?)";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, id);
	SQLPARAMS_PUSH(params, "s", string, now);
	SQLPARAMS_PUSH(params, "s", string, now);
	SQLPARAMS_PUSH(params, "s", string, dto->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", string, dto->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", string, activityId);
	SQLPARAMS_PUSH(params, "s", string, now);
	SQLPARAMS_PUSH(params, "s", string, dto->encounterId.getValue(""));
	SQLPARAMS_PUSH(params, "s", string, dto->dischargeWay.getValue(""));

	int rows = sqlSession->executeUpdate(sql, params);
	return rows > 0 ? id : "";
}

int DischargeOrderDAO::updateEncounterStatus(const string& encounterId)
{
	string sql = "UPDATE adm_encounter SET status_enum = 'ON_HOLD' WHERE id = ? AND is_deleted = 0";
	return sqlSession->executeUpdate(sql, "%s", encounterId);
}
