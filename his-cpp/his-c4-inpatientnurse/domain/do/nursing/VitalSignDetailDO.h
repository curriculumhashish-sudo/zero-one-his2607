#pragma once
#ifndef _VITALSIGNDETAILDO_
#define _VITALSIGNDETAILDO_

#include "../DoInclude.h"

class VitalSignDetailDO : public BaseDO
{
    MYSQL_SYNTHESIZE(std::string, id, Id);
    MYSQL_SYNTHESIZE(std::string, tenantId, TenantId);
    MYSQL_SYNTHESIZE(std::string, createdAt, CreatedAt);
    MYSQL_SYNTHESIZE(std::string, updatedAt, UpdatedAt);
    MYSQL_SYNTHESIZE(std::string, createdBy, CreatedBy);
    MYSQL_SYNTHESIZE(std::string, updatedBy, UpdatedBy);
    MYSQL_SYNTHESIZE(int, isDeleted, IsDeleted);
    MYSQL_SYNTHESIZE(std::string, deletedAt, DeletedAt);
    MYSQL_SYNTHESIZE(std::string, deletedBy, DeletedBy);
    MYSQL_SYNTHESIZE(int, version, Version);


    MYSQL_SYNTHESIZE(std::string, vitalSignId, VitalSignId);
    MYSQL_SYNTHESIZE(std::string, termCode, TermCode);
    MYSQL_SYNTHESIZE(std::string, itemCode, ItemCode);
    MYSQL_SYNTHESIZE(std::string, itemValue, ItemValue);
    MYSQL_SYNTHESIZE(std::string, units, Units);
    MYSQL_SYNTHESIZE(std::string, remark, Remark);


public:
    VitalSignDetailDO() : BaseDO("dia_vital_sign_detail")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD_NULLABLE("tenant_id", "s", tenantId, true);
        MYSQL_ADD_FIELD_NULLABLE("created_at", "s", createdAt, false);
        MYSQL_ADD_FIELD_NULLABLE("updated_at", "s", updatedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("created_by", "s", createdBy, true);
        MYSQL_ADD_FIELD_NULLABLE("updated_by", "s", updatedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("is_deleted", "i", isDeleted, false);
        MYSQL_ADD_FIELD_NULLABLE("deleted_at", "s", deletedAt, true);
        MYSQL_ADD_FIELD_NULLABLE("deleted_by", "s", deletedBy, true);
        MYSQL_ADD_FIELD_NULLABLE("version", "i", version, false);

        MYSQL_ADD_FIELD_NULLABLE("vital_sign_id", "s", vitalSignId, false);
        MYSQL_ADD_FIELD_NULLABLE("term_code", "s", termCode, false);
        MYSQL_ADD_FIELD_NULLABLE("item_code", "s", itemCode, false);
        MYSQL_ADD_FIELD_NULLABLE("item_value", "s", itemValue, false);
        MYSQL_ADD_FIELD_NULLABLE("units", "s", units, true);
        MYSQL_ADD_FIELD_NULLABLE("remark", "s", remark, true);
    }
};

typedef std::shared_ptr<VitalSignDetailDO> PtrVitalSignDetailDO;

#endif
