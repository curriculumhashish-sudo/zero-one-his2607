
#ifndef _DEPT_CONTROLLER_
#define _DEPT_CONTROLLER_

#include "domain/dto/dept/deptDTO.h"
#include "domain/query/dept/DeptQuery.h"
#include "domain/vo/dept/DeptVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("dept.tag")

class DeptController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DeptController);

public:
	// 定义新增接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.add.summary"),
		addDept,
		DeptAddJsonVO::Wrapper,
		API_TAG
	);
	// 定义新增接口处理
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/createDepartment",
		addDept,
		BODY_DTO(DeptAddDTO::Wrapper, dto),
		execAddDept(dto, authObject->getPayload())
	);

	// 定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.modify.summary"),
		modifyDept,
		DeptModifyJsonVO::Wrapper,
		API_TAG
	);
	// 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/editDepartment",
		modifyDept,
		BODY_DTO(DeptModifyDTO::Wrapper, dto),
		execModifyDept(dto, authObject->getPayload())
	);

	// 定义删除接口描述
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.remove.summary"),
		removeDept,
		DeptDeleteJsonVO::Wrapper,
		API_TAG
	);
	// 定义删除接口处理
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/deleteDepartment",
		removeDept,
		BODY_DTO(DeptDeleteQuery::Wrapper, query),
		execRemoveDept(query)
	);

private:
	DeptAddJsonVO::Wrapper execAddDept(const DeptAddDTO::Wrapper& dto, const PayloadDTO& payload);
	DeptModifyJsonVO::Wrapper execModifyDept(const DeptModifyDTO::Wrapper& dto, const PayloadDTO& payload);
	DeptDeleteJsonVO::Wrapper execRemoveDept(const DeptDeleteQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DEPT_CONTROLLER_
