#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/26 00:00:00

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
#ifndef _NURSING_SERVICE_
#define _NURSING_SERVICE_
#include "domain/vo/nursing/NursingRecordVO.h"
#include "domain/query/nursing/NursingRecordQuery.h"
#include "domain/dto/nursing/NursingRecordDTO.h"

class NursingDAO;

class NursingService
{
public:

    NursingRecordListResponseDTO::Wrapper listRecords(const NursingRecordQuery::Wrapper& query);

    NursingRecordSaveResponseDTO::Wrapper saveRecords(const NursingRecordSaveRequestDTO::Wrapper& dto);

private:

    std::string generateBusNo(NursingDAO& dao);
};

#endif // !_NURSING_SERVICE_
