#ifndef _outpatientrecordCONTROLLER_H_
#define _outpatientrecordCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "../../domain/dto/outpatientrecord/outpatientrecordSaveDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("outpatientrecord.tags.tag")

class outpatientrecordController
	: public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(outpatientrecordController);

public:

	// ±£´æÃÅÕï²¡Àú
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("outpatientrecord.save.summary"),
		saveoutpatientrecord,
		StringJsonVO::Wrapper,
		API_TAG);

	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/outpatientrecord/save",
		saveoutpatientrecord,
		BODY_DTO(outpatientrecordSaveDTO::Wrapper, dto),
		execSaveoutpatientrecord(dto, authObject->getPayload()));

private:

	StringJsonVO::Wrapper
		execSaveoutpatientrecord(
			const outpatientrecordSaveDTO::Wrapper& dto,
			const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif