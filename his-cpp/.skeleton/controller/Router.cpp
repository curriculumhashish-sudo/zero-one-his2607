
#include "stdafx.h"
#include "Router.h"
#include "ApiHelper.h"

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
	
}
