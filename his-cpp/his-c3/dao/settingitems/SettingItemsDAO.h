#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author:
 @Date: 2026/07/21

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
#ifndef _SETTINGITEMS_H_
#define _SETTINGITEMS_H_
#include "BaseDAO.h"
#include "../../domain/do/settingitems/SettingItemsDO.h"

/**
 * Data access object for setting items.
 */
class SettingItemsDAO : public BaseDAO
{
public:
	// Soft delete a configuration item
	int softDelete(const std::string& id, const std::string& deletedBy);

	// Existing add/update methods are reused directly
};
#endif
