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
#ifndef _MEDICATION_COLLECTION_SERVICE_H_
#define _MEDICATION_COLLECTION_SERVICE_H_
#include "domain/query/medicationcollection/PagePendingDispenseSummaryLinesQuery.h"
#include "domain/dto/medicationcollection/PagePendingDispenseSummaryLinesDTO.h"
#include "domain/dto/medicationcollection/CreateMedicationSummaryDTO.h"
#include "domain/dto/medicationcollection/SummarySlipDTO.h"

class MedicationCollectionService
{
public:
	PagePendingDispenseSummaryLinesDTO::Wrapper listPending(const PagePendingDispenseSummaryLinesQuery::Wrapper& query);
	CreateMedicationSummaryResponseDTO::Wrapper summarize(const CreateMedicationSummaryDTO::Wrapper& dto,const std::string& operatorId);
	SubmitSummaryResponseDTO::Wrapper submitSummary(const SubmitSummaryRequestDTO::Wrapper& dto,const std::string& operatorId);
	CancelSummaryResponseDTO::Wrapper cancelSummary(const CancelSummaryRequestDTO::Wrapper& dto,const std::string& operatorId);
};

#endif // !_MEDICATION_COLLECTION_SERVICE_H_
