#pragma once

#ifndef _PHARM_DO_
#define _PHARM_DO_
#include "../DoInclude.h"

// sys_pharm
class PharmDO : public BaseDO
{
    MYSQL_SYNTHESIZE(uint64_t, id, Id);
    MYSQL_SYNTHESIZE(std::string, name, Name);
    MYSQL_SYNTHESIZE(std::string, type, Type);
    MYSQL_SYNTHESIZE(int, status, Status);
    MYSQL_SYNTHESIZE(int, sort, Sort);
    MYSQL_SYNTHESIZE(std::string, remark, Remark);
    MYSQL_SYNTHESIZE(std::string, create_by, Create_By);
    MYSQL_SYNTHESIZE(std::string, update_by, Update_By);
    MYSQL_SYNTHESIZE(std::string, create_time, Create_Time);
    MYSQL_SYNTHESIZE(std::string, update_time, Update_Time);
public:
    PharmDO() : BaseDO("sys_pharm")
    {
        MYSQL_ADD_FIELD_PK("id", "ull", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("status", "i", status);
        MYSQL_ADD_FIELD("sort", "i", sort);
        MYSQL_ADD_FIELD_NULLABLE("type", "s", type, true);
        MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
        MYSQL_ADD_FIELD_NULLABLE("create_by", "s", create_by, true);
        MYSQL_ADD_FIELD_NULLABLE("update_by", "s", update_by, true);
        MYSQL_ADD_FIELD_NULLABLE("create_time", "s", create_time, true);
        MYSQL_ADD_FIELD_NULLABLE("update_time", "s", update_time, true);
    }
};
typedef std::shared_ptr<PharmDO> PtrPharmDO;

// med_medication_definition
class MedicationDefinitionDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, name, Name);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    MYSQL_SYNTHESIZE(string, categoryCode, CategoryCode);
    MYSQL_SYNTHESIZE(string, merchandiseName, MerchandiseName);
    MYSQL_SYNTHESIZE(string, totalVolume, TotalVolume);
    MYSQL_SYNTHESIZE(string, doseFormCode, DoseFormCode);
    MYSQL_SYNTHESIZE(string, purchaseUnitCode, PurchaseUnitCode);
    MYSQL_SYNTHESIZE(string, splitUnitCode, SplitUnitCode);
    MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
    MYSQL_SYNTHESIZE(string, packageUnitCode, PackageUnitCode);
    MYSQL_SYNTHESIZE(string, stockUnitCode, StockUnitCode);
    MYSQL_SYNTHESIZE(string, approvalNumber, ApprovalNumber);
    MYSQL_SYNTHESIZE(string, manufacturerText, ManufacturerText);
    MYSQL_SYNTHESIZE(string, supplyId, SupplyId);
    MYSQL_SYNTHESIZE(string, itemMinQuantity, ItemMinQuantity);
    MYSQL_SYNTHESIZE(string, itemMaxQuantity, ItemMaxQuantity);
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    MYSQL_SYNTHESIZE(string, rxFlag, RxFlag);
    MYSQL_SYNTHESIZE(string, antibioticFlag, AntibioticFlag);
    MYSQL_SYNTHESIZE(string, basicFlag, BasicFlag);
    MYSQL_SYNTHESIZE(string, activeFlag, ActiveFlag);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
