#pragma once
/*
版权所有 Zero One Star. 保留所有权利。

 @作者: his-c2-opdoctor
 @日期: 2026/07/28

 根据 Apache 许可证 2.0 版本（以下简称“许可证”）授权；
 除非遵守本许可证，否则你不得使用此文件。
 你可以通过以下地址获取许可证副本：

	  https://www.apache.org/licenses/LICENSE-2.0

 除非适用法律要求或书面同意，本软件按“原样”分发，
 不附带任何明示或暗示的担保条件。
 请查阅许可证了解权限限制与免责条款。
*/
#ifndef _outpatientrecordSAVEDTO_H_
#define _outpatientrecordSAVEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 门诊病历保存传输对象（DTO）
 *
 * 用于新增/更新门诊电子病历文档，对应数据库 emr_document 表
 * 若 outpatientrecordId 为空：执行新增病历操作；
 * 若该字段有值：对已有病历执行更新操作。
 */
class outpatientrecordSaveDTO : public oatpp::DTO
{
	DTO_INIT(outpatientrecordSaveDTO, DTO);

	// 病历主键ID，空值代表新增，有值则更新已有病历
	API_DTO_FIELD_REQUIRE(
		String,
		outpatientrecordId,
		ZH_WORDS_GETTER("outpatientrecord.field.outpatientrecordId"),
		false);

	// 患者ID，用于标识该病历归属的就诊人
	API_DTO_FIELD_REQUIRE(
		String,
		patientId,
		ZH_WORDS_GETTER("outpatientrecord.field.patientId"),
		true);

	// 就诊记录ID，标识本次门诊就诊单次流水
	API_DTO_FIELD_REQUIRE(
		String,
		visitId,
		ZH_WORDS_GETTER("outpatientrecord.field.visitId"),
		true);

	// 病历文档标题，不传参时后端自动生成标题
	API_DTO_FIELD_REQUIRE(
		String,
		title,
		ZH_WORDS_GETTER("outpatientrecord.field.title"),
		false);

	// 病历模板ID，标识生成这份病历所使用的电子病历模板
	API_DTO_FIELD_REQUIRE(
		String,
		templateId,
		ZH_WORDS_GETTER("outpatientrecord.field.templateId"),
		true);

	// 病历正文内容，存储格式为 Tiptap/ProseMirror 富文本JSON
	API_DTO_FIELD_REQUIRE(
		String,
		content,
		ZH_WORDS_GETTER("outpatientrecord.field.content"),
		true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_outpatientrecordSAVEDTO_H_