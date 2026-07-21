#pragma once

#ifndef _FREQUENCYDETAILDTO_H_
#define _FREQUENCYDETAILDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 频次详情DTO
 */
class FrequencyDetailDTO : public oatpp::DTO
{
	DTO_INIT(FrequencyDetailDTO, DTO);
	// 主键ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("frequency.field.id"), true);
	// 频次编码（QD/BID/TID）
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("frequency.field.code"), true);
	// 频次名称（每日一次、每日两次）
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("frequency.field.name"), true);
	// 状态 0停用 1启用
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("frequency.field.status"), true);
	// 周期类型：日周期/周周期
	API_DTO_FIELD_REQUIRE(String, cycleType, ZH_WORDS_GETTER("frequency.field.cycleType"), true);
	// 执行时点 多个逗号分隔 08:00,18:00
	API_DTO_FIELD_REQUIRE(String, execTime, ZH_WORDS_GETTER("frequency.field.execTime"), true);
	// 日计数
	API_DTO_FIELD_REQUIRE(Int32, dayCount, ZH_WORDS_GETTER("frequency.field.dayCount"), false);
	// 执行周期
	API_DTO_FIELD_REQUIRE(Int32, execCycle, ZH_WORDS_GETTER("frequency.field.execCycle"), false);
	// 日间隔
	API_DTO_FIELD_REQUIRE(Int32, dayInterval, ZH_WORDS_GETTER("frequency.field.dayInterval"), false);
	// 总次数
	API_DTO_FIELD_REQUIRE(Int32, totalTimes, ZH_WORDS_GETTER("frequency.field.totalTimes"), false);
	// 适用场景：全部/门诊、住院
	API_DTO_FIELD_REQUIRE(String, scene, ZH_WORDS_GETTER("frequency.field.scene"), true);
	// 跨日策略：允许/不允许跨日
	API_DTO_FIELD_REQUIRE(String, crossDayPolicy, ZH_WORDS_GETTER("frequency.field.crossDayPolicy"), true);
	// 排序号
	API_DTO_FIELD_REQUIRE(Int32, sort, ZH_WORDS_GETTER("frequency.field.sort"), true);
};

/**
 * 频次分页DTO
 */
class FrequencyDetailPageDTO : public PageDTO<FrequencyDetailDTO::Wrapper>
{
	DTO_INIT(FrequencyDetailPageDTO, PageDTO<FrequencyDetailDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYDETAILDTO_H_