#include"GetTodayAppointDAO.h"
#include "GetTodayAppointMapper.h"

inline std::string GetTodayAppointDAO::queryConditionBuilder(const GetTodayAppointQuery::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1 AND DATE(s.start_time) = CURDATE()";
	if (query->doctor) {

		sqlCondition << " AND `doctor`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->doctor.getValue(""));
	}

	if (query->register_type) {

		sqlCondition << " AND `register_type`=?";
		SQLPARAMS_PUSH(params, "s", std::string, query->register_type.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t GetTodayAppointDAO::count(const GetTodayAppointQuery::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM wkf_slot s ";
	sql += "INNER JOIN wkf_schedule sch ON s.schedule_id = sch.id ";

	sql += "LEFT JOIN adm_location loc ON sch.organization_id = loc.id ";

	sql += "LEFT JOIN wkf_schedule_actor sa ON sch.id = sa.schedule_id AND sa.actor_type = 10 ";

	sql += "LEFT JOIN adm_practitioner p ON sa.actor_id = p.id ";

	sql += "LEFT JOIN adm_healthcare_service hs ON sch.healthcare_service_id = hs.id ";

	sql += "LEFT JOIN adm_charge_item_definition cid ON cid.type_code = hs.type_code AND cid.status_enum = \'ACTIVE\' AND cid.price_type_code = \'REGISTRATION\' ";
	sql += queryConditionBuilder(query, params);
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<GetTodayAppointDO> GetTodayAppointDAO::selectTodayWithPage(const GetTodayAppointQuery::Wrapper & query)
{
	SqlParams params;
	string sql = "SELECT s.id AS id, \
						 s.status_code AS status,\
						 s.capacity AS capacity,\
						 s.booked_count AS booked_count,\
						 hs.name AS register_type.\
						 loc.name AS organization,\
						 MIN(s.start_time) AS start_time,\
						 MAX(s.end_time) AS end_time\
						 FROM wkf_slot s ";

	sql += "INNER JOIN wkf_schedule sch ON s.schedule_id = sch.id ";

	sql += "LEFT JOIN adm_location loc ON sch.organization_id = loc.id ";

	sql += "LEFT JOIN wkf_schedule_actor sa ON sch.id = sa.schedule_id AND sa.actor_type = 10 ";

	sql += "LEFT JOIN adm_practitioner p ON sa.actor_id = p.id ";

	sql += "LEFT JOIN adm_healthcare_service hs ON sch.healthcare_service_id = hs.id ";

	sql += "LEFT JOIN adm_charge_item_definition cid ON cid.type_code = hs.type_code AND cid.status_enum = \'ACTIVE\' AND cid.price_type_code = \'REGISTRATION\' ";

	sql += queryConditionBuilder(query, params);


	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	return sqlSession->executeQuery<GetTodayAppointDO>(sql, GetTodayAppointMapper(), params);
}