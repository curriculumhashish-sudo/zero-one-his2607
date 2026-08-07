#include "stdafx.h"
#include "EnrollDAO.h"
#include "EnrollListMapper.h"
#include "EnrollDetailMapper.h"
#include <sstream>
#include <vector>

/**
 * @brief 构建住院登记列表和总数查询共用的筛选条件。
 *
 * 根据前端传入的关键字、身份证号和登记状态，动态拼接 WHERE 条件。
 * 同时按照 SQL 中 ? 的出现顺序，将对应的实际值写入 params，
 * 供 count 和 selectWithPage 两个方法共同使用。
 *
 * @param query 前端传入的住院登记查询条件，包含关键字、身份证号、
 *              登记状态、页码和每页条数等信息。
 * @param[out] params SQL 参数集合。方法会将每个 ? 对应的实际参数按顺序加入其中。
 * @return 拼接完成的 WHERE 条件字符串，不包含 SELECT、FROM 和 ORDER BY。
 */
std::string EnrollDAO::buildCondition(const EnrollQuery::Wrapper& query, SqlParams& params)
{
    // 使用字符串流逐段拼接 WHERE 条件，最后通过 str() 获取完整字符串。
    std::ostringstream condition;

    // 排除逻辑删除数据，并限定查询对象为住院类型登记。
    condition << R"(
        WHERE e.is_deleted = 0
          AND p.is_deleted = 0
          AND e.class_enum = 'IMP'
    )";

    // 有关键字时，同时按患者姓名和住院号进行模糊查询。
    if (query->keyword && !query->keyword->empty())
    {
        // % 表示任意数量字符，例如 %小张% 可以匹配“小张三”。
        std::string keyword = "%" + (*query->keyword.get()) + "%";

        condition << R"(
            AND (p.name LIKE ? OR e.bus_no LIKE ?)
        )";

        // 两个 ? 分别对应姓名和住院号查询，必须按 SQL 中的顺序加入参数。
        SQLPARAMS_PUSH(params, "s", std::string, keyword);
        SQLPARAMS_PUSH(params, "s", std::string, keyword);
    }

    // 有身份证号时，使用精确匹配缩小查询范围。
    if (query->idCard && !query->idCard->empty())
    {
        std::string idCard = *query->idCard.get();

        condition << R"(
            AND p.id_card = ?
        )";

        SQLPARAMS_PUSH(params, "s", std::string, idCard);
    }

    // 保存前端传入的登记状态；一个请求可传入多个以逗号分隔的状态。
    std::vector<std::string> statuses;

    if (query->registrationListStatuses && !query->registrationListStatuses->empty())
    {
        std::istringstream statusStream(*query->registrationListStatuses.get());
        std::string status;

        // 例如 REGISTERED,IN_PROGRESS 会被拆分为两个独立状态。
        while (std::getline(statusStream, status, ','))
        {
            if (!status.empty())
            {
                statuses.push_back(status);
            }
        }
    }

    // 未传状态时，使用住院登记列表页面的默认展示状态。
    if (statuses.empty())
    {
        statuses =
        {
            "PLANNED",
            "REGISTERED",
            "IN_PROGRESS"
        };
    }

    // 根据状态数量生成 IN (?, ?, ...)；每个 ? 都要对应一个参数。
    condition << R"(
        AND e.status_enum IN (
    )";

    for (size_t index = 0; index < statuses.size(); ++index)
    {
        if (index > 0)
        {
            condition << R"(, )";
        }

        condition << R"(?)";
        SQLPARAMS_PUSH(params, "s", std::string, statuses[index]);
    }

    condition << R"(
        )
    )";

    return condition.str();
}

/**
 * @brief 统计符合筛选条件的住院登记总数。
 *
 * 使用与列表查询完全相同的筛选条件，计算符合条件的住院登记数量，
 * 供 Service 层计算总页数和前端显示“共多少条记录”。
 * 使用 COUNT(DISTINCT e.id) 防止关联或测试数据重复导致重复计数。
 *
 * @param query 前端传入的住院登记查询条件。
 * @return 符合条件的住院登记总数。
 */