public:
    MedicationDefinitionDO() : BaseDO("med_medication_definition")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        MYSQL_ADD_FIELD("category_code", "s", categoryCode);
        MYSQL_ADD_FIELD("merchandise_name", "s", merchandiseName);
        MYSQL_ADD_FIELD("total_volume", "s", totalVolume);
        MYSQL_ADD_FIELD("dose_form_code", "s", doseFormCode);
        MYSQL_ADD_FIELD("purchase_unit_code", "s", purchaseUnitCode);
        MYSQL_ADD_FIELD("split_unit_code", "s", splitUnitCode);
        MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
        MYSQL_ADD_FIELD("package_unit_code", "s", packageUnitCode);
        MYSQL_ADD_FIELD("stock_unit_code", "s", stockUnitCode);
        MYSQL_ADD_FIELD("approval_number", "s", approvalNumber);
        MYSQL_ADD_FIELD("manufacturer_text", "s", manufacturerText);
        MYSQL_ADD_FIELD("supply_id", "s", supplyId);
        MYSQL_ADD_FIELD("item_min_quantity", "s", itemMinQuantity);
        MYSQL_ADD_FIELD("item_max_quantity", "s", itemMaxQuantity);
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        MYSQL_ADD_FIELD("rx_flag", "s", rxFlag);
        MYSQL_ADD_FIELD("antibiotic_flag", "s", antibioticFlag);
        MYSQL_ADD_FIELD("basic_flag", "s", basicFlag);
        MYSQL_ADD_FIELD("active_flag", "s", activeFlag);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
    }
};
typedef std::shared_ptr<MedicationDefinitionDO> PtrMedicationDefinitionDO;

// med_medication_request
class MedicationRequestDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, prescriptionNo, PrescriptionNo);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
    MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, requesterId, RequesterId);
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, quantity, Quantity);
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
    MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
    MYSQL_SYNTHESIZE(string, rateCode, RateCode);
    MYSQL_SYNTHESIZE(string, dose, Dose);
    MYSQL_SYNTHESIZE(string, doseUnitCode, DoseUnitCode);
    MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
    MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
    MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
    MYSQL_SYNTHESIZE(string, sortNumber, SortNumber);
    MYSQL_SYNTHESIZE(string, remark, Remark);
public:
    MedicationRequestDO() : BaseDO("med_medication_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("prescription_no", "s", prescriptionNo);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("medication_id", "s", medicationId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("requester_id", "s", requesterId);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("quantity", "s", quantity);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("method_code", "s", methodCode);
        MYSQL_ADD_FIELD("rate_code", "s", rateCode);
        MYSQL_ADD_FIELD("dose", "s", dose);
        MYSQL_ADD_FIELD("dose_unit_code", "s", doseUnitCode);
        MYSQL_ADD_FIELD("dosage_instruction", "s", dosageInstruction);
        MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
        MYSQL_ADD_FIELD("trace_no", "s", traceNo);
        MYSQL_ADD_FIELD("sort_number", "s", sortNumber);
        MYSQL_ADD_FIELD("remark", "s", remark);
    }
};
typedef std::shared_ptr<MedicationRequestDO> PtrMedicationRequestDO;

// med_medication_dispense
class MedicationDispenseDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, dispenseEnum, DispenseEnum);
    MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
    MYSQL_SYNTHESIZE(string, medReqId, MedReqId);
    MYSQL_SYNTHESIZE(string, patientId, PatientId);
    MYSQL_SYNTHESIZE(string, encounterId, EncounterId);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, quantity, Quantity);
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
    MYSQL_SYNTHESIZE(string, dispenseQuantity, DispenseQuantity);
    MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
    MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
    MYSQL_SYNTHESIZE(string, preparerId, PreparerId);
    MYSQL_SYNTHESIZE(string, dispenseTime, DispenseTime);
    MYSQL_SYNTHESIZE(string, practitionerId, PractitionerId);
    MYSQL_SYNTHESIZE(string, dosageInstruction, DosageInstruction);
    MYSQL_SYNTHESIZE(string, methodCode, MethodCode);
    MYSQL_SYNTHESIZE(string, frequencyCode, FrequencyCode);
