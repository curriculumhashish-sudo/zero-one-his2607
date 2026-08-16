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
#include "domain/dto/admissiontransfer/BedDTO.h"

// Mapper for adm_location free-bed query
class BedMapper : public Mapper<BedDTO::Wrapper>
{
public:
    BedDTO::Wrapper mapper(ResultSet* resultSet) const override
    {
		auto data = BedDTO::createShared();
		data->id = resultSet->getString("bed_id").asStdString();
		data->name = resultSet->getString("bed_name").asStdString();
		data->parent_id = resultSet->getString("parent_id").asStdString();
		data->form_enum = resultSet->getString("form_enum").asStdString();
		data->status_enum = resultSet->getString("status_enum").asStdString();
		data->organization_id = resultSet->getString("organization_id").asStdString();
		data->operational_enum = resultSet->getString("operational_enum").asStdString();
		data->version = resultSet->getInt64("version");
		data->display_order = resultSet->getInt("display_order");
		data->house_id = resultSet->getString("house_id").asStdString();
		data->house_name = resultSet->getString("house_name").asStdString();
		data->ward_id = resultSet->getString("ward_id").asStdString();
		data->ward_name = resultSet->getString("ward_name").asStdString();
        return data;
    }
};

#endif // !_BED_MAPPER_
