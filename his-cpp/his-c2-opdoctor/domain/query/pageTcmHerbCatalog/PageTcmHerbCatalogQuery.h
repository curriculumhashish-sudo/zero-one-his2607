#pragma once

#ifndef _PAGE_TCM_HERB_CATALOG_QUERY_H_
#define _PAGE_TCM_HERB_CATALOG_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PageTcmHerbCatalogQuery : public oatpp::DTO
{
    DTO_INIT(PageTcmHerbCatalogQuery, DTO);

    API_DTO_FIELD(
        String,
        scope,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.scope"),
        true,
        "outpatient"
    );

    API_DTO_FIELD(
        UInt64,
        pageIndex,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.pageIndex"),
        true,
        1
    );

    API_DTO_FIELD(
        UInt64,
        pageSize,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.pageSize"),
        true,
        20
    );

    API_DTO_FIELD(
        String,
        keyword,
        ZH_WORDS_GETTER("pageTcmHerbCatalog.field.keyword"),
        false,
        ""
    );
};

#include OATPP_CODEGEN_END(DTO)

#endif
