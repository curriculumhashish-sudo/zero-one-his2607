#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_VO_H_
#define _PAGE_TCM_HERB_CATALOG_VO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PageTcmHerbCatalogItemVO : public oatpp::DTO
{
    DTO_INIT(PageTcmHerbCatalogItemVO, DTO);

    API_DTO_FIELD(String, id,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.id"), true,
        "323999010000000001");
    API_DTO_FIELD(String, code,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.code"), true,
        "MD0030");
    API_DTO_FIELD(String, name,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.name"), true,
        "Huang Qi");
    API_DTO_FIELD(String, specification,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.specification"), true,
        "decoction pieces");
    API_DTO_FIELD(Float64, defaultDose,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.defaultDose"), true,
        10.0);
    API_DTO_FIELD(String, doseUnit,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.doseUnit"), true,
        "g");
    API_DTO_FIELD(Float64, retailPrice,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.retailPrice"), true,
        0.085);
    API_DTO_FIELD(String, categoryCode,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.categoryCode"), true,
        "4");
    API_DTO_FIELD(String, category,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.category"), true,
        "TCM_HERB");
    API_DTO_FIELD(Float64, inventoryQuantity,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.inventoryQuantity"),
        true, 5000.0);
    API_DTO_FIELD(String, inventoryUnit,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.inventoryUnit"), true,
        "g");
};

using PageTcmHerbCatalogPageDTO =
    PageDTO<PageTcmHerbCatalogItemVO::Wrapper>;

class PageTcmHerbCatalogJsonVO
    : public JsonVO<oatpp::Object<PageTcmHerbCatalogPageDTO>>
{
    DTO_INIT(
        PageTcmHerbCatalogJsonVO,
        JsonVO<oatpp::Object<PageTcmHerbCatalogPageDTO>>
    );

public:
    PageTcmHerbCatalogJsonVO()
    {
        data = PageTcmHerbCatalogPageDTO::createShared();
    }
};

#include OATPP_CODEGEN_END(DTO)

#endif
