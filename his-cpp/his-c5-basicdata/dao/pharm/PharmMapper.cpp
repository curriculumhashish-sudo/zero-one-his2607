/*
 Copyright Zero One Star. All rights reserved.

 @Author: Lfff
 @Date: 2026/08/01 10:00:00

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
#include "stdafx.h"
#include "PharmMapper.h"

PtrPharmDO PharmMapper::mapper(ResultSet* resultSet) const
{
    auto data = std::make_shared<PharmDO>();

    data->setId(resultSet->getString("id"));
    data->setTenantId(resultSet->getString("tenant_id"));
    data->setName(resultSet->getString("name"));
    data->setBusNo(resultSet->getString("bus_no"));
    data->setStatusEnum(resultSet->getString("status_enum"));
    data->setOperationalEnum(resultSet->getString("operational_enum"));
    data->setFormEnum(resultSet->getString("form_enum"));
    data->setModeEnum(resultSet->getString("mode_enum"));
    data->setTypeJson(resultSet->getString("type_json"));
    data->setOrganizationId(resultSet->getString("organization_id"));
    data->setDisplayOrder(resultSet->getInt("display_order"));
    data->setPyStr(resultSet->getString("py_str"));
    data->setWbStr(resultSet->getString("wb_str"));
    data->setParentId(resultSet->getString("parent_id"));
    data->setCreatedBy(resultSet->getString("created_by"));
    data->setUpdatedBy(resultSet->getString("updated_by"));
    data->setCreatedAt(resultSet->getString("created_at"));
    data->setUpdatedAt(resultSet->getString("updated_at"));

    return data;
}
