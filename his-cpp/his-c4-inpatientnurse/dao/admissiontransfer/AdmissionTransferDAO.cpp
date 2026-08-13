#include "stdafx.h"
#include "AdmissionTransferDAO.h"
#include "id/UuidFacade.h"
namespace { class StringMapper:public Mapper<std::string>{public:std::string mapper(ResultSet* r)const override{return r->getString(1);}};class DoubleMapper:public Mapper<double>{public:double mapper(ResultSet* r)const override{return r->getDouble(1);}}; }
std::string AdmissionTransferDAO::selectEncounterStatus(const std::string& id){return sqlSession->executeQueryOne<std::string>("SELECT status_enum FROM adm_encounter WHERE id=? AND class_enum='IMP' AND is_deleted=0",StringMapper(),"%s",id);}
std::string AdmissionTransferDAO::selectEncounterStatusForUpdate(const std::string& id){return sqlSession->executeQueryOne<std::string>("SELECT status_enum FROM adm_encounter WHERE id=? AND class_enum='IMP' AND is_deleted=0 FOR UPDATE",StringMapper(),"%s",id);}
uint64_t AdmissionTransferDAO::countUnstoppedLongOrders(const std::string& id){return sqlSession->executeQueryNumerical("SELECT (SELECT COUNT(*) FROM med_medication_request WHERE encounter_id=? AND is_deleted=0 AND therapy_enum='LONG_TERM' AND status_enum IN('ACTIVE','VERIFIED'))+(SELECT COUNT(*) FROM wkf_service_request WHERE encounter_id=? AND is_deleted=0 AND therapy_enum='LONG_TERM' AND status_enum IN('ACTIVE','VERIFIED'))","%s%s",id,id);}
uint64_t AdmissionTransferDAO::countUnfinishedTemporaryOrders(const std::string& id){return sqlSession->executeQueryNumerical("SELECT (SELECT COUNT(*) FROM med_medication_request WHERE encounter_id=? AND is_deleted=0 AND therapy_enum='TEMPORARY' AND status_enum IN('ACTIVE','VERIFIED'))+(SELECT COUNT(*) FROM wkf_service_request WHERE encounter_id=? AND is_deleted=0 AND therapy_enum='TEMPORARY' AND status_enum IN('ACTIVE','VERIFIED'))","%s%s",id,id);}
uint64_t AdmissionTransferDAO::countPendingMedicationDispenses(const std::string& id){return sqlSession->executeQueryNumerical("SELECT COUNT(*) FROM med_medication_dispense WHERE encounter_id=? AND is_deleted=0 AND status_enum IN('PREPARATION','IN_PROGRESS','PENDING')","%s",id);}
double AdmissionTransferDAO::selectDisplayBalance(const std::string& id){return sqlSession->executeQueryOne<double>("SELECT COALESCE(SUM(total_price),0) FROM adm_charge_item WHERE encounter_id=? AND is_deleted=0 AND status_enum='PENDING_SETTLEMENT'",DoubleMapper(),"%s",id);}
std::string AdmissionTransferDAO::selectActiveBedId(const std::string& id){return sqlSession->executeQueryOne<std::string>("SELECT location_id FROM adm_encounter_location WHERE encounter_id=? AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0 LIMIT 1",StringMapper(),"%s",id);}
std::string AdmissionTransferDAO::selectActiveBedIdForUpdate(const std::string& id){return sqlSession->executeQueryOne<std::string>("SELECT location_id FROM adm_encounter_location WHERE encounter_id=? AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0 LIMIT 1 FOR UPDATE",StringMapper(),"%s",id);}
int AdmissionTransferDAO::markPendingDischarge(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter SET status_enum='PENDING',end_time=COALESCE(end_time,NOW()),updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND class_enum='IMP' AND is_deleted=0 AND status_enum='ON_HOLD'","%s%s",op,id);}
int AdmissionTransferDAO::markSettlementCompleted(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter SET status_enum='COMPLETED',updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND class_enum='IMP' AND is_deleted=0 AND status_enum='PENDING' AND NOT EXISTS(SELECT 1 FROM adm_charge_item ci WHERE ci.encounter_id=adm_encounter.id AND ci.is_deleted=0 AND ci.status_enum='PENDING_SETTLEMENT')","%s%s",op,id);}
int AdmissionTransferDAO::closeBedTrack(const std::string& id,const std::string& bed,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter_location SET status_enum='COMPLETED',end_time=NOW(),updated_at=NOW(),updated_by=?,version=version+1 WHERE encounter_id=? AND location_id=? AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0","%s%s%s",op,id,bed);}
int AdmissionTransferDAO::releaseBed(const std::string& bed,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_location SET operational_enum=CASE WHEN operational_enum='IN_USE' THEN 'NOT_IN_USE' ELSE 'VACANT' END,updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND form_enum='BED' AND operational_enum IN('OCCUPIED','IN_USE') AND is_deleted=0","%s%s",op,bed);}
int AdmissionTransferDAO::markDischarged(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter SET status_enum='DISCHARGED',end_time=COALESCE(end_time,NOW()),updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND class_enum='IMP' AND is_deleted=0 AND status_enum='COMPLETED'","%s%s",op,id);}

namespace {
std::string encounterStatusText(const std::string& status)
{
	if (status == "REGISTERED")
		return ZH_WORDS_GETTER("business-text.enum.encounter-registered");
	if (status == "IN_PROGRESS")
		return ZH_WORDS_GETTER("business-text.enum.encounter-in-hospital");
	if (status == "ON_HOLD")
		return ZH_WORDS_GETTER("business-text.enum.encounter-pending-discharge");
	if (status == "PENDING")
		return ZH_WORDS_GETTER("business-text.enum.encounter-pending-settlement");
	if (status == "COMPLETED")
		return ZH_WORDS_GETTER("business-text.enum.encounter-settled");
	if (status == "DISCHARGED")
		return ZH_WORDS_GETTER("business-text.enum.encounter-discharged");
	return ZH_WORDS_GETTER("business-text.enum.unknown");
}

class BedBoardMapper : public Mapper<BedBoardItemDTO::Wrapper>
{
public:
	BedBoardItemDTO::Wrapper mapper(ResultSet* r) const override
	{
		auto item = BedBoardItemDTO::createShared();
		item->bed = BedBoardBedDTO::createShared();
		item->bed->bedId = r->getString(1).asStdString();
		item->bed->bedName = r->getString(2).asStdString();
		item->bed->houseId = r->getString(3).asStdString();
		item->bed->houseName = r->getString(4).asStdString();
		item->bed->wardId = r->getString(5).asStdString();
		item->bed->wardName = r->getString(6).asStdString();
		item->bed->displayOrder = r->getInt(7);
		item->bed->operationalEnum = r->getString(8).asStdString();
		item->bed->version = r->getInt64(9);
		const auto encounter = r->getString(10).asStdString();
		if (!encounter.empty())
		{
			item->occupant = BedBoardOccupantDTO::createShared();
			item->occupant->encounterId = encounter;
			item->occupant->patientId = r->getString(11).asStdString();
			item->occupant->patientName = r->getString(12).asStdString();
			const auto gender = r->getString(13).asStdString();
			item->occupant->genderEnumText = gender == "FEMALE"
				? ZH_WORDS_GETTER("business-text.enum.gender-female")
				: (gender == "MALE"
					? ZH_WORDS_GETTER("business-text.enum.gender-male")
					: ZH_WORDS_GETTER("business-text.enum.unknown"));
			item->occupant->ageText = r->getString(14).asStdString()
				+ ZH_WORDS_GETTER("business-text.encounter.age-suffix");
			const auto status = r->getString(15).asStdString();
			item->occupant->statusEnum = status;
			item->occupant->statusEnumText = encounterStatusText(status);
		}
		return item;
	}
};
class PendingMapper:public Mapper<DischargePendingItemDTO::Wrapper>{public:DischargePendingItemDTO::Wrapper mapper(ResultSet* r)const override{auto d=DischargePendingItemDTO::createShared();d->requestId=r->getString(1).asStdString();d->requestType=r->getString(2).asStdString();d->statusEnum=r->getString(3).asStdString();d->content=r->getString(4).asStdString();return d;}};
}
std::list<BedBoardItemDTO::Wrapper> AdmissionTransferDAO::selectBedBoard(
	const std::string& panel,const std::string& wardIds,
	const std::string& houseIds,const std::string& userId)
{
	std::string sql="SELECT bed.id,bed.name,house.id,house.name,ward.id,ward.name,COALESCE(bed.display_order,0),bed.operational_enum,bed.version,e.id,e.patient_id,p.name,p.gender_enum,TIMESTAMPDIFF(YEAR,p.birth_date,CURDATE()),e.status_enum FROM adm_location bed LEFT JOIN adm_location house ON house.id=bed.parent_id AND house.is_deleted=0 LEFT JOIN adm_location ward ON ward.id=house.parent_id AND ward.is_deleted=0 LEFT JOIN adm_encounter_location el ON el.location_id=bed.id AND el.form_enum='BED' AND el.status_enum='ACTIVE' AND el.is_deleted=0 LEFT JOIN adm_encounter e ON e.id=el.encounter_id AND e.is_deleted=0 LEFT JOIN adm_patient p ON p.id=e.patient_id AND p.is_deleted=0 WHERE bed.form_enum='BED' AND bed.status_enum IN('ENABLE','ACTIVE') AND bed.is_deleted=0";
	SqlParams ps;
	sql+=" AND EXISTS(SELECT 1 FROM adm_practitioner access_practitioner JOIN adm_pr_assign access_assignment ON access_assignment.practitioner_id=access_practitioner.id AND access_assignment.is_deleted=0 AND access_assignment.active_flag=1 WHERE access_practitioner.user_id=? AND access_practitioner.is_deleted=0 AND access_practitioner.active_flag=1 AND access_assignment.location_id IN(bed.id,house.id,ward.id))";
	SQLPARAMS_PUSH(ps,"s",std::string,userId);
	if(panel=="DISCHARGE")sql+=" AND e.id IS NOT NULL AND e.status_enum IN('ON_HOLD','PENDING','COMPLETED')";
	if(!wardIds.empty()){sql+=" AND FIND_IN_SET(ward.id,?)";SQLPARAMS_PUSH(ps,"s",std::string,wardIds);}
	if(!houseIds.empty()){sql+=" AND FIND_IN_SET(house.id,?)";SQLPARAMS_PUSH(ps,"s",std::string,houseIds);}
	sql+=" ORDER BY ward.display_order,house.display_order,bed.display_order,bed.name";
	return sqlSession->executeQuery<BedBoardItemDTO::Wrapper>(sql,BedBoardMapper(),ps);
}
std::list<DischargePendingItemDTO::Wrapper> AdmissionTransferDAO::selectPendingExecutions(const std::string& id){return sqlSession->executeQuery<DischargePendingItemDTO::Wrapper>("SELECT id,'MEDICATION',status_enum,COALESCE(content_json,remark,'') FROM med_medication_request WHERE encounter_id=? AND is_deleted=0 AND status_enum IN('ACTIVE','VERIFIED') UNION ALL SELECT id,'SERVICE',status_enum,COALESCE(entrust_content,remark,'') FROM wkf_service_request WHERE encounter_id=? AND is_deleted=0 AND status_enum IN('ACTIVE','VERIFIED')",PendingMapper(),"%s%s",id,id);}
std::list<DischargePendingItemDTO::Wrapper> AdmissionTransferDAO::selectPendingMedications(const std::string& id){return sqlSession->executeQuery<DischargePendingItemDTO::Wrapper>("SELECT id,'DISPENSE',status_enum,COALESCE(dosage_instruction,'') FROM med_medication_dispense WHERE encounter_id=? AND is_deleted=0 AND status_enum IN('PREPARATION','IN_PROGRESS','PENDING')",PendingMapper(),"%s",id);}
int AdmissionTransferDAO::occupyVacantBed(const std::string& bed,int64_t ver,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_location SET operational_enum=CASE WHEN operational_enum='NOT_IN_USE' THEN 'IN_USE' ELSE 'OCCUPIED' END,updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND form_enum='BED' AND status_enum IN('ENABLE','ACTIVE') AND operational_enum IN('VACANT','AVAILABLE','NOT_IN_USE') AND version=? AND is_deleted=0","%s%s%lld",op,bed,ver);}
int AdmissionTransferDAO::releaseCurrentBed(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_location l JOIN adm_encounter_location el ON el.location_id=l.id SET l.operational_enum=CASE WHEN l.operational_enum='IN_USE' THEN 'NOT_IN_USE' ELSE 'VACANT' END,l.updated_at=NOW(),l.updated_by=?,l.version=l.version+1 WHERE el.encounter_id=? AND el.form_enum='BED' AND el.status_enum='ACTIVE' AND el.is_deleted=0 AND l.operational_enum IN('OCCUPIED','IN_USE')","%s%s",op,id);}
int AdmissionTransferDAO::closeCurrentBedTrack(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter_location SET status_enum='COMPLETED',end_time=NOW(),updated_at=NOW(),updated_by=?,version=version+1 WHERE encounter_id=? AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0","%s%s",op,id);}
int AdmissionTransferDAO::createBedTrack(const std::string& id,const std::string& bed,const std::string& op){return sqlSession->executeUpdate("INSERT INTO adm_encounter_location(id,encounter_id,location_id,form_enum,status_enum,start_time,created_at,created_by,updated_at,updated_by,version,is_deleted) VALUES(REPLACE(UUID(),'-',''),?,?,'BED','ACTIVE',NOW(),NOW(),?,NOW(),?,0,0)","%s%s%s%s",id,bed,op,op);}
int AdmissionTransferDAO::updateEncounterAfterAssignment(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter SET status_enum=IF(status_enum='REGISTERED','IN_PROGRESS',status_enum),updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND class_enum='IMP' AND is_deleted=0","%s%s",op,id);}
int AdmissionTransferDAO::recallDischarge(const std::string& id,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter SET status_enum='REGISTERED',end_time=NULL,updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND class_enum='IMP' AND is_deleted=0 AND status_enum IN('PENDING','COMPLETED','DISCHARGED')","%s%s",op,id);}
std::string AdmissionTransferDAO::selectBedHouseId(const std::string& bed){return sqlSession->executeQueryOne<std::string>("SELECT parent_id FROM adm_location WHERE id=? AND form_enum='BED' AND status_enum IN('ENABLE','ACTIVE') AND is_deleted=0",StringMapper(),"%s",bed);}
std::string AdmissionTransferDAO::selectBedHouseIdForUpdate(const std::string& bed){return sqlSession->executeQueryOne<std::string>("SELECT parent_id FROM adm_location WHERE id=? AND form_enum='BED' AND status_enum IN('ENABLE','ACTIVE') AND is_deleted=0 FOR UPDATE",StringMapper(),"%s",bed);}
int AdmissionTransferDAO::lockOccupiedBed(const std::string& bed,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_location SET updated_at=NOW(6),updated_by=? WHERE id=? AND form_enum='BED' AND operational_enum IN('OCCUPIED','IN_USE') AND is_deleted=0","%s%s",op,bed);}
int AdmissionTransferDAO::touchOccupiedBed(const std::string& bed,int64_t version,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_location SET updated_at=NOW(),updated_by=?,version=version+1 WHERE id=? AND form_enum='BED' AND operational_enum IN('OCCUPIED','IN_USE') AND version=? AND is_deleted=0","%s%s%lld",op,bed,version);}
int AdmissionTransferDAO::swapBedTracks(const std::string& encounter,const std::string& sourceBed,const std::string& swapEncounter,const std::string& targetBed,const std::string& op){return sqlSession->executeUpdate("UPDATE adm_encounter_location SET location_id=CASE WHEN encounter_id=? THEN ? WHEN encounter_id=? THEN ? ELSE location_id END,updated_at=NOW(),updated_by=?,version=version+1 WHERE encounter_id IN (?,?) AND form_enum='BED' AND status_enum='ACTIVE' AND is_deleted=0","%s%s%s%s%s%s%s",encounter,targetBed,swapEncounter,sourceBed,op,encounter,swapEncounter);}

int AdmissionTransferDAO::replaceEncounterParticipants(const std::string& encounterId,
	const oatpp::List<oatpp::Object<BedOperationParticipantDTO>>& participants,
	const std::string& operatorId)
{
	if (!participants || participants->empty()) return 0;
	std::set<std::string> roles;
	for (const auto& participant : *participants)
		roles.insert(participant->roleCode.getValue(""));
	for (const auto& role : roles)
	{
		if (sqlSession->executeUpdate(
			"UPDATE adm_encounter_participant SET status_enum='COMPLETED',"
			"end_time=NOW(6),updated_at=NOW(6),updated_by=?,version=version+1 "
			"WHERE encounter_id=? AND role_code=? AND status_enum='ACTIVE' AND is_deleted=0",
			"%s%s%s", operatorId, encounterId, role) < 0) return -1;
	}
	int inserted = 0;
	for (const auto& participant : *participants)
	{
		inserted += sqlSession->executeUpdate(R"SQL(
			INSERT INTO adm_encounter_participant(
				id,encounter_id,role_code,practitioner_id,status_enum,start_time,
				created_at,created_by,updated_at,updated_by,version,is_deleted)
			SELECT REPLACE(UUID(),'-',''),?,?,?,'ACTIVE',NOW(6),
				NOW(6),?,NOW(6),?,0,0
			WHERE EXISTS(SELECT 1 FROM adm_practitioner
				WHERE id=? AND is_deleted=0 AND active_flag=1)
		)SQL", "%s%s%s%s%s%s", encounterId,
			participant->roleCode.getValue(""),
			participant->practitionerId.getValue(""), operatorId, operatorId,
			participant->practitionerId.getValue(""));
	}
	return inserted;
}

int AdmissionTransferDAO::insertAdmissionVitalSigns(const std::string& encounterId,
	const oatpp::Object<AdmissionVitalSignDTO>& vitalSigns,
	const std::string& operatorId)
{
	if (!vitalSigns) return 0;
	const std::string vitalSignId = UuidFacade().genUuid();
	if (sqlSession->executeUpdate(R"SQL(
		INSERT INTO dia_vital_sign(
			id,patient_id,encounter_id,recorded_at,type_enum,bus_no,recorder_id,
			created_at,created_by,updated_at,updated_by,version,is_deleted)
		SELECT ?,e.patient_id,e.id,NOW(6),'NURSING_RECORD',
			CONCAT('ADMIT-',REPLACE(UUID(),'-','')),?,NOW(6),?,NOW(6),?,0,0
		FROM adm_encounter e WHERE e.id=? AND e.class_enum='IMP' AND e.is_deleted=0
	)SQL", "%s%s%s%s%s", vitalSignId, operatorId, operatorId, operatorId,
		encounterId) != 1) return -1;
	const std::pair<const char*, oatpp::String> details[] = {
		{"TEMP", vitalSigns->temperature}, {"PULSE", vitalSigns->pulse},
		{"HR", vitalSigns->heartRate}, {"BP_SYS", vitalSigns->bloodPressureHigh},
		{"BP_DIA", vitalSigns->bloodPressureLow}, {"HEIGHT", vitalSigns->heightCm},
		{"WEIGHT", vitalSigns->weightKg}
	};
	int inserted = 0;
	for (const auto& detail : details)
	{
		if (!detail.second || detail.second->empty()) continue;
		inserted += sqlSession->executeUpdate(R"SQL(
			INSERT INTO dia_vital_sign_detail(
				id,vital_sign_id,term_code,item_code,item_value,
				created_at,created_by,updated_at,updated_by,version,is_deleted)
			VALUES(REPLACE(UUID(),'-',''),?,'vital_signs_code',?,?,
				NOW(6),?,NOW(6),?,0,0)
		)SQL", "%s%s%s%s%s", vitalSignId, detail.first,
			detail.second.getValue(""), operatorId, operatorId);
	}
	return inserted;
}

std::string AdmissionTransferDAO::selectBedOperationIdempotency(
	const std::string& idempotencyKey)
{
	return sqlSession->executeQueryOne<std::string>(R"SQL(
		SELECT changes FROM whale_audit_logs
		WHERE entity_type='NURSE_STATION_BED_OPERATION'
			AND entity_id=? AND operation_type='EXECUTE'
		ORDER BY created_at DESC LIMIT 1
	)SQL", StringMapper(), "%s", idempotencyKey);
}

int AdmissionTransferDAO::insertBedOperationIdempotency(
	const std::string& idempotencyKey, const std::string& operatorId,
	const std::string& fingerprint, const std::string& resultData)
{
	return sqlSession->executeUpdate(R"SQL(
		INSERT INTO whale_audit_logs(
			id,entity_type,entity_id,operation_type,user_id,changes,created_at)
		VALUES(REPLACE(UUID(),'-',''),'NURSE_STATION_BED_OPERATION',
			?,'EXECUTE',?,?,NOW(6))
	)SQL", "%s%s%s", idempotencyKey, operatorId,
		fingerprint + "\n--RESULT--\n" + resultData);
}
