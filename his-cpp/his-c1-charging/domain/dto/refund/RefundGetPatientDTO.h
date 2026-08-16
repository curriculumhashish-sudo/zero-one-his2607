#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xbzml
 @Date: 2026/07/20 00:00:00

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
#ifndef _REFUND_GET_PATIENT_DTO_
#define _REFUND_GET_PATIENT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RefundGetPatientDTO : public oatpp::DTO
{
    DTO_INIT(RefundGetPatientDTO, DTO);

    // 14 fields mapped to the refund patient list response JSON
    // Each field description points to the corresponding zh-dict.yaml entry

    DTO_FIELD(String, encounterId);
    DTO_FIELD_INFO(encounterId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterId");
    };
    DTO_FIELD(String, encounterBusNo);
    DTO_FIELD_INFO(encounterBusNo) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.encounterBusNo");
    };
    DTO_FIELD(String, patientId);
    DTO_FIELD_INFO(patientId) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.patientId");
    };
    DTO_FIELD(String, patientName);
    DTO_FIELD_INFO(patientName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.patientName");
    };
    DTO_FIELD(String, genderCode);
    DTO_FIELD_INFO(genderCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.genderCode");
    };
    DTO_FIELD(String, genderEnumText);
    DTO_FIELD_INFO(genderEnumText) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.genderEnumText");
    };
    DTO_FIELD(String, ageText);
    DTO_FIELD_INFO(ageText) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.ageText");
    };
    DTO_FIELD(String, queueLabelCode);
    DTO_FIELD_INFO(queueLabelCode) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.queueLabelCode");
    };
    DTO_FIELD(String, chargeItemStatusSummary);
    DTO_FIELD_INFO(chargeItemStatusSummary) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.chargeItemStatusSummary");
    };
    DTO_FIELD(String, organizationName);
    DTO_FIELD_INFO(organizationName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.organizationName");
    };
    DTO_FIELD(String, doctorName);
    DTO_FIELD_INFO(doctorName) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.doctorName");
    };
    DTO_FIELD(String, startTime);
    DTO_FIELD_INFO(startTime) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.startTime");
    };
    DTO_FIELD(String, totalAmount);
    DTO_FIELD_INFO(totalAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.totalAmount");
    };
    DTO_FIELD(String, paidAmount);
    DTO_FIELD_INFO(paidAmount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.paidAmount");
    };
};

// 分页响应 DTO — 字段名与 sample-json 严格一一对应，不使用框架 PageDTO
// sample-json 响应: {content:[{...}], totalCount:1, pageIndex:1, pageSize:1, pages:1}
class RefundGetPatientPageDTO : public oatpp::DTO
{
    DTO_INIT(RefundGetPatientPageDTO, DTO);

    // 当前页数据列表 — 对应 sample-json: "content"
    DTO_FIELD(oatpp::List<RefundGetPatientDTO::Wrapper>, content);
    DTO_FIELD_INFO(content) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.content");
    };
    // 总记录数 — 对应 sample-json: "totalCount"（数字类型）
    DTO_FIELD(UInt64, totalCount);
    DTO_FIELD_INFO(totalCount) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.totalCount");
    };
    // 当前页码 — 对应 sample-json: "pageIndex"
    DTO_FIELD(UInt64, pageIndex) = 1;
    DTO_FIELD_INFO(pageIndex) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.pageIndex");
    };
    // 每页条数 — 对应 sample-json: "pageSize"
    DTO_FIELD(UInt64, pageSize) = 10;
    DTO_FIELD_INFO(pageSize) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.pageSize");
    };
    // 总页数 — 对应 sample-json: "pages"
    DTO_FIELD(UInt64, pages);
    DTO_FIELD_INFO(pages) {
        info->description = ZH_WORDS_GETTER("xbzml.charging.refund.field.pages");
    };

public:
    // 追加一条数据（供 Service 层使用）
    void addData(RefundGetPatientDTO::Wrapper one) {
        this->content->push_back(one);
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_REFUND_GET_PATIENT_DTO_
