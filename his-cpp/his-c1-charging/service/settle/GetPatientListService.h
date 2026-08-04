#ifndef _GET_PATIENT_LIST_SERVICE_
#define _GET_PATIENT_LIST_SERVICE_

#include "domain/vo/settle/GetPatientListVO.h" 
#include "domain/query/settle/GetPatientListQuery.h"
#include "domain/DTO/settle/GetPatientListDTO.h"
#include "domain/do/settle/GetPatientListDO.h"
#include "../../dao/settle/GetPatientListDAO.h"
#include <iostream> // 控制台输出头文件

class GetPatientListService
{
public:
    GetPatientListPageDTO::Wrapper selectPatientList(const GetPatientListQuery::Wrapper& query)
    {
        // 1. 创建分页对象
        auto pages = GetPatientListPageDTO::createShared();
        pages->pageIndex = query->pageIndex;
        pages->pageSize = query->pageSize;

        // 2. 初始化 DAO 并查询总数
        GetPatientListDAO dao;
        uint64_t count = dao.count(query);
        std::cout << "[DEBUG] total count = " << count << std::endl;

        // 如果没有数据，直接返回空分页对象
        if (count <= 0)
        {
            std::cout << "[DEBUG] count <= 0，直接返回空分页" << std::endl;
            return pages;
        }

        // 3. 设置分页信息
        pages->total = count;
        pages->calcPages();

        // 4. 查询数据列表
        std::list<GetPatientListDO> result = dao.selectWithPage(query);
        std::cout << "[DEBUG] DO列表数量：" << result.size() << std::endl;

        // ========== 循环打印DO，同事要求的调试代码 ==========
        for (auto& sub : result)
        {
            std::cout << "==== DO数据 ====" << std::endl;
            std::cout << "encounter_id:" << sub.getId() << std::endl;
            std::cout << "name:" << sub.getName() << std::endl;
            std::cout << "bus_no:" << sub.getBus_no() << std::endl;
            std::cout << "admit_date:" << sub.getAdmit_date() << std::endl;
        }
        // =================================================

        // 5. DO 转 DTO (手动处理类型转换)
        uint64_t startSort = (pages->pageIndex - 1) * pages->pageSize + 1;
        for (GetPatientListDO& sub : result)
        {
            auto dto = GetPatientListDTO::createShared();
            //dto->sort_num = startSort++;

            // --- 1. 处理 ID (String -> UInt64) ---
            if (!sub.getId().empty()) {
                try { dto->encounter_id = std::stoull(sub.getId()); }
                catch (...) { dto->encounter_id = (v_uint64)0; }
            }

            // ⚠️警告："2026-07-01"字符串无法使用stoull，必然捕获异常置0
            if (!sub.getAdmit_date().empty()) {
                try {
                    dto->admit_date = (v_uint64)std::stoull(sub.getAdmit_date());
                }
                catch (...) {
                    dto->admit_date = (v_uint64)0;
                    std::cout << "[WARN] admit_date 转换异常，日期字符串无法转为数字" << std::endl;
                }
            }

            // --- 3. std::string → oatpp::String ---
            dto->inpatient_no = sub.getBus_no();
            dto->patient_name = sub.getName();
            dto->dept_name = sub.getOrg_name();
            dto->ward_name = sub.getWard_name();
            dto->bed_no = sub.getBed_no();
            // dto->doctor_name = sub.getDoctor_name(); // 已注释，DAO无此字段

            // --- 4. 数值类型转换 ---
            dto->gender = (v_uint8)sub.getGender_enum();
            dto->age = (v_uint32)sub.getAge();
            dto->insurance_type = (v_uint8)sub.getInsurance_type();

            pages->addData(dto);
        }
        //        std::cout << "[DEBUG] 最终分页data数量：" << pages->data->size() << std::endl;
        return pages;
    }
};

#endif // !_GET_PATIENT_LIST_SERVICE_