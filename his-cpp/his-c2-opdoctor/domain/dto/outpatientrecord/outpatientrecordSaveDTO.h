#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c2-opdoctor
 @Date: 2026/07/28

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _outpatientrecordSAVEDTO_H_
#define _outpatientrecordSAVEDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Outpatient record save DTO
 *
 * Saves (insert or update) an outpatient medical record document
 * into the emr_document table.
 * If outpatientrecordId is empty, a new record is created;
 * otherwise the existing record is updated.
 */
class outpatientrecordSaveDTO : public oatpp::DTO
{
	DTO_INIT(outpatientrecordSaveDTO, DTO);

	// Record ID, empty means insert new, non-empty means update existing
	API_DTO_FIELD_REQUIRE(
		String,
		outpatientrecordId,
		ZH_WORDS_GETTER("outpatientrecord.field.outpatientrecordId"),
		false);

	// Patient ID, identifies the patient this record belongs to
	API_DTO_FIELD_REQUIRE(
		String,
		patientId,
		ZH_WORDS_GETTER("outpatientrecord.field.patientId"),
		true);

	// Visit/Encounter ID, identifies which outpatient visit this record is for
	API_DTO_FIELD_REQUIRE(
		String,
		visitId,
		ZH_WORDS_GETTER("outpatientrecord.field.visitId"),
		true);

	// Document title, auto-generated if not provided
	API_DTO_FIELD_REQUIRE(
		String,
		title,
		ZH_WORDS_GETTER("outpatientrecord.field.title"),
		false);

	// Template ID, identifies which emr_template was used as the basis for this record
	API_DTO_FIELD_REQUIRE(
		String,
		templateId,
		ZH_WORDS_GETTER("outpatientrecord.field.templateId"),
		true);

	// Document content in Tiptap/ProseMirror JSON format
	API_DTO_FIELD_REQUIRE(
		String,
		content,
		ZH_WORDS_GETTER("outpatientrecord.field.content"),
		true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_outpatientrecordSAVEDTO_H_
