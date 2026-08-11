#ifndef __CHINAPRES2CONTROLLER_H_
#define __CHINAPRES2CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/chinapres2/Chinapres2AddDTO.h"
#include "domain/dto/chinapres2/Chinapres2UpdateDTO.h"
#include "domain/dto/chinapres2/Chinapres2AddRespDTO.h"
#include "domain/dto/chinapres2/Chinapres2RemoveDTO.h"
#include "domain/dto/chinapres2/Chinapres2RemoveRespDTO.h"
#include "domain/vo/chinapres2/Chinapres2AddRespJsonVO.h"
#include "domain/vo/chinapres2/Chinapres2RemoveRespJsonVO.h"
#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("chinapres2.tags.tag")
class Chinapres2Controller : public oatpp::web::server::api::ApiController
{
    // 定义控制器访问入口
    API_ACCESS_DECLARE(Chinapres2Controller);
public:
    // 定义接口

     // 3.1 定义分页查询接口描述
   // API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("chinapres2.query.summary"), queryChinapre, Chinapres2Query, Chinapres2DetailPageJsonVO::Wrapper, API_TAG);
    // 3.2 定义分页查询接口处理
   // API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chinapres2", queryChinapre, Chinapres2Query, execQueryChinapre(query));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("chinapres2.add.summary"), addPrescription,
        Chinapres2AddRespJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/c2-opdoctor/add", addPrescription,
        BODY_DTO(Chinapres2AddDTO::Wrapper, dto),
        execAddChinapre(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("chinapres2.update.summary"), updatePrescription,
        Chinapres2AddRespJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/c2-opdoctor/update", updatePrescription,
        BODY_DTO(Chinapres2UpdateDTO::Wrapper, dto),
        execUpdateChinapre(dto, authObject->getPayload()));

    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("chinapres2.remove.enhanced.summary"), removePrescriptionEnhanced,
        Chinapres2RemoveRespJsonVO::Wrapper, API_TAG);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/c2-opdoctor/remove", removePrescriptionEnhanced,
        BODY_DTO(Chinapres2RemoveDTO::Wrapper, dto),
        execRemoveChinapreEnhanced(dto, authObject->getPayload()));

private: // 定义接口执行函数
    //Chinapres2DetailPageJsonVO::Wrapper execQueryChinapre(const Chinapres2Query::Wrapper& query);


    Chinapres2AddRespJsonVO::Wrapper execAddChinapre(const Chinapres2AddDTO::Wrapper& dto, const PayloadDTO& payload);
    Chinapres2AddRespJsonVO::Wrapper execUpdateChinapre(const Chinapres2UpdateDTO::Wrapper& dto, const PayloadDTO& payload);
    Chinapres2RemoveRespJsonVO::Wrapper execRemoveChinapreEnhanced(const Chinapres2RemoveDTO::Wrapper& dto, const PayloadDTO& payload);
    BooleanJsonVO::Wrapper execRemoveChinapre(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !__CHINAPRES2CONTROLLER_H_