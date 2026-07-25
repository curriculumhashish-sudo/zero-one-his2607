#include "stdafx.h"

#include "patientQueueDAO.h"
#include "patientQueueMapper.h"
#include <iostream>

std::list<PtrBkgWaitlistEntryDO> PatientQueueDAO::getPatientQueueList(const PatientQueueQuery::Wrapper& query)
{
    //我需要患者的姓名、性别、出生日期、病历号，挂号的号类型名称，候诊状态，创建时间
    string sql = R"(SELECT
                            p.name,
                            p.gender_enum,
                            p.birth_date,
                            p.bus_no,
                            serv.name AS service_name,
                            w.created_at,
                            w.status,
                            w.id,
                            w.patient_id,
                            w.schedule_id
                    FROM bkg_waitlist_entry w
                    LEFT JOIN adm_patient p ON w.patient_id = p.id
                    LEFT JOIN bkg_schedule_template sch ON w.schedule_id = sch.id
                    LEFT JOIN adm_healthcare_service serv ON sch.healthcare_service_id = serv.id
                    WHERE 1=1
                    )";
    SqlParams params;

    // 为调试收集参数，便于打印
    std::string dbg_date;
    std::string dbg_patientName;
    std::string dbg_status;

    //就诊日期：支持传入 YYYY-MM-DD 或带时间戳的字符串
    if (!query->date.getValue("").empty())
    {
        std::string raw = query->date.getValue("");
        std::string dateOnly = raw.size() >= 10 ? raw.substr(0, 10) : raw; // YYYY-MM-DD
        // 使用半开区间匹配： >= 'YYYY-MM-DD 00:00:00' AND < 'YYYY-MM-DD' + INTERVAL 1 DAY
        sql += "AND w.created_at >= CONCAT(?, ' 00:00:00') AND w.created_at < DATE_ADD(CONCAT(?, ' 00:00:00'), INTERVAL 1 DAY) ";
        SQLPARAMS_PUSH(params, "s", std::string, dateOnly);
        SQLPARAMS_PUSH(params, "s", std::string, dateOnly);
        dbg_date = dateOnly;
    }
    //患者姓名模糊搜索/直接指定搜索
    if (!query->patientName.getValue("").empty())
    {
        sql += "AND p.name LIKE CONCAT('%', ?, '%') ";
        SQLPARAMS_PUSH(params, "s", std::string, query->patientName.getValue(""));
        dbg_patientName = query->patientName.getValue("");
    }

    //状态
    if (!query->allStatus.getValue("全部").empty())
    {
        if (!query->visitStatus.getValue("").empty())
        {
            sql += "AND w.status = ?";
            SQLPARAMS_PUSH(params, "s", std::string, query->visitStatus.getValue(""));
            dbg_status = query->visitStatus.getValue("");
        }
        else if(!query->finishStatus.getValue("").empty())
        {
            sql += "AND w.status = ?";
            SQLPARAMS_PUSH(params, "s", std::string, query->finishStatus.getValue(""));
            dbg_status = query->finishStatus.getValue("");
        }
    }



    // 执行查询并打印返回行数，便于判断是 DB 无数据 还是 Mapper 问题
    auto result = sqlSession->executeQuery<PtrBkgWaitlistEntryDO>(sql, PatientQueueMapper(), params);
    return result;
}
