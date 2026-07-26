#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: shiye
 @Date: 2026/07/26

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

#ifndef _SUPPLY_DELIVERY_DO_
#define _SUPPLY_DELIVERY_DO_

#include "../DoInclude.h"

// Database entity for workflow supply-delivery records.
class SupplyDeliveryDO : public BaseDO
{
    // Primary key.
    MYSQL_SYNTHESIZE(string, id, Id);
    // Tenant identifier.
    MYSQL_SYNTHESIZE(string, tenant_id, Tenant_id);
    // Creation time.
    MYSQL_SYNTHESIZE(string, created_at, Created_at);
    // Last update time.
    MYSQL_SYNTHESIZE(string, updated_at, Updated_at);
    // Creator identifier.
    MYSQL_SYNTHESIZE(string, created_by, Created_by);
    // Last updater identifier.
    MYSQL_SYNTHESIZE(string, updated_by, Updated_by);
    // Soft-delete flag.
    MYSQL_SYNTHESIZE(bool, is_deleted, Is_deleted);
    // Deletion time.
    MYSQL_SYNTHESIZE(string, deleted_at, Deleted_at);
    // Deleter identifier.
    MYSQL_SYNTHESIZE(string, deleted_by, Deleted_by);
    // Optimistic lock version.
    MYSQL_SYNTHESIZE(int, version, Version);
    // Upstream supply-request line identifier.
    MYSQL_SYNTHESIZE(string, request_id, Request_id);
    // Related medication-summary identifier.
    MYSQL_SYNTHESIZE(string, summary_id, Summary_id);
    // Delivery status code.
    MYSQL_SYNTHESIZE(string, status_enum, Status_enum);
    // Related patient identifier.
    MYSQL_SYNTHESIZE(string, patient_id, Patient_id);
    // Delivery business type code.
    MYSQL_SYNTHESIZE(string, type_enum, Type_enum);
    // Source item table name.
    MYSQL_SYNTHESIZE(string, item_table, Item_table);
    // Source item identifier.
    MYSQL_SYNTHESIZE(string, item_id, Item_id);
    // Predecessor table name.
    MYSQL_SYNTHESIZE(string, based_on_table, Based_on_table);
    // Predecessor record identifiers.
    MYSQL_SYNTHESIZE(string, based_on_ids, Based_on_ids);
    // Measurement unit code.
    MYSQL_SYNTHESIZE(string, unit_code, Unit_code);
    // Delivered quantity.
    MYSQL_SYNTHESIZE(double, quantity, Quantity);
    // Delivery condition description.
    MYSQL_SYNTHESIZE(string, condition, Condition);
    // Inventory lot number.
    MYSQL_SYNTHESIZE(string, lot_number, Lot_number);
    // Trace number.
    MYSQL_SYNTHESIZE(string, trace_no, Trace_no);
    // Supplier identifier.
    MYSQL_SYNTHESIZE(string, supplier_id, Supplier_id);
    // Delivering practitioner identifier.
    MYSQL_SYNTHESIZE(string, practitioner_id, Practitioner_id);
    // Delivery occurrence time.
    MYSQL_SYNTHESIZE(string, occurrence_time, Occurrence_time);
    // Receiving practitioner identifier.
    MYSQL_SYNTHESIZE(string, receiver_id, Receiver_id);
    // Receipt time.
    MYSQL_SYNTHESIZE(string, receive_time, Receive_time);

public:
    SupplyDeliveryDO() : BaseDO("wkf_supply_delivery")
    {
        MYSQL_ADD_FIELD_PK("id", "s", id);
        MYSQL_ADD_FIELD("tenant_id", "s", tenant_id);
        MYSQL_ADD_FIELD("created_at", "s", created_at);
        MYSQL_ADD_FIELD("updated_at", "s", updated_at);
        MYSQL_ADD_FIELD("created_by", "s", created_by);
        MYSQL_ADD_FIELD("updated_by", "s", updated_by);
        MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
        MYSQL_ADD_FIELD("deleted_at", "s", deleted_at);
        MYSQL_ADD_FIELD("deleted_by", "s", deleted_by);
        MYSQL_ADD_FIELD("version", "i", version);
        MYSQL_ADD_FIELD("request_id", "s", request_id);
        MYSQL_ADD_FIELD("summary_id", "s", summary_id);
        MYSQL_ADD_FIELD("status_enum", "s", status_enum);
        MYSQL_ADD_FIELD("patient_id", "s", patient_id);
        MYSQL_ADD_FIELD("type_enum", "s", type_enum);
        MYSQL_ADD_FIELD("item_table", "s", item_table);
        MYSQL_ADD_FIELD("item_id", "s", item_id);
        MYSQL_ADD_FIELD("based_on_table", "s", based_on_table);
        MYSQL_ADD_FIELD("based_on_ids", "s", based_on_ids);
        MYSQL_ADD_FIELD("unit_code", "s", unit_code);
        MYSQL_ADD_FIELD("quantity", "d", quantity);
        MYSQL_ADD_FIELD("condition", "s", condition);
        MYSQL_ADD_FIELD("lot_number", "s", lot_number);
        MYSQL_ADD_FIELD("trace_no", "s", trace_no);
        MYSQL_ADD_FIELD("supplier_id", "s", supplier_id);
        MYSQL_ADD_FIELD("practitioner_id", "s", practitioner_id);
        MYSQL_ADD_FIELD("occurrence_time", "s", occurrence_time);
        MYSQL_ADD_FIELD("receiver_id", "s", receiver_id);
        MYSQL_ADD_FIELD("receive_time", "s", receive_time);
        setIs_deleted(false);
        setVersion(0);
    }
};


typedef std::shared_ptr<SupplyDeliveryDO> PtrSupplyDeliveryDO;

#endif // !_SUPPLY_DELIVERY_DO_
