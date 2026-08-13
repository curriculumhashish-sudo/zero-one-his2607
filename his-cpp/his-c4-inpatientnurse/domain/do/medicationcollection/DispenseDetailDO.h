#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/18 22:40:00

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
#ifndef _DISPENSEDETAIL_DO_
#define _DISPENSEDETAIL_DO_
#include "../DoInclude.h"

/**
 * Query result entity of dispense details
 * (med_medication_dispense JOIN med_medication_definition).
 */
class DispenseDetailDO
{
	// detail id
	MYSQL_SYNTHESIZE(string, id, Id);
	MYSQL_SYNTHESIZE(string, medicationId, MedicationId);
	// medication name (from med_medication_definition)
	MYSQL_SYNTHESIZE(string, name, Name);
	// requested quantity
	MYSQL_SYNTHESIZE(double, quantity, Quantity);
	// unit code (specification)
	MYSQL_SYNTHESIZE(string, unitCode, UnitCode);
	MYSQL_SYNTHESIZE(string, specification, Specification);
	MYSQL_SYNTHESIZE(string, packageDisplay, PackageDisplay);
	MYSQL_SYNTHESIZE(double, unitPrice, UnitPrice);
	MYSQL_SYNTHESIZE(double, totalPrice, TotalPrice);
};

#endif // !_DISPENSEDETAIL_DO_
