#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _SAMPLE_FULL_SELECT_DTO_H_
#define _SAMPLE_FULL_SELECT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 样本完整数据选择弹窗DTO
 * 页面说明：列表携带完整样本数据，可直接用于修改表单回填
 */
class SampleDTO : public oatpp::DTO
{
	DTO_INIT(SampleDTO, DTO);

	// 主键ID
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("sample.field.id"), true);
	// Response fields use the exact adm_specimen_definition column names.
	API_DTO_FIELD_DEFAULT(String, tenant_id, "tenant_id");
	API_DTO_FIELD_DEFAULT(String, created_at, "created_at");
	API_DTO_FIELD_DEFAULT(String, updated_at, "updated_at");
	API_DTO_FIELD_DEFAULT(String, created_by, "created_by");
	API_DTO_FIELD_DEFAULT(String, updated_by, "updated_by");
	API_DTO_FIELD_DEFAULT(Int32, is_deleted, "is_deleted");
	API_DTO_FIELD_DEFAULT(String, deleted_at, "deleted_at");
	API_DTO_FIELD_DEFAULT(String, deleted_by, "deleted_by");
	API_DTO_FIELD_DEFAULT(Int32, version, "version");
	API_DTO_FIELD_REQUIRE(String, bus_no, "bus_no", true);
	API_DTO_FIELD_REQUIRE(String, name, "name", true);
	API_DTO_FIELD_DEFAULT(String, py_str, "py_str");
	API_DTO_FIELD_DEFAULT(String, wb_str, "wb_str");
	API_DTO_FIELD_DEFAULT(String, type_enum, "type_enum");
	API_DTO_FIELD_DEFAULT(String, status_enum, "status_enum");
};

using SampleFullSelectDTO = SampleDTO;

class SamplePageDTO : public oatpp::DTO
{
	DTO_INIT(SamplePageDTO, DTO);

	DTO_FIELD(UInt64, page_index) = 1;
	DTO_FIELD_INFO(page_index) {
		info->required = true;
#ifndef LINUX
		info->description = u8"当前页码";
#else
		info->description = "current page index";
#endif
	}

	DTO_FIELD(UInt64, page_size) = 10;
	DTO_FIELD_INFO(page_size) {
		info->required = true;
#ifndef LINUX
		info->description = u8"每页数据条数";
#else
		info->description = "page size";
#endif
	}

	DTO_FIELD(Int64, total);
	DTO_FIELD_INFO(total) {
		info->required = true;
#ifndef LINUX
		info->description = u8"数据的总条数";
#else
		info->description = "total";
#endif
	}

	DTO_FIELD(Int64, pages);
	DTO_FIELD_INFO(pages) {
		info->required = true;
#ifndef LINUX
		info->description = u8"数据的总页数";
#else
		info->description = "pages";
#endif
	}

	DTO_FIELD(List<SampleDTO::Wrapper>, rows) = {};
	DTO_FIELD_INFO(rows) {
#ifndef LINUX
		info->description = u8"当前页数据列表";
#else
		info->description = "page data list";
#endif
	}

public:
	void calcPages()
	{
		const auto size = page_size.getValue(1);
		const auto count = total.getValue(0);
		pages = count / size + (count % size == 0 ? 0 : 1);
	}

	void addData(const SampleDTO::Wrapper& one)
	{
		rows->push_back(one);
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_FULL_SELECT_DTO_H_
