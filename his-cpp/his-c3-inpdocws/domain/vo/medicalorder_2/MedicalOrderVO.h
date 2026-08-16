#pragma once

/*
 Copyright Zero One Star. All rights reserved.
 MedicalOrder VO wrappers
 @Author: PKQ
 @Date: 2026/07/20 00:00:00

 Licensed under the Apache License, Version 2.0 (the "License");
 ...
*/
#ifndef _MEDICALORDERVO_H_
#define _MEDICALORDERVO_H_

#include "../../GlobalInclude.h"
#include "../../dto/medicalorder_2/MedOrderDTO.h"
#include "../../dto/medicalorder_2/OrderMergeDTO.h"
#include "../../dto/medicalorder_2/OrderSplitDTO.h"
#include "../../dto/medicalorder_2/OrderStopDTO.h"
#include "domain/vo/JsonVO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

// Page query VO
class MedOrderPageJsonVO : public JsonVO<MedOrderPageDTO::Wrapper>
{
	DTO_INIT(MedOrderPageJsonVO, JsonVO<MedOrderPageDTO::Wrapper>);
};

// Merge result: groupId + optional error
class MergeResultDTO : public oatpp::DTO
{
	DTO_INIT(MergeResultDTO, DTO);
	API_DTO_FIELD_DEFAULT(String, groupId, ZH_WORDS_GETTER("medicalorder.field.groupId"));
	API_DTO_FIELD_DEFAULT(String, errorMessage, ZH_WORDS_GETTER("medicalorder.field.errorMessage"));
};

class MergeResultJsonVO : public JsonVO<MergeResultDTO::Wrapper>
{
	DTO_INIT(MergeResultJsonVO, JsonVO<MergeResultDTO::Wrapper>);

};

// Split result: count + optional error
class SplitResultDTO : public oatpp::DTO
{
	DTO_INIT(SplitResultDTO, DTO);
	API_DTO_FIELD_DEFAULT(Int32, splitCount, ZH_WORDS_GETTER("medicalorder.field.splitCount"));
	API_DTO_FIELD_DEFAULT(String, errorMessage, ZH_WORDS_GETTER("medicalorder.field.errorMessage"));
};
class SplitResultJsonVO : public JsonVO<SplitResultDTO::Wrapper>
{
	DTO_INIT(SplitResultJsonVO, JsonVO<SplitResultDTO::Wrapper>);

};

// Stop result: success/fail counts + structured fail details
class StopResultDTO : public oatpp::DTO
{
	DTO_INIT(StopResultDTO, DTO);
	API_DTO_FIELD_DEFAULT(Int32, successCount, ZH_WORDS_GETTER("medicalorder.field.successCount"));
	API_DTO_FIELD_DEFAULT(Int32, failCount, ZH_WORDS_GETTER("medicalorder.field.failCount"));
	API_DTO_FIELD_DEFAULT(List<Object<StopFailDetailDTO>>, failDetails, ZH_WORDS_GETTER("medicalorder.field.failDetails"));
};
class StopResultJsonVO : public JsonVO<StopResultDTO::Wrapper>
{
	DTO_INIT(StopResultJsonVO, JsonVO<StopResultDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif
