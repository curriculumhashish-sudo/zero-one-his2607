/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "MedicalOrderVerificationDAO.h"
#include "MedicalOrderVerificationMapper.h"

namespace
{
class VerificationStringMapper : public Mapper<std::string>
{
public:
	std::string mapper(ResultSet* resultSet) const override
	{
		return resultSet->getString(1);
	}
};

class VerificationWriteItemMapper : public Mapper<VerificationWriteItemDTO::Wrapper>
{
public:
	VerificationWriteItemDTO::Wrapper mapper(ResultSet* resultSet) const override
	{
		auto item = VerificationWriteItemDTO::createShared();
		item->orderType = "MED_REQUEST";
		item->requestId = resultSet->getString("request_id").asStdString();
		item->encounterId = resultSet->getString("encounter_id").asStdString();
		item->expectedVersion = static_cast<v_int64>(resultSet->getInt64("version"));
		return item;
	}
};

std::string verificationUnionSql()
{
	return "SELECT * FROM ("
		"SELECT mr.id request_id,'MED_REQUEST' order_type,p.name patient_name,"
		"IF(mr.status_enum='VERIFIED','true','false') is_passed,"
		"COALESCE(md.name,mr.content_json,mr.remark,'') content,"
		"COALESCE(mr.therapy_enum,'') therapy_enum,mr.status_enum,mr.patient_id,"
		"mr.encounter_id,mr.location_id,mr.version,"
		"COALESCE(md.name,mr.content_json,mr.remark,'') advice_display_name,"
		"COALESCE(md.total_volume,'') specification_text,"
		"CONCAT(COALESCE(CAST(mr.dose AS CHAR),''),COALESCE(mr.dose_unit_code,'')) dosage_display,"
		"COALESCE(mr.method_code,'') usage_display,COALESCE(mr.rate_code,'') frequency_display,"
		"COALESCE(mr.req_authored_time,'') authored_time,COALESCE(mr.effective_dose_end,'') stop_time,"
		"COALESCE(pr.name,'') requester_name,COALESCE(loc.name,'') performer_location_display,"
		"IF(COALESCE(mr.tcm_flag,0)=1,'TCM','WEST') medicine_category,"
		"COALESCE(CAST(mr.group_id AS SIGNED),0) group_no,COALESCE(mr.sort_number,0) sort_number,"
		"COALESCE(mr.tcm_flag,0) tcm_flag,COALESCE(mr.prescription_no,'') prescription_no,"
		"COALESCE(mr.suffering_flag,0) suffering_flag,"
		"COALESCE(mr.chinese_herbs_dose_quantity,0) chinese_herbs_dose_quantity "
		"FROM med_medication_request mr JOIN adm_patient p ON p.id=mr.patient_id AND p.is_deleted=0 "
		"LEFT JOIN med_medication_definition md ON md.id=mr.medication_id AND md.is_deleted=0 "
		"LEFT JOIN adm_practitioner pr ON pr.id=mr.requester_id AND pr.is_deleted=0 "
		"LEFT JOIN adm_location loc ON loc.id=mr.location_id AND loc.is_deleted=0 "
		"WHERE mr.is_deleted=0 AND COALESCE(mr.generate_source_enum,'')<>'NURSE_PRICING' UNION ALL "
		"SELECT sr.id,'SERVICE_REQUEST',p.name,IF(sr.status_enum='VERIFIED','true','false'),"
		"COALESCE(ad.name,sr.entrust_content,sr.remark,''),COALESCE(sr.therapy_enum,''),"
		"sr.status_enum,sr.patient_id,sr.encounter_id,sr.location_id,sr.version,"
		"COALESCE(ad.name,sr.entrust_content,sr.remark,''),COALESCE(ad.description_text,''),"
		"CONCAT(COALESCE(CAST(sr.quantity AS CHAR),''),COALESCE(sr.unit_code,'')),"
		"COALESCE(sr.entrust_content,''),COALESCE(sr.rate_code,''),COALESCE(sr.authored_time,''),"
		"COALESCE(sr.occurrence_end_time,''),COALESCE(pr.name,''),COALESCE(loc.name,''),'WEST',"
		"COALESCE(CAST(sr.group_id AS SIGNED),0),COALESCE(sr.sort_number,0),0,"
		"COALESCE(sr.prescription_no,''),0,0 "
		"FROM wkf_service_request sr JOIN adm_patient p ON p.id=sr.patient_id AND p.is_deleted=0 "
		"LEFT JOIN wkf_activity_definition ad ON ad.id=sr.activity_id AND ad.is_deleted=0 "
		"LEFT JOIN adm_practitioner pr ON pr.id=sr.requester_id AND pr.is_deleted=0 "
		"LEFT JOIN adm_location loc ON loc.id=sr.location_id AND loc.is_deleted=0 "
		"WHERE sr.is_deleted=0 AND COALESCE(sr.generate_source_enum,'')<>'NURSE_PRICING') x WHERE 1=1";
}

std::string bindMarkers(std::size_t count)
{
	std::string out;
	for (std::size_t index = 0; index < count; ++index) {
		if (index > 0) out += ',';
		out += '?';
	}
	return out;
}
}

