/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#include "MedicationCollectionController.h"

PagePendingDispenseSummaryLinesJsonVO::Wrapper
MedicationCollectionController::execQuerySummaryOrderItems(
	const PagePendingDispenseSummaryLinesQuery::Wrapper& query)
{
	return {};
}

CreateMedicationSummaryJsonVO::Wrapper MedicationCollectionController::execSummaryMedication(
	const CreateMedicationSummaryDTO::Wrapper& dto)
{
	return {};
}

SubmitSummaryJsonVO::Wrapper MedicationCollectionController::execSubmitSummary(
	const SubmitSummaryRequestDTO::Wrapper& dto)
{
	auto jvo = SubmitSummaryJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}

CancelSummaryJsonVO::Wrapper MedicationCollectionController::execCancelSummary(
	const CancelSummaryRequestDTO::Wrapper& dto)
{
	auto jvo = CancelSummaryJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}
