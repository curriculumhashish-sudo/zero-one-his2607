#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: sqdxy
 @Date: 2026/07/20

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
#ifndef _PRINT_JOB_QUERY_
#define _PRINT_JOB_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrintJobQuery : public PageQuery
{
	DTO_INIT(PrintJobQuery, PageQuery);
	DTO_FIELD(String, docType);
	DTO_FIELD_INFO(docType) { info->description = ZH_WORDS_GETTER("print.field.docType"); }
	DTO_FIELD(String, pname);
	DTO_FIELD_INFO(pname) { info->description = ZH_WORDS_GETTER("print.field.patientName"); }
	DTO_FIELD(String, dept);
	DTO_FIELD_INFO(dept) { info->description = ZH_WORDS_GETTER("print.field.execDept"); }
	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) { info->description = ZH_WORDS_GETTER("print.field.doctorName"); }
	DTO_FIELD(Int32, st);
	DTO_FIELD_INFO(st) { info->description = ZH_WORDS_GETTER("print.field.status"); }
	DTO_FIELD(String, tmfrom);
	DTO_FIELD_INFO(tmfrom) { info->description = ZH_WORDS_GETTER("print.field.tmfrom"); }
	DTO_FIELD(String, tmto);
	DTO_FIELD_INFO(tmto) { info->description = ZH_WORDS_GETTER("print.field.tmto"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_JOB_QUERY_