std::list<MedicalOrderVerificationDetailDTO::Wrapper> MedicalOrderVerificationDAO::selectItems(const MedicalVerificationQuery::Wrapper& q)
{
	std::string sql=verificationUnionSql();
	SqlParams p;
	if(q->name&&!q->name->empty()){sql+=" AND x.patient_name LIKE CONCAT('%',?,'%')";SQLPARAMS_PUSH(p,"s",std::string,q->name.getValue(""));}
	if(q->isPassed&&!q->isPassed->empty()){sql+=q->isPassed.getValue("")=="true"?" AND x.status_enum='VERIFIED'":" AND x.status_enum='ACTIVE'";}
	if(q->statusEnum&&!q->statusEnum->empty()){sql+=" AND x.status_enum=?";SQLPARAMS_PUSH(p,"s",std::string,q->statusEnum.getValue(""));}
	if(q->medicalType&&!q->medicalType->empty()){sql+=" AND x.order_type=?";SQLPARAMS_PUSH(p,"s",std::string,q->medicalType.getValue(""));}
	if(q->orderType&&!q->orderType->empty()){sql+=" AND x.order_type=?";SQLPARAMS_PUSH(p,"s",std::string,q->orderType.getValue(""));}
	if(q->medicalPeriod&&!q->medicalPeriod->empty()){sql+=" AND x.therapy_enum=?";SQLPARAMS_PUSH(p,"s",std::string,q->medicalPeriod.getValue(""));}
	if(q->therapyEnum&&!q->therapyEnum->empty()){sql+=" AND x.therapy_enum=?";SQLPARAMS_PUSH(p,"s",std::string,q->therapyEnum.getValue(""));}
	if(q->patientId&&!q->patientId->empty()){sql+=" AND x.patient_id=?";SQLPARAMS_PUSH(p,"s",std::string,q->patientId.getValue(""));}
	if(q->encounterId&&!q->encounterId->empty()){sql+=" AND x.encounter_id=?";SQLPARAMS_PUSH(p,"s",std::string,q->encounterId.getValue(""));}
	if(q->locationId&&!q->locationId->empty()){sql+=" AND x.location_id=?";SQLPARAMS_PUSH(p,"s",std::string,q->locationId.getValue(""));}
	sql+=" ORDER BY x.patient_name,x.request_id";
	return sqlSession->executeQuery<MedicalOrderVerificationDetailDTO::Wrapper>(sql,MedicalOrderVerificationMapper(),p);
}

std::list<MedicalOrderVerificationDetailDTO::Wrapper>
MedicalOrderVerificationDAO::selectWorkbenchItems(
	const MedicalVerificationWorkbenchQueryDTO::Wrapper& q)
{
	SqlParams params;
	std::string sql = verificationUnionSql();
	sql += " AND x.encounter_id IN (" + bindMarkers(q->encounterIds->size()) + ")";
	for (const auto& encounterId : *q->encounterIds)
		SQLPARAMS_PUSH(params, "s", std::string, encounterId.getValue(""));
	const std::string status = q->auditStatus.getValue("");
	if (status == "PENDING") sql += " AND x.status_enum='ACTIVE'";
	else if (status == "AUDITED") sql += " AND x.status_enum='VERIFIED'";
	else if (status == "STOPPED") sql += " AND x.status_enum IN ('STOPPED','COMPLETED')";
	else if (status == "VOIDED") sql += " AND x.status_enum IN ('CANCELLED','REJECTED','DRAFT')";
	if (q->therapyType != "ALL") {
		sql += " AND x.therapy_enum=?";
		SQLPARAMS_PUSH(params, "s", std::string, q->therapyType.getValue(""));
	}
	if (q->medicineCategory == "TCM") sql += " AND x.medicine_category='TCM'";
	else sql += " AND x.medicine_category='WEST'";
	sql += " ORDER BY x.encounter_id,x.authored_time,x.group_no,x.sort_number,x.request_id";
	return sqlSession->executeQuery<MedicalOrderVerificationDetailDTO::Wrapper>(
		sql, MedicalOrderVerificationMapper(), params);
}

std::string MedicalOrderVerificationDAO::selectEncounterId(
	const std::string& type, const std::string& id)
{
	const std::string table = type == "MED_REQUEST" ? "med_medication_request" :
		(type == "SERVICE_REQUEST" ? "wkf_service_request" : "");
	if (table.empty()) return "";
	return sqlSession->executeQueryOne<std::string>(
		"SELECT encounter_id FROM " + table + " WHERE id=? AND is_deleted=0 LIMIT 1",
		VerificationStringMapper(), "%s", id);
}

