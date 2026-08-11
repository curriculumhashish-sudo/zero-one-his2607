#pragma once
#ifndef _MEDICALRECORDDTO_H_
#define _MEDICALRECORDDTO_H_

#include "../../GlobalInclude.h"
#include "../../../../lib-oatpp/include/domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

//获取历史病历列表（条件+分页）DTO类
class MedicalRecordBriefDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(MedicalRecordBriefDTO, DTO);
	//病历主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpatient-record.id"), true);

	////病历主键
	//DTO_FIELD(String,id); 
	//DTO_FIELD_INFO(id) { 
	//	info->description = YamlHelper().getString(&(ServerInfo::getInstance().getZhDictNode()), "inpatient-record.id");
	//	info->required = true; 
	//}
	
	//病历类型名称
	API_DTO_FIELD_REQUIRE(String, document_type_name, ZH_WORDS_GETTER("inpatient-record.document_type_name"), true);
	//病历名称
	API_DTO_FIELD(String, title, ZH_WORDS_GETTER("inpatient-record.title"), false, "");
	//病历状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("inpatient-record.status"), true);
	//病历更新人id
	API_DTO_FIELD_REQUIRE(String, updated_by, ZH_WORDS_GETTER("inpatient-record.updated_by"), true);
};
class MedicalRecordBriefPageDTO : public PageDTO<MedicalRecordBriefDTO::Wrapper>
{
	DTO_INIT(MedicalRecordBriefPageDTO, PageDTO<MedicalRecordBriefDTO::Wrapper>);
};

//获取历史病历详情DTO类
class MedicalRecordDetailDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(MedicalRecordDetailDTO, DTO);
	//病历主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpatient-record.id"), true);
	//病历内容详情
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("inpatient-record.content"), true);
};

//新增病历DTO类
class MedicalRecordAddDTO :public oatpp::DTO
{
	//定义
	DTO_INIT(MedicalRecordAddDTO, DTO);
	//病历主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpatient-record.id"), false);
	//病人id
	API_DTO_FIELD_REQUIRE(String, patient_id, ZH_WORDS_GETTER("inpatient-record.patient_id"), false);
	//门诊id
	API_DTO_FIELD_REQUIRE(String, encounter_id, ZH_WORDS_GETTER("inpatient-record.encounter_id"), false);
	//病历类型编码
	API_DTO_FIELD_REQUIRE(String, document_type_code, ZH_WORDS_GETTER("inpatient-record.document_type_code"), false);
	////病历类型名称
	//API_DTO_FIELD_REQUIRE(String, document_type_name, ZH_WORDS_GETTER("inpatient-record.document_type_name"), false);
	//病历名称
	API_DTO_FIELD_REQUIRE(String, title, ZH_WORDS_GETTER("inpatient-record.title"), false);
	//病历状态
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("inpatient-record.status"), false);
	//病历内容详情
	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("inpatient-record.content"), true);
	////病历创建时间
	//API_DTO_FIELD_REQUIRE(String, created_at, ZH_WORDS_GETTER("inpatient-record.created_at"), false);
	////病历更新时间
	//API_DTO_FIELD_REQUIRE(String, updated_at, ZH_WORDS_GETTER("inpatient-record.updated_at"), true);
	////病历创建人id
	//API_DTO_FIELD_REQUIRE(String, created_by, ZH_WORDS_GETTER("inpatient-record.created_by"), false);
	////病历更新人id
	//API_DTO_FIELD_REQUIRE(String, updated_by, ZH_WORDS_GETTER("inpatient-record.updated_by"), true);
	// 关联一个PayloadDTO负载数据对象
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
	// 数据校验
	std::string validate()
	{
		if (!encounter_id)
			return "content invalidate.";
		return "";
	}

};

////修改病历DTO类
//class MedicalRecordModifyDTO :public oatpp::DTO
//{
//	//定义
//	DTO_INIT(MedicalRecordModifyDTO, DTO);
//	//病历主键
//	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("inpatient-record.id"), true);
//	//病历内容详情
//	API_DTO_FIELD_REQUIRE(String, content, ZH_WORDS_GETTER("inpatient-record.content"), true);
//	//病历更新时间
//	API_DTO_FIELD_REQUIRE(String, update_at, ZH_WORDS_GETTER("inpatient-record.update_at"), true);
//	//病历更新人id
//	API_DTO_FIELD_REQUIRE(String, update_by, ZH_WORDS_GETTER("inpatient-record.update_by"), true);
//};

#include OATPP_CODEGEN_END(DTO)
#endif






///**
// * 示例分页传输对象
// */
//class SamplePageDTO : public PageDTO<SampleDTO::Wrapper>
//{
//	DTO_INIT(SamplePageDTO, PageDTO<SampleDTO::Wrapper>);
//};