public:
    MedicationDispenseDO() : BaseDO("med_medication_dispense")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("dispense_enum", "s", dispenseEnum);
        MYSQL_ADD_FIELD("medication_id", "s", medicationId);
        MYSQL_ADD_FIELD("med_req_id", "s", medReqId);
        MYSQL_ADD_FIELD("patient_id", "s", patientId);
        MYSQL_ADD_FIELD("encounter_id", "s", encounterId);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("quantity", "s", quantity);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("dispense_quantity", "s", dispenseQuantity);
        MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
        MYSQL_ADD_FIELD("trace_no", "s", traceNo);
        MYSQL_ADD_FIELD("preparer_id", "s", preparerId);
        MYSQL_ADD_FIELD("dispense_time", "s", dispenseTime);
        MYSQL_ADD_FIELD("practitioner_id", "s", practitionerId);
        MYSQL_ADD_FIELD("dosage_instruction", "s", dosageInstruction);
        MYSQL_ADD_FIELD("method_code", "s", methodCode);
        MYSQL_ADD_FIELD("frequency_code", "s", frequencyCode);
    }
};
typedef std::shared_ptr<MedicationDispenseDO> PtrMedicationDispenseDO;

// wkf_inventory_item
class InventoryItemDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
    MYSQL_SYNTHESIZE(string, itemId, ItemId);
    MYSQL_SYNTHESIZE(string, name, Name);
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    MYSQL_SYNTHESIZE(string, inventoryStatusEnum, InventoryStatusEnum);
    MYSQL_SYNTHESIZE(string, supplierId, SupplierId);
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
    MYSQL_SYNTHESIZE(string, quantity, Quantity);
    MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
    MYSQL_SYNTHESIZE(string, productionDate, ProductionDate);
    MYSQL_SYNTHESIZE(string, expirationDate, ExpirationDate);
    MYSQL_SYNTHESIZE(string, locationId, LocationId);
    MYSQL_SYNTHESIZE(string, locationStoreId, LocationStoreId);
    MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
public:
    InventoryItemDO() : BaseDO("wkf_inventory_item")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("item_table", "s", itemTable);
        MYSQL_ADD_FIELD("item_id", "s", itemId);
        MYSQL_ADD_FIELD("name", "s", name);
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        MYSQL_ADD_FIELD("inventory_status_enum", "s", inventoryStatusEnum);
        MYSQL_ADD_FIELD("supplier_id", "s", supplierId);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("quantity", "s", quantity);
        MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
        MYSQL_ADD_FIELD("production_date", "s", productionDate);
        MYSQL_ADD_FIELD("expiration_date", "s", expirationDate);
        MYSQL_ADD_FIELD("location_id", "s", locationId);
        MYSQL_ADD_FIELD("location_store_id", "s", locationStoreId);
        MYSQL_ADD_FIELD("trace_no", "s", traceNo);
    }
};
typedef std::shared_ptr<InventoryItemDO> PtrInventoryItemDO;

// wkf_supply_request
class SupplyRequestDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, categoryEnum, CategoryEnum);
    MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
    MYSQL_SYNTHESIZE(string, itemId, ItemId);
    MYSQL_SYNTHESIZE(string, itemQuantity, ItemQuantity);
    MYSQL_SYNTHESIZE(string, totalQuantity, TotalQuantity);
    MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
    MYSQL_SYNTHESIZE(string, lotNumber, LotNumber);
    MYSQL_SYNTHESIZE(string, traceNo, TraceNo);
    MYSQL_SYNTHESIZE(string, supplierId, SupplierId);
    MYSQL_SYNTHESIZE(string, sourceTypeEnum, SourceTypeEnum);
    MYSQL_SYNTHESIZE(string, sourceLocationId, SourceLocationId);
    MYSQL_SYNTHESIZE(string, purposeTypeEnum, PurposeTypeEnum);
    MYSQL_SYNTHESIZE(string, purposeLocationId, PurposeLocationId);
    MYSQL_SYNTHESIZE(string, applicantId, ApplicantId);
    MYSQL_SYNTHESIZE(string, applyTime, ApplyTime);
    MYSQL_SYNTHESIZE(string, approverId, ApproverId);
    MYSQL_SYNTHESIZE(string, approvalTime, ApprovalTime);
    MYSQL_SYNTHESIZE(string, price, Price);
    MYSQL_SYNTHESIZE(string, totalPrice, TotalPrice);
    MYSQL_SYNTHESIZE(string, profitLossPrice, ProfitLossPrice);
    MYSQL_SYNTHESIZE(string, remake, Remake);
