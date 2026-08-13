#pragma once
#ifndef _PAGE_TCM_HERB_CATALOG_CONTROLLER_H_
#define _PAGE_TCM_HERB_CATALOG_CONTROLLER_H_

#include "../../domain/query/pageTcmHerbCatalog/PageTcmHerbCatalogQuery.h"
#include "../../domain/vo/pageTcmHerbCatalog/PageTcmHerbCatalogVO.h"
#include "../../service/pageTcmHerbCatalog/PageTcmHerbCatalogService.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("pageTcmHerbCatalog.tags.tag")

class PageTcmHerbCatalogController
    : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(PageTcmHerbCatalogController);

public:
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("pageTcmHerbCatalog.page.summary"),
        pageTcmHerbCatalog,
        PageTcmHerbCatalogJsonVO::Wrapper,
        API_TAG,
        addSwaggerExamples(info);
    );

    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/c2-opdoctor/tcm-prescription/page-herb-catalog",
        pageTcmHerbCatalog,
        BODY_DTO(PageTcmHerbCatalogQuery::Wrapper, query),
        execPageTcmHerbCatalog(query, authObject->getPayload())
    );

private:
    using EndpointInfo = oatpp::web::server::api::Endpoint::Info;

    static void addSwaggerExamples(
        const std::shared_ptr<EndpointInfo>& info);

    PageTcmHerbCatalogJsonVO::Wrapper execPageTcmHerbCatalog(
        const PageTcmHerbCatalogQuery::Wrapper& query,
        const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif
