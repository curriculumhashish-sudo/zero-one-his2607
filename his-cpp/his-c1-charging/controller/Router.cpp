<<<<<<< HEAD
﻿/*
=======
/*
>>>>>>> 29e277be45e89526e4fb67cd3f10e4bac478e7d3
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

<<<<<<< HEAD
=======
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


>>>>>>> 29e277be45e89526e4fb67cd3f10e4bac478e7d3
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
<<<<<<< HEAD
=======

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
>>>>>>> 29e277be45e89526e4fb67cd3f10e4bac478e7d3
	
}
