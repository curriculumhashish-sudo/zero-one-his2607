#pragma once
#ifndef _MEDICATIONREQUESTDETAILDO1_H_
#define _MEDICATIONREQUESTDETAILDO1_H_

#include "../DoInclude.h"

class MedicationRequestDetailDO1 : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, requestId, RequestId);           // 关联处方主表ID（= med_medication_request.id）
    MYSQL_SYNTHESIZE(string, medicationId, MedicationId);     // 药品ID
    MYSQL_SYNTHESIZE(double, quantity, Quantity);             // 数量
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);             // 单位编码
    MYSQL_SYNTHESIZE(double, dose, Dose);                     // 单次剂量
    MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);     // 剂量单位编码
    MYSQL_SYNTHESIZE(int, sortNo, SortNo);                    // 排序号

public:
    MedicationRequestDetailDO1() : BaseDO("med_medication_request_detail")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("request_id", "s", requestId);
        MYSQL_ADD_FIELD("medication_id", "s", medicationId);
        MYSQL_ADD_FIELD("quantity", "d", quantity);
        MYSQL_ADD_FIELD_NULLABLE("unit_code", "s", unitCode, false);
        MYSQL_ADD_FIELD_NULLABLE("dose", "d", dose, false);
        MYSQL_ADD_FIELD_NULLABLE("dose_unit_code", "s", doseUnitCode, false);
        MYSQL_ADD_FIELD_NULLABLE("sort_no", "i", sortNo, false);
    }
};

typedef std::shared_ptr<MedicationRequestDetailDO1> PtrMedicationRequestDetailDO;

#endif // !_MEDICATIONREQUESTDETAILDO1_H_