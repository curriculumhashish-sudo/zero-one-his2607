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
#ifndef _PRINT_JOB_DTO_
#define _PRINT_JOB_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PrintJobAddDTO : public oatpp::DTO
{
	DTO_INIT(PrintJobAddDTO, DTO);
	// doc type
	DTO_FIELD(String, docType);
	DTO_FIELD_INFO(docType) {
		info->description = ZH_WORDS_GETTER("print.field.docType");
	}
	// patient name
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("print.field.patientName");
	}
	// treatment item
	DTO_FIELD(String, item);
	DTO_FIELD_INFO(item) {
		info->description = ZH_WORDS_GETTER("print.field.treatmentItem");
	}
	// exec dept
	DTO_FIELD(String, dept);
	DTO_FIELD_INFO(dept) {
		info->description = ZH_WORDS_GETTER("print.field.execDept");
	}
	// doctor name
	DTO_FIELD(String, doctor);
	DTO_FIELD_INFO(doctor) {
		info->description = ZH_WORDS_GETTER("print.field.doctorName");
	}
	// order time
	DTO_FIELD(String, otime);
	DTO_FIELD_INFO(otime) {
		info->description = ZH_WORDS_GETTER("print.field.orderTime");
	}
	// exec time
	DTO_FIELD(String, etime);
	DTO_FIELD_INFO(etime) {
		info->description = ZH_WORDS_GETTER("print.field.execTime");
	}
	// executor
	DTO_FIELD(String, executor);
	DTO_FIELD_INFO(executor) {
		info->description = ZH_WORDS_GETTER("print.field.executor");
	}
	// status
	DTO_FIELD(Int32, status);
	DTO_FIELD_INFO(status) {
		info->description = ZH_WORDS_GETTER("print.field.status");
	}
	// print count
	DTO_FIELD(Int32, printCount);
	DTO_FIELD_INFO(printCount) {
		info->description = ZH_WORDS_GETTER("print.field.printCount");
	}
	// remark
	DTO_FIELD(String, remark);
	DTO_FIELD_INFO(remark) {
		info->description = ZH_WORDS_GETTER("print.field.remark");
	}
};

class PrintJobDTO : public PrintJobAddDTO
{
	DTO_INIT(PrintJobDTO, PrintJobAddDTO);
	// id
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("print.field.id");
	}
};

class PrintJobPageDTO : public PageDTO<PrintJobDTO::Wrapper>
{
	DTO_INIT(PrintJobPageDTO, PageDTO<PrintJobDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_JOB_DTO_
