#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 包装DTO
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDICALORDERVO_H_
#define _MEDICALORDERVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/medicalorder/MedOrderDTO.h"
#include "../../dto/medicalorder/OrderMergeDTO.h"
#include "../../dto/medicalorder/OrderSplitDTO.h"
#include "../../dto/medicalorder/OrderStopDTO.h"
#include "domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

//VO列表
//分页订单列表查询的 JSON 响应
class MedOrderPageJsonVO : public JsonVO<MedOrderPageDTO::Wrapper>
{
	DTO_INIT(MedOrderPageJsonVO, JsonVO<MedOrderPageDTO::Wrapper>);
};

//合并VO返回group_id
//合并操作结果的JSON响应
class MedgeResultDTO : public oatpp::DTO
{
	DTO_INIT(MedgeResultDTO, DTO);
	API_DTO_FIELD_DEFAULT(String, groupId, ZH_WORDS_GETTER("medicalorder.field.groupId"));
};

class MergeResultJsonVO : public JsonVO<MedgeResultDTO::Wrapper>
{
	DTO_INIT(MergeResultJsonVO, JsonVO<MedgeResultDTO::Wrapper>);
};

//分嘱VO
//分嘱操作结果的JSON响应
class SplitReslutDTO : public oatpp::DTO
{
	DTO_INIT(SplitReslutDTO, DTO);
	//拆分嘱托数量
	API_DTO_FIELD_DEFAULT(Int32, splitCount, ZH_WORDS_GETTER("medicalorder.field.splitCount"));
};
class SplitResultJsonVO : public JsonVO<SplitReslutDTO::Wrapper>
{
	DTO_INIT(SplitResultJsonVO, JsonVO<SplitReslutDTO::Wrapper>);
};

//停嘱VO
//停嘱操作结果的JSON响应
class StopResultDTO : public oatpp::DTO
{
	DTO_INIT(StopResultDTO, DTO);
	//成功停嘱的数量
	API_DTO_FIELD_DEFAULT(Int32, successCount, ZH_WORDS_GETTER("medicalorder.field.successCount"));
	//停嘱失败的数量
	API_DTO_FIELD_DEFAULT(Int32, failCount, ZH_WORDS_GETTER("medicalorder.field.failCount"));
	//失败医嘱ID和原因
	API_DTO_FIELD_DEFAULT(List<String>, failReasons, ZH_WORDS_GETTER("medicalorder.field.failReasons"));
};
class StopResultJsonVO : public JsonVO<StopResultDTO::Wrapper>
{
	DTO_INIT(StopResultJsonVO, JsonVO<StopResultDTO::Wrapper>);
};

#include OATPP_CODEGEN_BEGIN(DTO)
#endif