#pragma once
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
#ifndef _PHARMMAPPER_H_
#define _PHARMMAPPER_H_

#include "Mapper.h"
#include "domain/do/pharm/PharmDO.h"

/**
 * PharmMapper - Full ResultSet mapper for adm_location pharmacy/warehouse rows.
 */
class PharmMapper : public Mapper<PtrPharmDO>
{
public:
    PtrPharmDO mapper(ResultSet* resultSet) const override;
};

#endif // !_PHARMMAPPER_H_
