#ifndef __CHINAPREVO_H_
#define __CHINAPREVO_H_

#include "../../dto/chinapres/ChinapreDetailDTO.h"
#include OATPP_CODEGEN_BEGIN(DTO)


 // 定义一个租户信息分页显示JsonVO对象，用于响应给客户端
 
class ChinapreDetailPageJsonVO : public JsonVO<ChinapreDetailPageDTO::Wrapper>
{
    DTO_INIT(ChinapreDetailPageJsonVO, JsonVO<ChinapreDetailPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !__CHINAPREVO_H_