std::list<VerificationWriteItemDTO::Wrapper>
MedicalOrderVerificationDAO::selectActiveTcmPrescriptionLines(
	const std::string& requestId, const std::string& encounterId)
{
	return sqlSession->executeQuery<VerificationWriteItemDTO::Wrapper>(R"SQL(
		SELECT line.id AS request_id,line.encounter_id,line.version
		FROM med_medication_request seed
		JOIN med_medication_request line
			ON line.encounter_id=seed.encounter_id
			AND line.prescription_no=seed.prescription_no
			AND COALESCE(line.tcm_flag,0)=1
			AND line.is_deleted=0
		WHERE seed.id=? AND seed.encounter_id=? AND seed.is_deleted=0
			AND COALESCE(seed.tcm_flag,0)=1
			AND COALESCE(seed.prescription_no,'')<>''
			AND line.status_enum='ACTIVE'
			AND COALESCE(line.generate_source_enum,'')<>'NURSE_PRICING'
		ORDER BY line.sort_number,line.id
	)SQL", VerificationWriteItemMapper(), "%s%s", requestId, encounterId);
}

bool MedicalOrderVerificationDAO::hasWriteState(
	const std::string& type, const std::string& id,
	const std::string& encounterId, const std::string& status,
	const std::string& operatorId)
{
	const std::string table = type == "MED_REQUEST" ? "med_medication_request" :
		(type == "SERVICE_REQUEST" ? "wkf_service_request" : "");
	if (table.empty()) return false;
	return sqlSession->executeQueryNumerical(
		"SELECT COUNT(*) FROM " + table + " WHERE id=? AND encounter_id=? "
		"AND status_enum=? AND performer_check_id=? AND is_deleted=0",
		"%s%s%s%s", id, encounterId, status, operatorId) == 1;
}

int MedicalOrderVerificationDAO::verify(
	const std::string& type,const std::string& id,const std::string& encounterId,
	const oatpp::Int64& expectedVersion,const std::string& op)
{
	std::string table=type=="MED_REQUEST"?"med_medication_request":(type=="SERVICE_REQUEST"?"wkf_service_request":""); if(table.empty())return 0;
	SqlParams p;
	SQLPARAMS_PUSH(p,"s",std::string,op);
	SQLPARAMS_PUSH(p,"s",std::string,op);
	SQLPARAMS_PUSH(p,"s",std::string,id);
	SQLPARAMS_PUSH(p,"s",std::string,encounterId);
	std::string sql="UPDATE "+table+" SET status_enum='VERIFIED',performer_check_id=?,check_time=NOW(),updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND encounter_id=? AND is_deleted=0 AND status_enum='ACTIVE'";
	if (expectedVersion.getPtr()) {
		sql += " AND version=?";
		SQLPARAMS_PUSH(p,"ll",int64_t,expectedVersion.getValue(0));
	}
	return sqlSession->executeUpdate(sql,p);
}
int MedicalOrderVerificationDAO::reject(
	const std::string& type,const std::string& id,const std::string& encounterId,
	const oatpp::Int64& expectedVersion,const std::string& reason,const std::string& op)
{
	std::string table=type=="MED_REQUEST"?"med_medication_request":(type=="SERVICE_REQUEST"?"wkf_service_request":""); if(table.empty())return 0;
	std::string reasonCol=type=="MED_REQUEST"?"remark":"reason_text";
	SqlParams p;
	SQLPARAMS_PUSH(p,"s",std::string,reason);
	SQLPARAMS_PUSH(p,"s",std::string,op);
	SQLPARAMS_PUSH(p,"s",std::string,op);
	SQLPARAMS_PUSH(p,"s",std::string,id);
	SQLPARAMS_PUSH(p,"s",std::string,encounterId);
	std::string sql="UPDATE "+table+" SET status_enum='DRAFT',"+reasonCol+"=?,performer_check_id=?,check_time=NOW(),updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND encounter_id=? AND is_deleted=0 AND status_enum='ACTIVE'";
	if (expectedVersion.getPtr()) {
		sql += " AND version=?";
		SQLPARAMS_PUSH(p,"ll",int64_t,expectedVersion.getValue(0));
	}
	return sqlSession->executeUpdate(sql,p);
}

std::list<std::string> MedicalOrderVerificationDAO::selectIdempotencyResult(
	const std::string& operation, const std::string& idempotencyKey)
{
	return sqlSession->executeQuery<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_ORDER_VERIFICATION'
			AND entity_id=? AND operation_type=?
		ORDER BY created_at DESC LIMIT 1
	)SQL", VerificationStringMapper(), "%s%s", idempotencyKey, operation);
}

int MedicalOrderVerificationDAO::insertIdempotencyResult(
	const std::string& auditId, const std::string& operation,
	const std::string& idempotencyKey, const std::string& operatorId,
	const std::string& fingerprint, const std::string& resultData)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(?,'NURSE_STATION_ORDER_VERIFICATION',?,?,?,?,NOW())
	)SQL", "%s%s%s%s%s", auditId, idempotencyKey, operation, operatorId,
		fingerprint + "\n--RESULT--\n" + resultData);
}
