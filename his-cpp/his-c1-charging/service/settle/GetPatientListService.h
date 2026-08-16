#ifndef _GET_PATIENT_LIST_SERVICE_
#define _GET_PATIENT_LIST_SERVICE_

#include "domain/vo/settle/GetPatientListVO.h" 
#include "domain/query/settle/GetPatientListQuery.h"
#include "domain/dto/settle/GetPatientListDTO.h"
#include "domain/do/settle/GetPatientListDO.h"
#include "../../dao/settle/GetPatientListDAO.h"

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

        // 如果没有数据，直接返回空分页对象
        if (count <= 0)
        {
            return pages;
        }

        // 3. 设置分页信息
        pages->total = count;
        pages->calcPages();

        // 4. 查询数据列表
        std::list<GetPatientListDO> result = dao.selectWithPage(query);

        // 5. DO 转 DTO（主键与日期按字符串直接赋值，保持数据库原样，不转数字）
        for (GetPatientListDO& sub : result)
        {
            auto dto = GetPatientListDTO::createShared();

            // 就诊/患者/账户主键直接返回数据库主键字符串
            dto->encounter_id = sub.getId();
            dto->patient_id = sub.getPatient_id();
            dto->account_id = sub.getAccount_id();

            // 入院日期按日期字符串返回
            dto->admit_date = sub.getAdmit_date();

            // 业务展示字段
            dto->inpatient_no = sub.getBus_no();
            dto->patient_name = sub.getName();
            dto->dept_name = sub.getOrg_name();
            dto->ward_name = sub.getWard_name();
            dto->bed_no = sub.getBed_no();
            // dto->doctor_name = sub.getDoctor_name(); // 已注释，DAO无此字段

            // 数值类型转换
            dto->gender = (v_uint8)sub.getGender_enum();
            dto->age = (v_uint32)sub.getAge();
            dto->insurance_type = (v_uint8)sub.getInsurance_type();

            pages->addData(dto);
        }
        return pages;
    }
};

#endif // !_GET_PATIENT_LIST_SERVICE_