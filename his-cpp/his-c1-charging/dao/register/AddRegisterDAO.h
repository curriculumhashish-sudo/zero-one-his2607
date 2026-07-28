#ifndef __ADD_REGISTER_DAO__
#define __ADD_REGISTER_DAO__
#include"stdafx.h"
#include "BaseDAO.h"
#include"../../domain/dto/register/AddRegisterDTO.h"
class AddRegisterDAO : public BaseDAO
{
public:
	std::string InsertRegister(const AddRegisterDTO::Wrapper &dto)
	{
        // 1. 生成业务流水号（可通过序列或时间戳）
        std::string encounter_bus_no = "EN" + GetCurrentDate() + std::to_string(GetSequence("EN"));
        std::string account_bus_no = "AC" + GetCurrentDate() + std::to_string(GetSequence("AC"));
        std::string charge_bus_no = "CI" + GetCurrentDate() + std::to_string(GetSequence("CI"));
        std::string invoice_bus_no = "IV" + GetCurrentDate() + std::to_string(GetSequence("IV"));
        std::string payment_bus_no = "PMR-" + GetCurrentDate() + std::to_string(GetSequence("PMR"));

        // 2. 生成 UUID（或使用数据库自增）
        std::string encounter_id = GenerateUUID();
        std::string account_id = GenerateUUID();
        std::string charge_id = GenerateUUID();
        std::string invoice_id = GenerateUUID();
        std::string payment_id = GenerateUUID();

        // 3. 获取当前时间
        std::string now = GetCurrentDateTime();

        // 4. 开始构造 SQL
        std::stringstream sql;
        sql << "START TRANSACTION;";

        // 5. 插入就诊主表
        sql << "INSERT INTO adm_encounter ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "patient_id, group_id, bus_no, status_enum, class_enum, "
            << "yb_class_enum, yb_class_text, class_json, "
            << "priority_enum, type_enum, service_type_id, subject_status_enum, "
            << "start_time, end_time, organization_id, display_order, first_enum, "
            << "admit_source_code, in_way_code, reception_time, "
            << "registrar_id, amb_encounter_id, out_way_code) "
            << "VALUES ("
            << "'" << encounter_id << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << dto->patient_id << "', NULL, "
            << "'" << encounter_bus_no << "', "
            << "'ACTIVE', 'AMB', "
            << "NULL, NULL, NULL, "
            << "NULL, NULL, NULL, NULL, "
            << "'" << now << "', NULL, "
            << "'" << dto->organization_id << "', NULL, NULL, "
            << "NULL, NULL, "
            << "'" << now << "', "
            << "'" << dto->operator_id << "', NULL, NULL);";

        // 6. 插入参与者（医生）
        sql << "INSERT INTO adm_encounter_participant ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "encounter_id, role_code, practitioner_id, status_enum, start_time, end_time) "
            << "VALUES ("
            << "'" << GenerateUUID() << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << encounter_id << "', "
            << "'doctor', "
            << "'" << dto->doctor_id << "', "
            << "'ACTIVE', '" << now << "', NULL);";

