#include "stdafx.h"
#include "EnterOrdersDAO.h"
#include "EnterOrdersMapper.h"

//EnterOrders
//enter-orders
//ENTERORDERS

std::string EnterOrdersDAO::queryById(const EnterOrdersQueryById::Wrapper& query, SqlParams& params)
{
	stringstream sqlCondition;
	sqlCondition << " WHERE 1=1";
	if (query->tenant_id) {

		sqlCondition << " AND tenant_id LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->tenant_id.getValue(""));
	}
	if (query->patient_id) {

		sqlCondition << " AND patient_id LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->patient_id.getValue(""));
	}
	if (query->encounter_id) {

		sqlCondition << " AND encounter_id  LIKE CONCAT('%',?,'%')";
		SQLPARAMS_PUSH(params, "s", std::string, query->encounter_id.getValue(""));
	}
	return sqlCondition.str();
}

uint64_t EnterOrdersDAO::count(const EnterOrdersQueryById::Wrapper& query)
{
	SqlParams params;
	string sql = "SELECT COUNT(*) FROM med_medication_request ";
	// 构建查询条件
	sql += queryById(query, params);
	// 执行查询
	return sqlSession->executeQueryNumerical(sql, params);
}

std::list<EnterOrdersQueryDO> EnterOrdersDAO::selectWithPage(const EnterOrdersQueryById::Wrapper& query)
{
	SqlParams params;
	string sql = R"(
SELECT
    mr.id,
    mr.tenant_id,
    mr.encounter_id,
    mr.patient_id,
    mr.group_id,
    mr.status_enum,
    md.category_code,
    mr.therapy_enum,
    md.name,
    md.total_volume,
    mr.dose,
    mr.dose_unit_code,
    mr.method_code,
    mr.rate_code,
    ai.total_price,
    cc.description,
    ap.name AS practitioner_name,
    mr.skin_test_flag
FROM med_medication_request mr
LEFT JOIN med_medication_definition md
    ON mr.medication_id = md.id
LEFT JOIN adm_encounter_diagnosis aed
    ON mr.encounter_diagnosis_id = aed.id
LEFT JOIN cln_condition cc
    ON cc.id = aed.condition_id
LEFT JOIN adm_practitioner ap
    ON mr.requester_id = ap.id
LEFT JOIN adm_charge_item ai
    ON ai.service_id = mr.id
WHERE
    mr.tenant_id = ?
    AND mr.patient_id = ? AND mr.encounter_id=?
	)";
	// 构建分页条件
	sql += " LIMIT " + std::to_string(((query->pageIndex - 1) * query->pageSize)) + "," + std::to_string(query->pageSize);

	// 执行查询
	return sqlSession->executeQuery<EnterOrdersQueryDO>(sql, EnterOrdersQueryMapper(), "%s%s%s", query->tenant_id.getValue({}), query->patient_id.getValue({}), query->encounter_id.getValue({}));
}




//int EnterOrdersDAO::insertBatchTwoTable(const std::list<EnterOrdersAddDO>& dos, int batchSize)
//{
//    using T = EnterOrdersAddDO;
//    static_assert(std::is_base_of<BaseDO, T>::value, "T must inherit BaseDO");
//
//    if (dos.empty())
//        return 0;
//
//    sqlSession->beginTransaction();
//    int totalMainRows = 0;
//
//    try
//    {
//        const auto& firstDo = dos.front();
//        std::ostringstream colMain;
//        colMain << " (";
//        colMain << firstDo.getPrimaryField()->getColumn();
//        for (auto& f : firstDo.getFields())
//        {
//            colMain << ", " << f->getColumn();
//        }
//        colMain << ")";
//        const std::string tableMain = firstDo.getTable();
//
//        for (const auto& item : dos)
//        {
//            // ========== 1. 插入主表 med_medication_request（原有逻辑不动）==========
//            SqlParams paramsMain;
//            std::ostringstream valMain;
//            valMain << "(?";
//
//            if (!item.getPrimaryField()->get())
//            {
//                throw std::runtime_error("Main table primary key id cannot be null");
//            }
//
//            extractFieldValueWithNul(item.getPrimaryField(), &paramsMain);
//            for (auto& f : item.getFields())
//            {
//                extractFieldValueWithNul(f, &paramsMain);
//                valMain << ",?";
//            }
//            valMain << ")";
//
//            std::ostringstream sqlMain;
//            sqlMain << "INSERT INTO " << tableMain << colMain.str() << " VALUES " << valMain.str();
//            int row = sqlSession->executeUpdate(sqlMain.str(), paramsMain);
//            totalMainRows += row;
//
//            // ========== 2. 内部构造子表DO EnterOrdersAddDO1 插入adm_charge_item ==========
//            EnterOrdersAddDO1 subDo;
//            // 调用子DO自己的set方法赋值
//            subDo.setId1(item.getId1());
//            subDo.setTotalPrice(item.getTotalPrice());
//            // 给子DO外键赋值关联主表ID
//            subDo.service_id = item.getId();
//
//            // 拼接子表INSERT SQL（框架通用写法，不用session->insert）
//            SqlParams subParams;
//            std::ostringstream subSql;
//            subSql << "INSERT INTO adm_charge_item (id, service_id, total_price) VALUES (?,?,?)";
//            extractFieldValueWithNul(&subDo.getId1(), &subParams);
//            extractFieldValueWithNul(&subDo.service_id, &subParams);
//            extractFieldValueWithNul(&subDo.getTotalPrice(), &subParams);
//            sqlSession->executeUpdate(subSql.str(), subParams);
//        }
//
//        sqlSession->commitTransaction();
//        return totalMainRows;
//    }
//    catch (...)
//    {
//        sqlSession->rollbackTransaction();
//        throw;
//    }
//}