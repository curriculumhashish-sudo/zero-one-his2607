#ifndef DIAGNOSE_SERVICE_H
#define DIAGNOSE_SERVICE_H

#include <string>
#include "dto/diagnose/DiagnoseAddDTO.hpp"
#include "dto/diagnose/DiagnoseEditDTO.hpp"
#include "dto/PayloadDTO.hpp"

class DiagnoseService
{
public:
    // 单条新增诊断记录
    std::string saveData(const DiagnoseAddDTO::Wrapper& dto);

    // 根据ID修改诊断记录
    bool editData(const DiagnoseEditDTO::Wrapper& dto);

    // 逻辑删除单条诊断记录
    bool removeData(const std::string& id, const PayloadDTO& payload);
};

#endif
DiagnoseService.cpp 实现（严格对齐 Sample 编码规范 + 匹配既定 DTO 字段）
cpp
运行
#include "DiagnoseService.h"
#include "../../dao/diagnose/DiagnoseDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

// ====================== 1. 单条新增 ======================
std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
{
    // 1. 入参DTO 转 数据库DO（完全匹配既定AddDTO字段）
    DiagnoseDO data;
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        Name, name,
        CategoryId, categoryId,
        DiagType, diagType,
        ChronicCode, chronicCode,
        MedicalInsuranceCode, medicalInsuranceCode,
        Status, status,
        MedicalInsuranceTag, medicalInsuranceTag,
        MedicalInsuranceMatch, medicalInsuranceMatch,
        PinyinCode, pinyinCode,
        WubiCode, wubiCode,
        Remark, remark
    );

    // 2. 生成全局唯一主键ID
    UuidFacade uf;
    data.setId(uf.genUuid());

    // 3. 从DTO内嵌的登录载荷取创建人（匹配CC_SYNTHESIZE生成的getPayload）
    data.setCreateBy(dto->getPayload()->getUsername());
    // 4. 自动填充创建时间
    data.setCreateTime(SimpleDateTimeFormat::format());
    // 5. 逻辑删除标记默认正常
    data.setIsDelete(0);

    // 6. 执行入库，成功返回主键ID，失败返回空串
    DiagnoseDAO dao;
    return dao.insert(data) == 1 ? data.getId() : "";
}


#endif