uint64_t EnrollDAO::count(const EnrollQuery::Wrapper& query)
{
    // 保存 SQL 中每个 ? 对应的实际参数，顺序必须与 SQL 保持一致。
    SqlParams params;
    std::ostringstream sql;

    // 总数查询只需要住院登记和患者表；筛选条件会使用这两个表的字段。
    sql << R"(
        SELECT COUNT(DISTINCT e.id)
        FROM adm_encounter e
        INNER JOIN adm_patient p
            ON p.id = e.patient_id
    )";

    // 复用列表查询的筛选规则，确保总数与列表数据一致。
    sql << buildCondition(query, params);

    // 执行数值查询并返回第一列的总记录数。
    return sqlSession->executeQueryNumerical(sql.str(), params);
}

/**
 * @brief 分页查询符合条件的住院登记列表。
 *
 * 查询住院登记、患者、住院科室、当前病区和当前床位信息，
 * 根据查询条件筛选后按入院时间倒序返回当前页记录。
 * 查询结果通过 EnrollListMapper 映射为 EnrollListDO 列表。
 *
 * @param query 前端传入的住院登记查询条件，包含关键字、身份证号、
 *              登记状态、当前页码和每页条数。
 * @return 当前页的住院登记列表；无符合条件的数据时返回空列表。
 */
std::list<EnrollListDO> EnrollDAO::selectWithPage(const EnrollQuery::Wrapper& query)
{
    // 保存 SQL 中每个 ? 对应的实际参数，顺序必须与 SQL 保持一致。
    SqlParams params;

    // 使用字符串流拼接复杂 SQL，最后通过 sql.str() 获取完整语句。
    std::ostringstream sql;

    // 字段别名必须与 EnrollListMapper 中读取的列名完全一致。
    sql << R"(
        SELECT DISTINCT
            e.id AS encounter_id,
            e.patient_id AS patient_id,
            e.bus_no AS encounter_bus_no,
            p.name AS patient_name,
            e.status_enum AS status_enum,
            e.organization_id AS organization_id,
            o.name AS organization_name,
            loc.ward_id AS ward_id,
            loc.ward_name AS ward_name,
            loc.bed_id AS bed_id,
            loc.bed_name AS bed_name,
            e.start_time AS start_time
        FROM adm_encounter e
        INNER JOIN adm_patient p
            ON p.id = e.patient_id
        LEFT JOIN adm_organization o
            ON o.id = e.organization_id
            AND o.is_deleted = 0
    )";

    // 一个住院登记可能分别关联病区和床位两条位置记录。
    // 先按 encounter_id 汇总为一行，避免外层列表产生重复记录。
    sql << R"(
        LEFT JOIN
        (
            SELECT
                el.encounter_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'WARD'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.id
                    END
                ) AS ward_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'WARD'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.name
                    END
                ) AS ward_name,

                MAX(
                    CASE
                        WHEN el.form_enum = 'BED'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.id
                    END
                ) AS bed_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'BED'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.name
                    END
                ) AS bed_name
            FROM adm_encounter_location el
            LEFT JOIN adm_location l
                ON l.id = el.location_id
                AND l.is_deleted = 0
                AND l.status_enum = 'ACTIVE'
            WHERE el.is_deleted = 0
            GROUP BY el.encounter_id
        ) loc
            ON loc.encounter_id = e.id
    )";

    // 复用总数查询的筛选规则，并同步填充所有筛选参数。
    sql << buildCondition(query, params);

    // 使用固定排序规则，保证翻页时不会因排序不稳定而重复或漏数据。
    sql << R"(
        ORDER BY e.start_time DESC, e.id DESC
    )";

    // 未传分页参数时使用 PageQuery 的默认值；页码从 1 开始。
    uint64_t pageIndex = query->pageIndex.getValue(1);
    uint64_t pageSize = query->pageSize.getValue(10);

    if (pageIndex == 0)
    {
        pageIndex = 1;
    }

    if (pageSize == 0)
    {
        pageSize = 10;
    }

    // LIMIT 的第一个参数是跳过数量，第二个参数是当前页返回数量。
    uint64_t offset = (pageIndex - 1) * pageSize;

    sql << R"(
        LIMIT ?, ?
    )";

    SQLPARAMS_PUSH(params, "ull", uint64_t, offset);
    SQLPARAMS_PUSH(params, "ull", uint64_t, pageSize);

    // 框架逐行调用 EnrollListMapper，将结果集转换为 EnrollListDO 列表。
    return sqlSession->executeQuery<EnrollListDO>(sql.str(), EnrollListMapper(), params);
}

