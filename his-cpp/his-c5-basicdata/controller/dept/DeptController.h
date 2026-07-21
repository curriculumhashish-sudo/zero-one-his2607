
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
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.add.summary"),
		addDept,
		DeptAddJsonVO::Wrapper,
		API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/createDepartment",
		addDept,
		BODY_DTO(DeptAddDTO::Wrapper, dto),
		execAddDept(dto)
	);

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.modify.summary"),
		modifyDept,
		DeptModifyJsonVO::Wrapper,
		API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/editDepartment",
		modifyDept,
		BODY_DTO(DeptModifyDTO::Wrapper, dto),
		execModifyDept(dto)
	);

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dept.remove.summary"),
		removeDept,
		DeptDeleteJsonVO::Wrapper,
		API_TAG
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/department/deleteDepartment",
		removeDept,
		BODY_DTO(DeptDeleteQuery::Wrapper, query),
		execRemoveDept(query)
	);

private:
	DeptAddJsonVO::Wrapper execAddDept(const DeptAddDTO::Wrapper& dto);
	DeptModifyJsonVO::Wrapper execModifyDept(const DeptModifyDTO::Wrapper& dto);
	DeptDeleteJsonVO::Wrapper execRemoveDept(const DeptDeleteQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DEPT_CONTROLLER_
