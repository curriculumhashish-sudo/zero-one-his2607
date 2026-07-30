/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#pragma once
#ifndef _BED_MAPPER_
#define _BED_MAPPER_
#include "Mapper.h"
#include "../../domain/do/admissiontransfer/LocationDO.h"

// Mapper for adm_location free-bed query
class BedMapper : public Mapper<LocationDO>
{
public:
    LocationDO mapper(ResultSet* resultSet) const override
    {
        LocationDO data;
        data.setId(resultSet->getString(1));
        data.setName(resultSet->getString(2));
        data.setParent_id(resultSet->getString(3));
        data.setForm_enum(resultSet->getString(4));
        data.setStatus_enum(resultSet->getString(5));
        data.setOrganization_id(resultSet->getString(6));
        return data;
    }
};

#endif // !_BED_MAPPER_
