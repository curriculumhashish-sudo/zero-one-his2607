#ifndef __CHINAPRECONTROLLER_H_
#define __CHINAPRECONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/chinapres/ChinapreQuery.h"
#include "domain/vo/chinapres/ChinapreVO.h"
#include "domain/dto/chinapres/ChinapreAddDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("prescription.tags.tag")
class ChinapreController : public oatpp::web::server::api::ApiController
{
    // 定义控制器访问入口
    API_ACCESS_DECLARE(ChinapreController);
public: 
    // 定义接口
     // 3.1 定义分页查询接口描述
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("prescription.query.summary"), queryChinapre, ChinapreQuery, ChinapreDetailPageJsonVO::Wrapper, API_TAG);
    // 3.2 定义分页查询接口处理
    API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chinapre", queryChinapre, ChinapreQuery, execQueryChinapre(query));

    // 3.1 定义处方新增接口描述
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.add.summary"), addPrescription, StringJsonVO::Wrapper, API_TAG);
    // 3.2 定义处方新增接口处理
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/chinapre/add", addPrescription, BODY_DTO(ChinapreAddDTO::Wrapper, dto), execAddChinapre(dto, authObject->getPayload()));

    // 3.1 定义删除接口描述
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("prescription.remove.summary"), removePrescription, BooleanJsonVO::Wrapper, API_TAG);
    // 3.2 定义删除接口处理
    API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/chinapre/{id}", removePrescription, PATH(String, id), execRemoveChinapre(id));

private: // 定义接口执行函数
    ChinapreDetailPageJsonVO::Wrapper execQueryChinapre(const ChinapreQuery::Wrapper& query);

    
    StringJsonVO::Wrapper execAddChinapre(const ChinapreAddDTO::Wrapper& dto, const PayloadDTO& payload);
    BooleanJsonVO::Wrapper execRemoveChinapre(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !__CHINAPRECONTROLLER_H_