public:
    SupplyRequestDO() : BaseDO("wkf_supply_request")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("category_enum", "s", categoryEnum);
        MYSQL_ADD_FIELD("item_table", "s", itemTable);
        MYSQL_ADD_FIELD("item_id", "s", itemId);
        MYSQL_ADD_FIELD("item_quantity", "s", itemQuantity);
        MYSQL_ADD_FIELD("total_quantity", "s", totalQuantity);
        MYSQL_ADD_FIELD("unit_code", "s", unitCode);
        MYSQL_ADD_FIELD("lot_number", "s", lotNumber);
        MYSQL_ADD_FIELD("trace_no", "s", traceNo);
        MYSQL_ADD_FIELD("supplier_id", "s", supplierId);
        MYSQL_ADD_FIELD("source_type_enum", "s", sourceTypeEnum);
        MYSQL_ADD_FIELD("source_location_id", "s", sourceLocationId);
        MYSQL_ADD_FIELD("purpose_type_enum", "s", purposeTypeEnum);
        MYSQL_ADD_FIELD("purpose_location_id", "s", purposeLocationId);
        MYSQL_ADD_FIELD("applicant_id", "s", applicantId);
        MYSQL_ADD_FIELD("apply_time", "s", applyTime);
        MYSQL_ADD_FIELD("approver_id", "s", approverId);
        MYSQL_ADD_FIELD("approval_time", "s", approvalTime);
        MYSQL_ADD_FIELD("price", "s", price);
        MYSQL_ADD_FIELD("total_price", "s", totalPrice);
        MYSQL_ADD_FIELD("profit_loss_price", "s", profitLossPrice);
        MYSQL_ADD_FIELD("remake", "s", remake);
    }
};
typedef std::shared_ptr<SupplyRequestDO> PtrSupplyRequestDO;

// cus_price_adjustment
class PriceAdjustmentDO : public BaseDO
{
    MYSQL_SYNTHESIZE(string, id, Id);
    MYSQL_SYNTHESIZE(string, busNo, BusNo);
    MYSQL_SYNTHESIZE(string, itemTable, ItemTable);
    MYSQL_SYNTHESIZE(string, itemId, ItemId);
    MYSQL_SYNTHESIZE(string, newPurchaseAmount, NewPurchaseAmount);
    MYSQL_SYNTHESIZE(string, newSellingAmount, NewSellingAmount);
    MYSQL_SYNTHESIZE(string, adjustmentReason, AdjustmentReason);
    MYSQL_SYNTHESIZE(string, adjustmentTime, AdjustmentTime);
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    MYSQL_SYNTHESIZE(string, operatorId, OperatorId);
    MYSQL_SYNTHESIZE(string, orgId, OrgId);
public:
    PriceAdjustmentDO() : BaseDO("cus_price_adjustment")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("bus_no", "s", busNo);
        MYSQL_ADD_FIELD("item_table", "s", itemTable);
        MYSQL_ADD_FIELD("item_id", "s", itemId);
        MYSQL_ADD_FIELD("new_purchase_amount", "s", newPurchaseAmount);
        MYSQL_ADD_FIELD("new_selling_amount", "s", newSellingAmount);
        MYSQL_ADD_FIELD("adjustment_reason", "s", adjustmentReason);
        MYSQL_ADD_FIELD("adjustment_time", "s", adjustmentTime);
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        MYSQL_ADD_FIELD("operator_id", "s", operatorId);
        MYSQL_ADD_FIELD("org_id", "s", orgId);
    }
};
typedef std::shared_ptr<PriceAdjustmentDO> PtrPriceAdjustmentDO;

#endif // !_PHARM_DO_
