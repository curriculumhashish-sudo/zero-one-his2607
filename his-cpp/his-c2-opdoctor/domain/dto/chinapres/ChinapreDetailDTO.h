#ifndef __CHINAPREDETAILDTO_H_
#define __CHINAPREDETAILDTO_H_

#include "ChinapreAddDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ChinapreDetailDTO : public ChinapreAddDTO
{
    DTO_INIT(ChinapreDetailDTO, ChinapreAddDTO);
    //处方ID
    API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("prescription.field.id"), true);
};

/**
 * 定义一个处方信息分页传输对象
 */
class ChinapreDetailPageDTO : public PageDTO<ChinapreDetailDTO::Wrapper>
{
    DTO_INIT(ChinapreDetailPageDTO, PageDTO<ChinapreDetailDTO::Wrapper>);
};


#include OATPP_CODEGEN_END(DTO)

#endif // !__CHINAPREDETAILDTO_H_