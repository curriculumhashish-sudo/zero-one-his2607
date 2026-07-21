 #include "DiagnoseService.h"
#include "../../dao/Diagnosis/DiagnosisDAO.h"
#include "../../lib-common/include/id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

std::string DiagnoseService::saveData(const DiagnoseAddDTO::Wrapper& dto)
{
    // 1. 入参DTO 转 数据库DO
    DiagnosisDO data;
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnosisName, name,//诊断名称
        //CategoryId, categoryId,//诊断目录id外键
        DiagnosisType, diagType,//诊断类型
        DiagnosisCode, chronicCode,//诊断编码
        MedicalInsuranceCode, medicalInsuranceCode,//医保编码
        Status, status,//状态
        IsDelete, isDelete//删除标记
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,//医保标签
        MedicalInsuranceMatch, medicalInsuranceMatch,//医保对码（T或F）
        PinyinCode, pinyinCode,//拼音助记
        WubiCode, wubiCode,//五笔助记
        Remark, remark,//说明
        CatName,catName//分类项目名
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
    DiagnosisDAO dao;
    return dao.insert(data) == 1 ? data.getId() : "";
}
// 根据ID修改诊断记录
bool DiagnoseService::updateData(const DiagnoseDTO::Wrapper& dto)
{
    DiagnosisDAO dao;

    //// 1. 先按主键查询原有数据，避免全量更新覆盖未传字段
    //DiagnosisDO data = dao.selectById(dto->getId()->getValue({}));
    //if (data.getId().empty()) {
    //    return false; // 数据不存在直接返回失败
    //}
    DiagnosisDO data;
    // 2. 入参DTO 覆盖更新到数据库DO
    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        DiagnosisName, name,       //诊断名称
        //CategoryId, categoryId,//诊断目录id外键
        DiagnosisType, diagType,   //诊断类型
        DiagnosisCode, chronicCode,//诊断编码
        MedicalInsuranceCode, medicalInsuranceCode, //医保编码
        Status, status             //状态
    );

    ZO_STAR_DOMAIN_DTO_TO_DO(data, dto,
        MedicalInsuranceTag, medicalInsuranceTag,    //医保标签
        MedicalInsuranceMatch, medicalInsuranceMatch,//医保对码
        PinyinCode, pinyinCode,    //拼音助记
        WubiCode, wubiCode,        //五笔助记
        Remark, remark             //说明
    );

    // 3. 从登录载荷取操作人，填充修改人
    data.setUpdateBy(dto->getPayload()->getUsername());

    // 4. 自动填充修改时间
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 5. 执行更新，影响行数为1则成功
    return dao.update(data) == 1;
}
// 逻辑单条删除
bool DiagnoseService::removeOne(const std::string& id)
{
    DiagnosisDAO dao;

    // 1. 查询原有数据，保证数据存在且避免全量更新丢字段
  /*  DiagnosisDO data = dao.selectById(id);
    if (data.getId().empty()) {
        return false;
    }*/

    DiagnosisDO data;
    // 2. 标记逻辑删除
    data.setIsDelete(1);

    // 3. 填充修改时间
    data.setUpdateTime(SimpleDateTimeFormat::format());

    // 注：若需记录删除操作人，需补充操作人参数，调用 data.setUpdateBy(操作人用户名);

    // 4. 执行更新完成逻辑删除
    return dao.update(data) == 1;
}
// 逻辑批量删除（停用）诊断记录
bool DiagnoseService::removeData(const oatpp::List<oatpp::String>& ids)
{
    DiagnosisDAO dao;
    //int successCount = 0;

    // 遍历ID列表逐个执行逻辑删除
    //std::list<std::string> datas;
    //for (auto& idItem : *ids) {
    //    std::string id = idItem->getValue({});
    //    DiagnosisDO data = dao.selectById(id);

    //    if (data.getId().empty()) {
    //        continue; // 不存在的ID跳过
    //    }

    //    data.setIsDelete(1);
    //    data.setUpdateTime(SimpleDateTimeFormat::format());
    //    // 同上，需记录操作人可在此统一设置

    //    if (dao.update(data) == 1) {
    //        successCount++;
    //    }
    //}
    std::list<std::string> datas;
    for (auto one : *ids.get())
        datas.emplace_back(one.getValue({}));
    int rows = dao.deleteByIds<DiagnosisDO>(datas);
    if (rows == ids->size())
        return true;
    return false;
 
}