/**
 * @brief 根据住院登记主键查询一条住院登记详情。
 *
 * @param encounterId 住院登记主键，对应 adm_encounter.id。
 * @return 查询到时返回住院登记详情对象；未查询到时返回 nullptr。
 *
 * @details
 * 查询前先将病区、床位、主诊断和历史住院次数分别汇总，
 * 再关联到住院登记主表，保证一个 encounterId 最终只返回一行。
 * 使用参数化查询绑定 encounterId，避免直接拼接输入值。
 */
PtrEnrollDetailDO EnrollDAO::selectDetailById(const std::string& encounterId)
{
    SqlParams params;
    std::ostringstream sql;

    sql << R"(
        SELECT DISTINCT
            e.id AS encounter_id,
            e.bus_no AS encounter_bus_no,
            e.patient_id AS patient_id,
            p.name AS patient_name,
            p.id_card AS id_card,
            p.gender_enum AS gender_enum,
            p.birth_date AS birth_date,
            p.phone AS phone,

            e.status_enum AS status_enum,
            e.organization_id AS organization_id,
            o.name AS organization_name,
            loc.ward_id AS ward_id,
            loc.ward_name AS ward_name,
            loc.bed_id AS bed_id,
            loc.bed_name AS bed_name,
            e.start_time AS start_time,

            diag.condition_definition_id AS condition_definition_id,
            diag.condition_definition_name AS condition_definition_name,
            diag.diagnosis_description AS diagnosis_description,

            e.admit_source_code AS admit_source_code,
            admit_source.display AS admit_source_code_text,
            e.in_way_code AS in_way_code,
            in_way.display AS in_way_code_text,
            e.priority_enum AS priority_enum,
            outpatient.outpatient_doctor_name AS outpatient_doctor_name,
            attending.attending_doctor_id AS attending_doctor_id,
            attending.attending_doctor_name AS attending_doctor_name,
            COALESCE(history.admission_count, 0) AS admission_count

        FROM adm_encounter e

        INNER JOIN adm_patient p
            ON p.id = e.patient_id
            AND p.is_deleted = 0

        LEFT JOIN adm_organization o
            ON o.id = e.organization_id
            AND o.is_deleted = 0
    )";

    sql << R"(
        LEFT JOIN
        (
            SELECT
                el.encounter_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'WARD'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.id
                    END
                ) AS ward_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'WARD'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.name
                    END
                ) AS ward_name,

                MAX(
                    CASE
                        WHEN el.form_enum = 'BED'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.id
                    END
                ) AS bed_id,

                MAX(
                    CASE
                        WHEN el.form_enum = 'BED'
                        AND el.status_enum = 'ACTIVE'
                        THEN l.name
                    END
                ) AS bed_name

            FROM adm_encounter_location el

            LEFT JOIN adm_location l
                ON l.id = el.location_id
                AND l.is_deleted = 0
                AND l.status_enum = 'ACTIVE'

            WHERE el.is_deleted = 0

            GROUP BY el.encounter_id
        ) loc
            ON loc.encounter_id = e.id
    )";

    sql << R"(
        LEFT JOIN
        (
            SELECT
                ed.encounter_id,

                MAX(
                    CASE
                        WHEN ed.primary_flag = 1
                        THEN cd.id
                    END
                ) AS condition_definition_id,

                MAX(
                    CASE
                        WHEN ed.primary_flag = 1
                        THEN cd.name
                    END
                ) AS condition_definition_name,

                MAX(
                    CASE
                        WHEN ed.primary_flag = 1
                        THEN ed.description
                    END
                ) AS diagnosis_description

            FROM adm_encounter_diagnosis ed

            LEFT JOIN cln_condition_definition cd
                ON cd.id = ed.condition_id
                AND cd.is_deleted = 0

            WHERE ed.is_deleted = 0

            GROUP BY ed.encounter_id
        ) diag
            ON diag.encounter_id = e.id
    )";

    // 将当前住院登记的主治医生归并为一行，避免多名参与人导致详情重复。
    sql << R"(
        LEFT JOIN
        (
            SELECT
                ep.encounter_id,

                MAX(
                    CASE
                        WHEN ep.role_code = 'ATTENDING_DOCTOR'
                        AND ep.status_enum = 'ACTIVE'
                        THEN pr.id
                    END
                ) AS attending_doctor_id,

                MAX(
                    CASE
                        WHEN ep.role_code = 'ATTENDING_DOCTOR'
                        AND ep.status_enum = 'ACTIVE'
                        THEN pr.name
                    END
                ) AS attending_doctor_name

            FROM adm_encounter_participant ep

            LEFT JOIN adm_practitioner pr
                ON pr.id = ep.practitioner_id
                AND pr.is_deleted = 0
                AND pr.status_enum = 'ACTIVE'

            WHERE ep.is_deleted = 0

            GROUP BY ep.encounter_id
        ) attending
            ON attending.encounter_id = e.id
    )";

    // 门诊医生属于门诊就诊记录，因此通过当前登记的 amb_encounter_id 关联。
    sql << R"(
        LEFT JOIN
        (
            SELECT
                ep.encounter_id,

                MAX(
                    CASE
                        WHEN ep.role_code = 'ADMITTER'
                        AND ep.status_enum = 'ACTIVE'
                        THEN pr.name
                    END
                ) AS outpatient_doctor_name

            FROM adm_encounter_participant ep

            LEFT JOIN adm_practitioner pr
                ON pr.id = ep.practitioner_id
                AND pr.is_deleted = 0
                AND pr.status_enum = 'ACTIVE'

            WHERE ep.is_deleted = 0

            GROUP BY ep.encounter_id
        ) outpatient
            ON outpatient.encounter_id = e.amb_encounter_id
    )";

    // 从术语表读取代码对应的显示文字，避免在 C++ 中硬编码业务字典。
    sql << R"(
        LEFT JOIN
        (
            SELECT
                tc.code,
                MAX(tc.display) AS display

            FROM ter_code_system cs

            INNER JOIN ter_concept tc
                ON tc.code_system_id = cs.id
                AND tc.is_deleted = 0
                AND tc.is_active = 1

            WHERE cs.is_deleted = 0
                AND cs.name = 'admit_source_code'

            GROUP BY tc.code
        ) admit_source
            ON admit_source.code = e.admit_source_code

        LEFT JOIN
        (
            SELECT
                tc.code,
                MAX(tc.display) AS display

            FROM ter_code_system cs

            INNER JOIN ter_concept tc
                ON tc.code_system_id = cs.id
                AND tc.is_deleted = 0
                AND tc.is_active = 1

            WHERE cs.is_deleted = 0
                AND cs.name = 'in_way_code'

            GROUP BY tc.code
        ) in_way
            ON in_way.code = e.in_way_code
    )";

    sql << R"(
        LEFT JOIN
        (
            SELECT
                patient_id,
                COUNT(DISTINCT id) AS admission_count

            FROM adm_encounter

            WHERE is_deleted = 0
                AND class_enum = 'IMP'

            GROUP BY patient_id
        ) history
            ON history.patient_id = e.patient_id

        WHERE e.id = ?
            AND e.is_deleted = 0
            AND e.class_enum = 'IMP'
    )";

    SQLPARAMS_PUSH(params, "s", std::string, encounterId);

    return sqlSession->executeQueryOne<PtrEnrollDetailDO>
    (
        sql.str(),
        EnrollDetailMapper(),
        params
    );
}
