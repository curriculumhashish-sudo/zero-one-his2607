#include "stdafx.h"
#include "Router.h"
#include "ApiHelper.h"
#include "dept/DeptController.h"
#include "dept/DeptBatchStatusController.h"
#include "pat/Patcontroller.h"
#include "pat/PatientController.h"
#include "organization/OrganizationController.h"
#include "sup/SupplierController.h"

#include "ward/WardController.h"
#include "pharm/PharmController.h"
#include "room/RoomController.h"
#include "bed/BedController.h"

#include "sup/SupplierAddUpdateController.h"
#include "sup/SupController.h"

// If the macro for disabling Swagger documentation is defined
#ifdef CLOSE_SWAGGER_DOC
// Simplified binding controller macro definition
#define ROUTER_SIMPLE_BIND(__CLASS__) \
router->addController(__CLASS__::createShared())
#else
// Simplified binding controller macro definition
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
	ROUTER_SIMPLE_BIND(DeptController);
	ROUTER_SIMPLE_BIND(DeptBatchStatusController);
	ROUTER_SIMPLE_BIND(PatController);
	ROUTER_SIMPLE_BIND(PatientController);
	ROUTER_SIMPLE_BIND(OrganizationController);
	ROUTER_SIMPLE_BIND(SupplierAddUpdateController);
	// Supplier management routes
	ROUTER_SIMPLE_BIND(SupplierController);
	// Supplier name query routes
	ROUTER_SIMPLE_BIND(SupController);
	ROUTER_SIMPLE_BIND(WardController);
	ROUTER_SIMPLE_BIND(PharmController);
	ROUTER_SIMPLE_BIND(RoomController);
	ROUTER_SIMPLE_BIND(BedController);
}
