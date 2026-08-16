#include "stdafx.h"
#include "EnterOrdersService.h"
#include "../../dao/enter-orders/EnterOrdersDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
EnterOrdersQueryPageDTO::Wrapper EnterOrdersService::select(const EnterOrdersQueryById::Wrapper& query)
{
	// 构建返回对象
	auto pages = EnterOrdersQueryPageDTO::createShared();
	pages->pageIndex = query->pageIndex;
	pages->pageSize = query->pageSize;

	// 查询数据总条数
	EnterOrdersDAO dao;
	uint64_t count = dao.count(query);
	if (count <= 0)
	{
		return pages;
	}

	// 分页查询数据
	pages->total = count;
	pages->calcPages();
	list<EnterOrdersQueryDO> result = dao.selectWithPage(query);
	// 将DO转换成DTO
	for (EnterOrdersQueryDO& sub : result)
	{
		auto dto = EnterOrdersQueryDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO(dto, sub, id, Id, tenant_id, Tenant_id, encounter_id, Encounter_id, patient_id, Patient_id, group_id, Group_id, status, Status, medicine_type, Medicine_type, therapy_type, Therapy_type, medicine_name, Medicine_name
		, specification, Specification, dose, Dose, dose_unit, Dose_unit, method, Method, rate, Rate, total_price, Total_price, description, Description, authored_by, Authored_by, skin_text, Skin_text);
		pages->addData(dto);
	}
	return pages;
}


bool EnterOrdersService::batchAddTwoTable(const oatpp::List<EnterOrdersAddDTO::Wrapper>& mainDtoList, const oatpp::List<EnterOrdersAddDTO1::Wrapper>& subDtoList)
{
    UuidFacade uf1(1);
    UuidFacade uf2(1);
    std::string ct = SimpleDateTimeFormat::format();
    std::list<EnterOrdersAddDO> dos1;
    std::list<EnterOrdersAddDO1> dos2;

    auto iterMain = mainDtoList->begin();
    auto iterSub = subDtoList->begin();
    for (; iterMain != mainDtoList->end(); ++iterMain, ++iterSub)
    {
        auto dto1 = *iterMain;
        auto dto2 = *iterSub;
        EnterOrdersAddDO& data1 = dos1.emplace_back();
        EnterOrdersAddDO1& data2 = dos2.emplace_back();
        // 大写字段名同步DO修正后的映射
        ZO_STAR_DOMAIN_DTO_TO_DO(data1, dto1,
            Tenant_id, tenant_id,
            Encounter_id, encounter_id,
            Patient_id, patient_id,
            Group_id, group_id,
            Status_enum, status,
            Therapy_enum, therapy_type,
            Dose, dose,
            Dose_unit_code, dose_unit,
            Method_code, method,
            Rate_code, rate,
            Created_at, created_at,
            Created_by, created_by,
            Skin_test_flag, skin_text,
            Medication_id, medication_id,
            Condition_definition_id, condition_definition_id,
            Bus_no, bus_no,
            Quantity, quantity,
            Unit_code, unit_code
        );
        data1.setCreated_at(ct);
        data1.setId(uf1.genUuid());
        data1.setUpdated_at(ct);
        data1.setUpdated_by(data1.getCreated_by());
        ZO_STAR_DOMAIN_DTO_TO_DO(data2, dto2, 
            Total_price, total_price 
        );
        data2.setId(uf2.genUuid());
        data2.setService_id(data1.getId());
        data2.setCreated_at(ct);
        data2.setPatient_id(data1.getPatient_id());
        data2.setEncounter_id(data1.getEncounter_id());
    }
    int res1 = EnterOrdersDAO().insertBatch(dos1);
    int res2 = EnterOrdersDAO().insertBatch(dos2);
    if (res1 != 0 && res2 != 0)
        return true;
    else
        return false;
}


bool EnterOrdersService::updateData(const EnterOrdersModifyDTO::Wrapper& dto)
{
    // 组装DO数据
    EnterOrdersAddDO data1;
    EnterOrdersAddDO1 data2;
    ZO_STAR_DOMAIN_DTO_TO_DO(data1, dto, Therapy_enum, therapy_type, Dose, dose, Dose_unit_code, dose_unit, Method_code, method, Rate_code, rate, Updated_at, updated_at, Updated_by, updated_by, Skin_test_flag, skin_text, Medication_id, medication_id, Condition_definition_id, condition_definition_id, Id, id);
    // 设置修改时间
    data1.setUpdated_at(SimpleDateTimeFormat::format());
    ZO_STAR_DOMAIN_DTO_TO_DO(data2, dto, Total_price, total_price, Id, adm_id);

    // 执行数据修改
    EnterOrdersDAO dao;
    int res1 = dao.update(data1);
    int res2 = dao.update(data2);
    if (res1 == 1 && res2 == 1)
        return true;
    else
        return false;
}


bool EnterOrdersService::removeData(const oatpp::List<oatpp::String>& ids)
{
    EnterOrdersDAO dao;
    std::list<std::string> datas;
    for (auto one : *ids.get())
        datas.emplace_back(one.getValue({}));
    int rows = dao.deleteByIds<EnterOrdersAddDO>(datas);
    if (rows == ids->size())
        return true;
    return false;
}








//// 医嘱主键
//MYSQL_SYNTHESIZE(string, id, Id);                            对应med_medication_request.id
//
//MYSQL_SYNTHESIZE(string, id1, Id1);                            对应adm_charge_item.id
//// 租户id
//MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);                对应med_medication_request.tenant_id
//// 诊断id
//MYSQL_SYNTHESIZE(string, encounter_id, Encounter_id);        对应med_medication_request.encounter_id
//// 患者id
//MYSQL_SYNTHESIZE(string, patient_id, Patient_id);            对应med_medication_request.patient_id
//// 小组id
//MYSQL_SYNTHESIZE(string, group_id, Group_id);                对应med_medication_request.group_id
//// 医嘱状态
//MYSQL_SYNTHESIZE(string, status, Status);                    对应med_medication_request.status_enum
//// 医嘱类型
//MYSQL_SYNTHESIZE(string, therapy_type, Therapy_type);        对应med_medication_request.therapy_enum
//// 单次剂量
//MYSQL_SYNTHESIZE(string, dose, Dose);                        对应med_medication_request.dose
//// 剂量单位
//MYSQL_SYNTHESIZE(string, dose_unit, Dose_unit);                对应med_medication_request.dose_unit_code
//// 服用方式
//MYSQL_SYNTHESIZE(string, method, Method);                    对应med_medication_request.method_code
//// 服用时间
//MYSQL_SYNTHESIZE(string, rate, Rate);                         对应med_medication_request.rate_code
//// 新增时间
//MYSQL_SYNTHESIZE(string, created_at, Created_at);            对应med_medication_request.created_at
//// 新增人
//MYSQL_SYNTHESIZE(string, created_by, Created_by);            对应med_medication_request.created_by
//// 诊断描述
//MYSQL_SYNTHESIZE(string, description, Description);            对应cln_condition.description
//// 皮试标志
//MYSQL_SYNTHESIZE(string, skin_text, Skin_text);                对应med_medication_request.skin_test_flag
//// 药品id
//MYSQL_SYNTHESIZE(string, medication_id, Medication_id);         对应med_medication_request.medication_id
//// 诊断id
//MYSQL_SYNTHESIZE(string, condition_definition_id, Condition_definition_id);     对应med_medication_request.condition_definition_id
//// 总金额
//MYSQL_SYNTHESIZE(string, total_price, Total_price);            对应adm_charge_item.total_price
//
//其中adm_charge_item.service_id = med_medication_request.id