        // 7. 插入就诊地点（科室）
        sql << "INSERT INTO adm_encounter_location ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "encounter_id, location_id, form_enum, status_enum, start_time, end_time) "
            << "VALUES ("
            << "'" << GenerateUUID() << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << encounter_id << "', "
            << "'" << dto->organization_id << "', "
            << "NULL, 'ACTIVE', '" << now << "', NULL);";

        // 8. 插入账户
        sql << "INSERT INTO adm_account ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "patient_id, encounter_id, type_code, bus_no, name, "
            << "balance, billing_status_enum, contract_no, yb_area_no, "
            << "encounter_flag, insutype, default_flag, status_enum) "
            << "VALUES ("
            << "'" << account_id << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << dto->patient_id << "', "
            << "'" << encounter_id << "', "
            << "'PERSONAL_CASH_ACCOUNT', "
            << "'" << account_bus_no << "', "
            << "'自费账户', "
            << "0.000000, 'open', "
            << "'" << dto->cost_type_id << "', NULL, "
            << "NULL, NULL, 1, 'ACTIVE');";

        // 9. 插入收费项目（挂号费）
        // 需要根据 register_type 查询对应的收费项定义 ID（此处假设 dto->register_def_id 已设置）
        sql << "INSERT INTO adm_charge_item ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "generate_source_enum, context_enum, bus_no, parent_id, "
            << "prescription_no, patient_id, encounter_id, "
            << "service_table, service_id, account_id, definition_id, "
            << "status_enum, total_price, occurrence_time, "
            << "performer_id, performing_org_id, requesting_org_id, "
            << "cost_org_id, quantity_value, quantity_unit, unit_price, "
            << "def_detail_id, base_amount, discount_amount, surcharge_amount, "
            << "system_discount_price, manual_adjusted_price, "
            << "override_reason_code, override_reason_text, "
            << "enterer_id, entered_date, product_table, product_id, "
            << "procedure_id, dispense_table, dispense_id, "
            << "claim_state_enum, print_count, org_id, "
            << "refund_id, condition_id, encounter_diagnosis_id, tcm_flag) "
            << "VALUES ("
            << "'" << charge_id << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'registration', '4', "
            << "'" << charge_bus_no << "', NULL, "
            << "NULL, "
            << "'" << dto->patient_id << "', "
            << "'" << encounter_id << "', "
            << "'adm_healthcare_service', "
            << "'" << dto->register_type_id << "', " 
            << "'" << account_id << "', "
            << "'" << dto->register_def_id << "', " 
            << "'SETTLED', "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "'" << now << "', "
            << "'" << dto->operator_id << "', "
            << "'" << dto->organization_id << "', NULL, "
            << "NULL, "
            << "1.0000, '次', "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "NULL, "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "0.000000, NULL, NULL, NULL, NULL, "
            << "'" << dto->operator_id << "', "
            << "'" << now << "', NULL, NULL, "
            << "NULL, NULL, NULL, "
            << "NULL, NULL, "
            << "'" << dto->organization_id << "', "
            << "NULL, NULL, NULL, NULL);";

        // 10. 插入发票
        sql << "INSERT INTO adm_invoice ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "patient_id, reconciliation_id, payment_id, bus_no, "
            << "bill_no, type_code, cancelled_reason, "
            << "charge_item_ids, payment_terms, amount, "
            << "status_enum, issued_at) "
            << "VALUES ("
            << "'" << invoice_id << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << dto->patient_id << "', NULL, NULL, "
            << "'" << invoice_bus_no << "', "
            << "NULL, 'REG', NULL, "
            << "'" << charge_id << "', NULL, "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "'DRAFT', '" << now << "');";

        // 11. 插入支付流水
        sql << "INSERT INTO fin_payment_reconciliation ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "status_enum, relation_id, payment_no, "
            << "payment_enum, kind_enum, enterer_id, issuer_enum, "
            << "patient_id, practitioner_id, outcome_enum, "
            << "location_id, expiration_date, "
            << "tendered_amount, returned_amount, display_amount, "
            << "print_count, contract_no, "
            << "charge_item_ids, encounter_id, bill_date, "
            << "invoice_id, account_id, yb_clear_flag, cashier_settlement_id) "
            << "VALUES ("
            << "'" << payment_id << "', "
            << "'" << dto->tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << dto->operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'SUCCESS', NULL, "
            << "'" << payment_bus_no << "', "
            << "'CASH', 'OUTPATIENT_CLINIC', "
            << "'" << dto->operator_id << "', NULL, "
            << "'" << dto->patient_id << "', NULL, NULL, "
            << "'" << dto->organization_id << "', NULL, "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "0.000000, "
            << "(SELECT price_value FROM adm_charge_item_definition WHERE id = '" << dto->register_def_id << "'), "
            << "0, "
            << "'" << dto->cost_type_id << "', "
            << "'" << charge_id << "', "
            << "'" << encounter_id << "', "
            << "'" << now << "', "
            << "'" << invoice_id << "', "
            << "'" << account_id << "', "
            << "0, NULL);";
        return sqlSession->executeInsert(sql) > 0);
	}	
};


#endif