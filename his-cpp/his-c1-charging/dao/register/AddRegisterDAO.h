#ifndef __ADD_REGISTER_DAO__
#define __ADD_REGISTER_DAO__
#include"stdafx.h"
#include "BaseDAO.h"
#include"../../domain/do/register/AddRegisterDO.h"
class AddRegisterDAO : public BaseDAO
{
public:
    uint64_t InsertRegister(const AddRegisterDO& data)
    {
        // 检查主键是否为空
        if (data.getId().empty()) {
            throw std::runtime_error("Primary field 'id' is empty.");
        }

        // 从 DO 中提取所有需要的值
        const std::string& id = data.getId();
        const std::string& tenant_id = data.getTenant_id();
        const std::string& operator_id = data.getOperator_id();
        const std::string& name = data.getName();
        const std::string& sex = data.getSex();
        const std::string& phone = data.getPhone();
        const std::string& birth_date = data.getBirth_date();
        const std::string& id_card = data.getId_number();
        std::string p_id;
        if(data.getPatient_id()!="")
        {
            p_id = data.getPatient_id();
        }else{
            UuidFacade uf;
            p_id = uf.genUuid();
        }
        const std::string& patient_id = p_id;
        const std::string& doctor_id = data.getDoctor_id();
        const std::string& organization_id = data.getOrganization_id();
        const std::string& now = data.getNow();
        const std::string& encounter_bus_no = data.getEncounter_bus_no();
        const std::string& patient_bus_no = data.getPatient_bus_no();
        const std::string& participant_id = data.getParticipant_id();
        const std::string& location_id = data.getLocation_id();
        const std::string& service_Type_Id = data.getServiceTypeId();
        const std::string& slot_Id = data.getSlotId();
        const std::string& account_Id = data.getAccount_id();
        const std::string& account_bus_no = data.getAccount_bus_no();
        const std::string& contract_no = data.getContractNo();
        uint64_t result;
        // 4. 开始构造 SQL
        std::ostringstream sql1;
        std::ostringstream sql2;
        std::ostringstream sql3;
        std::ostringstream sql4;
        std::ostringstream sql5;
        std::ostringstream sql6;
        // 插入就诊主表
        sql1 << "INSERT INTO adm_encounter ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "patient_id, group_id, bus_no, status_enum, class_enum, "
            << "yb_class_enum, yb_class_text, class_json, "
            << "priority_enum, type_enum, service_type_id, subject_status_enum, "
            << "start_time, end_time, organization_id, display_order, first_enum, "
            << "admit_source_code, in_way_code, reception_time, "
            << "registrar_id, amb_encounter_id, out_way_code) "
            << "VALUES ("
            << "'" << id << "', "
            << "'" << tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << patient_id << "', NULL, "
            << "'" << encounter_bus_no << "', "
            << "'IN_PROGRESS', 'AMB', "
            << "NULL, NULL, NULL, "
            << "NULL, NULL," <<"'" << service_Type_Id<<"'"<< ", NULL, "
            << "'" << now << "', NULL, "
            << "'" << organization_id << "', NULL, NULL, "
            << "NULL, NULL, "
            << "'" << now << "', "
            << "'" << operator_id << "', NULL, NULL);";
        cout << sql1.str() << endl;
        result = sqlSession->executeUpdate(sql1.str());
        cout << result << endl;
        if (result != 1) { return result; }
        sql2 << "INSERT INTO adm_encounter_participant ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "encounter_id, role_code, practitioner_id, status_enum, start_time, end_time) "
            << "VALUES ("
            << "'" << participant_id << "', "
            << "'" << tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << id << "', "
            << "'doctor', "
            << "'" << doctor_id << "', "
            << "'ACTIVE', '" << now << "', NULL);";
        uint64_t participant = sqlSession->executeUpdate(sql2.str());
        cout << participant << endl;
        cout << sql2.str() << endl;
        if (participant != 1){return participant;}

        sql3 << "INSERT INTO adm_encounter_location ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "encounter_id, location_id, form_enum, status_enum, start_time, end_time) "
            << "VALUES ("
            << "'" << location_id << "', "
            << "'" << tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << id << "', "
            << "'" << organization_id << "', "
            << "NULL, 'ACTIVE', '" << now << "', NULL);";
        uint64_t location = sqlSession->executeUpdate(sql3.str());
        cout << location << endl;
        if(location != 1)return location;
        if (data.getPatient_id() == "")
        {
            sql4 << "INSERT INTO adm_patient ("
                << "id,tenant_id,created_at,updated_at,created_by,updated_by,is_deleted,version,bus_no,"
                << "name,gender_enum,birth_date,status_enum,active_flag,temp_flag,id_card,phone)"
                << "VALUES ("
                << "'" << patient_id << "', "
                << "'" << now << "', '" << now << "', "
                << "'" << operator_id << "', NULL, "
                << "0, 0, "
                << "'" << patient_bus_no << "', "
                << "'" << name << "', "
                << "'" << sex << "', "
                << "'" << birth_date << "', "
                << "'" << "ACTIVE" << "', "
                << "1,0,"
                << "'" << id_card << "',"
                << "'" << phone << "');";
           uint64_t patient = sqlSession->executeUpdate(sql4.str());
           cout << patient << endl;
           if (patient != 1)return patient;
        }
        sql5 << "UPDATE wkf_slot SET booked_count=booked_count+1 WHERE id=; "<<slot_Id<<"'" << "'";
        uint64_t slot = sqlSession->executeUpdate(sql5.str());
        cout << slot << endl;
        return slot;
        sql6 << "INSERT INTO adm_account ("
            << "id, tenant_id, created_at, updated_at, created_by, updated_by, "
            << "is_deleted, deleted_at, deleted_by, version, "
            << "patient_id, encounter_id, type_code, bus_no, name, "
            << "balance, billing_status_enum, contract_no, yb_area_no, "
            << "encounter_flag, insutype, default_flag, status_enum) "
            << "VALUES ("
            << "'" << account_Id << "', "
            << "'" << tenant_id << "', "
            << "'" << now << "', '" << now << "', "
            << "'" << operator_id << "', NULL, "
            << "0, NULL, NULL, 0, "
            << "'" << patient_id << "', "
            << "'" << id << "', "
            << "'PERSONAL_CASH_ACCOUNT', " 
            << "'" << account_bus_no << "', "
            << "'自费账户', "
            << "0.000000, 'open', "
            << "'" << contract_no << "', NULL, "  
            << "NULL, NULL, 1, 'ACTIVE');";
        return sqlSession->executeUpdate(sql6.str());
	}

};


#endif