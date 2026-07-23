#include "stdafx.h"
#include "Router.h"
#include "ApiHelper.h"
#include "dept/DeptController.h"
#include "pat/PatController.h"
#include "pat/PatientController.h"
#include "organization/OrganizationController.h"
#include "sup/SupplierController.h"

#include "ward/WardController.h"
#include "pat/PatController.h"
#include "pharm/PharmController.h"

// 如果定义了关闭Swagger文档宏
#include "sup/SupplierAddUpdateController.h"

// If the macro for disabling Swagger documentation is defined
#ifdef CLOSE_SWAGGER_DOC
// 简化绑定控制器宏定义
#define ROUTER_SIMPLE_BIND(__CLASS__) \
router->addController(__CLASS__::createShared())
#else
// 简化绑定控制器宏定义
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
	//#TIP :系统扩展路由定义，写在这个后面
	ROUTER_SIMPLE_BIND(WardController);
	ROUTER_SIMPLE_BIND(PatController);
	ROUTER_SIMPLE_BIND(PharmController);
	// #TIP: Add system extension route definitions below
	ROUTER_SIMPLE_BIND(DeptController);
	ROUTER_SIMPLE_BIND(PatController);
	ROUTER_SIMPLE_BIND(PatientController);
	ROUTER_SIMPLE_BIND(OrganizationController);
	
	ROUTER_SIMPLE_BIND(SupplierAddUpdateController);
	// Supplier management routes
	ROUTER_SIMPLE_BIND(SupplierController);
}
