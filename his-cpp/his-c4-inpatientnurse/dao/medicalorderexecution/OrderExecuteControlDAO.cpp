#include "stdafx.h"
#include "OrderExecuteControlDAO.h"
#include "ClnProcedureMapper.h"
#include "OriginalOrderRequestMapper.h"

PtrOriginalOrderRequestDO OrderExecuteControlDAO::queryOriginalOrderRequestById(const string& orderId, const string& adviceType)
{
	string table;
	if (adviceType == "MED_REQUEST")
		table = "med_medication_request";
	else if (adviceType == "SERVICE_REQUEST")
		table = "wkf_service_request";
	else
		return nullptr;

	const string querySql = R"SQL(
		SELECT
			encounter_id,
			patient_id,
			category_enum
		FROM )SQL" +
		table +
		" WHERE id = ? AND is_deleted = 0;";

	return sqlSession->executeQueryOne<PtrOriginalOrderRequestDO>(querySql, OriginalOrderRequestMapper(), "%s", orderId);
}

PtrClnProcedureDO OrderExecuteControlDAO::queryActiveTerminalProcedureBySlot(
	const string& requestId,
	const string& requestTable,
	const string& occurrenceTime)
{
	const string querySql = R"SQL(
		SELECT
			id,
			status_enum,
			occurrence_slot_active
		FROM cln_procedure
		WHERE request_id = ?
			AND request_table = ?
			AND occurrence_time = ?
			AND is_deleted = 0
			AND occurrence_slot_active = 1
			AND status_enum IN ('COMPLETED', 'STOPPED')
		LIMIT 1;
	)SQL";

	SqlParams params;
	SQLPARAMS_PUSH(params, "s", string, requestId);
	SQLPARAMS_PUSH(params, "s", string, requestTable);
	SQLPARAMS_PUSH(params, "dt", string, occurrenceTime);
	return sqlSession->executeQueryOne<PtrClnProcedureDO>(querySql, ClnProcedureSlotStateMapper(), params);
}

PtrClnProcedureDO OrderExecuteControlDAO::queryCompletedProcedureById(const string& procedureId)
{
	const string querySql = R"SQL(
		SELECT
			id,
			tenant_id,
			version,
			request_id,
			request_table,
			status_enum,
			category_enum,
			encounter_id,
			patient_id,
			occurrence_time,
			recorder_id,
			org_id,
			location_id,
			group_id,
			refund_id,
			occurrence_slot_active
		FROM cln_procedure
		WHERE id = ?
			AND status_enum = 'COMPLETED'
			AND is_deleted = 0
			AND occurrence_slot_active = 1
		LIMIT 1;
	)SQL";

	return sqlSession->executeQueryOne<PtrClnProcedureDO>(querySql, ClnProcedureCompletedMapper(), "%s", procedureId);
}

bool OrderExecuteControlDAO::existsCancelProcedureByRefundId(const string& refundId)
{
	const string querySql = R"SQL(
		SELECT COUNT(1)
		FROM cln_procedure
		WHERE refund_id = ?
			AND status_enum = 'CANCEL'
			AND is_deleted = 0;
	)SQL";

	return sqlSession->executeQueryNumerical(querySql, "%s", refundId) > 0;
}
