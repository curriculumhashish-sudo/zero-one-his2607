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
#ifndef _ADMISSION_TRANSFER_SERVICE_H_
#define _ADMISSION_TRANSFER_SERVICE_H_
#include "domain/dto/admissiontransfer/ExecuteInpatientDischargeDTO.h"
#include "domain/dto/admissiontransfer/ClearDischargedBedDTO.h"

class AdmissionTransferService
{
public:
	ExecuteInpatientDischargeResponseDTO::Wrapper discharge(const ExecuteInpatientDischargeDTO::Wrapper& dto,const std::string& operatorId);
	ClearDischargedBedResponseDTO::Wrapper clearBed(const ClearDischargedBedDTO::Wrapper& dto,const std::string& operatorId);
};

#endif // !_ADMISSION_TRANSFER_SERVICE_H_
