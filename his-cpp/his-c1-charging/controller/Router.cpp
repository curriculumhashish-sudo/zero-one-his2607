/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/12/03 14:58:34

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
#include "Router.h"
#include "ApiHelper.h"
#include "refund/RefundGetPatientController.h"
#include "refund/RefundGetItemController.h"
#include "refund/RefundVerifyRefundController.h"
#include "refund/RefundExecuteRefundController.h"
#include "refund/RefundPreparePrintController.h"
#include "refund/RefundConfirmPrintController.h"

#include "../controller/register/ChargeConfirmController.h"
#include "../controller/register/QuitNumberController.h"
#include "../controller/register/PrintRegisterReceiptController.h"
#include"register/GetSeeController.h"
#include"register/GetTodayAppointController.h"
#include"register/AddRegisterContorller.h"

#include "charge/PatientQueueController.h"
#include "charge/ChargeItemController.h"
#include "charge/SettlementController.h"
#include "charge/PrintController.h"

#include"settle/GetPatientListController.h"
#include"settle/GetCategoryDetailsController.h"
#include"settle/GetTotalFeesController.h"

#include "enroll/EnrollController.h"
#include"enroll/admission.h"

#include"pay_manage/Pay_ManageGetPatientDetailsController.h"
#include"pay_manage/Pay_ManageGetPatientPayRecordsController.h"
#include"pay_manage/Pay_ManageGetPatientQueueController.h"

#include"pay_manage/PrepaymentController.h"

#include"settle/GetExpenseDetailsListController.h"

#include"settle/GetHistorySettleListController.h"

#include"settle/SettleController.h"

// If the macro for disabling Swagger documentation is defined
#ifdef CLOSE_SWAGGER_DOC
// Simplified controller binding macro
#define ROUTER_SIMPLE_BIND(__CLASS__) \
router->addController(__CLASS__::createShared())
#else
// Simplified controller binding macro (with Swagger support)
#define ROUTER_SIMPLE_BIND(__CLASS__) \
BIND_CONTROLLER(docEndpoints, router, __CLASS__)
#endif

Router::Router(Endpoints* docEndpoints, HttpRouter* router)
{
	this->docEndpoints = docEndpoints;
	this->router = router;
}

void Router::initRouter()
{
	// #TIP: Add system extension route definitions below

    ROUTER_SIMPLE_BIND(AdmissionController);

	ROUTER_SIMPLE_BIND(ChargeConfirmController);
	ROUTER_SIMPLE_BIND(QuitNumberController);
	ROUTER_SIMPLE_BIND(PrintRegisterReceiptController);
	ROUTER_SIMPLE_BIND(GetSeeController);
	ROUTER_SIMPLE_BIND(GetTodayAppointController);
	ROUTER_SIMPLE_BIND(AddRegisterContorller);
	// 患者队列
	ROUTER_SIMPLE_BIND(PatientQueueController);
	// 缴费项管理
	ROUTER_SIMPLE_BIND(ChargeItemController);
	// 结算管理
	ROUTER_SIMPLE_BIND(SettlementController);
	// 打印管理
	ROUTER_SIMPLE_BIND(PrintController);

	ROUTER_SIMPLE_BIND(GetPatientListController);
	ROUTER_SIMPLE_BIND(GetCategoryDetailsController);
	ROUTER_SIMPLE_BIND(GetTotalFeesController);


	ROUTER_SIMPLE_BIND(EnrollController);

	
	ROUTER_SIMPLE_BIND(RefundGetPatientController);
	ROUTER_SIMPLE_BIND(RefundGetItemController);
	ROUTER_SIMPLE_BIND(RefundVerifyRefundController);
	ROUTER_SIMPLE_BIND(RefundExecuteRefundController);
	ROUTER_SIMPLE_BIND(RefundPreparePrintController);
	ROUTER_SIMPLE_BIND(RefundConfirmPrintController);

	ROUTER_SIMPLE_BIND(Pay_ManageGetPatientDetailsController);
	ROUTER_SIMPLE_BIND(Pay_ManageGetPatientPayRecordsController);
	ROUTER_SIMPLE_BIND(Pay_ManageGetPatientQueueController);
	ROUTER_SIMPLE_BIND(PrepaymentController);

	ROUTER_SIMPLE_BIND(GetExpenseDetailsListController);

	ROUTER_SIMPLE_BIND(GetHistorySettleListController);
	ROUTER_SIMPLE_BIND(SettleController);
}
