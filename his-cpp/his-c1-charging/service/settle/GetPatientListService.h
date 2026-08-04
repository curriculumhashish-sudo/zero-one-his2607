#ifndef __GET_PATIENT_LIST_SERVICE__
#define __GET_PATIENT_LIST_SERVICE__

// 请根据实际路径调整 include
#include "domain/vo/settle/GetPatientListVO.h" 
#include "domain/query/settle/GetPatientListQuery.h"
#include "domain/dto/settle/GetPatientListDTO.h" 
#include "domain/do/settle/GetPatientListDO.h"
#include "../../dao/settle/GetPatientListDAO.h"

class GetPatientListService
{
public:
    /**
     * @brief 获取患者列表（分页）
     * @param query 查询条件
     * @return 分页结果 DTO
     */
    GetPatientListPageDTO::Wrapper selectPatientList(const GetPatientListQuery::Wrapper& query)
    {
        // 1. 创建分页对象
        auto pages = GetPatientListPageDTO::createShared();
        pages->pageIndex = query->pageIndex;
        pages->pageSize = query->pageSize;

        // 2. 初始化 DAO 并查询总数
        GetPatientListDAO dao;
        uint64_t count = dao.count(query);

        // 如果没有数据，直接返回空分页对象
        if (count <= 0)
        {
            return pages;
        }

        // 3. 设置分页信息
        pages->total = count;
        pages->calcPages(); // 计算总页数

        // 4. 查询数据列表 (注意方法名是 selectWithPage)
        list<GetPatientListDO> result = dao.selectWithPage(query);

        
               // 5. DO 转 DTO (手动处理类型转换)
        for (GetPatientListDO& sub : result)
        {
            auto dto = GetPatientListDTO::createShared();

            // --- 1. 处理 String -> UInt64 (需要 std::stoull 转换) ---
            // encounter_id
            if (!sub.getEncounter_id().empty()) {
                try { dto->encounter_id = std::stoull(sub.getEncounter_id()); }
                catch (...) { dto->encounter_id = 0.0; } // 转换失败给默认值
            }

            // admit_date
            if (!sub.getAdmit_date().empty()) {
                try { dto->admit_date = std::stoull(sub.getAdmit_date()); }
                catch (...) { dto->admit_date = 0.0; }
            }

            // --- 2. 处理 String -> Oatpp::String (通常支持隐式转换) ---
            dto->inpatient_no = sub.getInpatient_no();
            dto->patient_name = sub.getPatient_name();
            dto->dept_name = sub.getDept_name();
            dto->ward_name = sub.getWard_name();
            dto->bed_no = sub.getBed_no();
            dto->doctor_name = sub.getDoctor_name();

            // --- 3. 处理 int -> UInt8/UInt32 (需要强转) ---
            dto->gender = (v_uint8)sub.getGender();
            dto->age = (v_uint32)sub.getAge();
            dto->insurance_type = (v_uint8)sub.getInsurance_type();

            // 将 DTO 添加到分页列表中
            pages->addData(dto);
        }


        return pages;
    }
};

#endif // __GET_PATIENT_LIST_SERVICE__
