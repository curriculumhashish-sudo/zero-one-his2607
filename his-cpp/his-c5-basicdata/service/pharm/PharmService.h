#ifndef _PHARMSERVICE_H_
#define _PHARMSERVICE_H_

#include "domain/query/pharm/PharmQuery.h"
#include "domain/dto/pharm/PharmAddDTO.h"
#include "domain/dto/pharm/PharmModifyDTO.h"
#include "domain/vo/pharm/PharmPageVO.h"
#include "domain/vo/pharm/PharmVO.h"
#include "domain/GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

class PharmService
{
public:
    // Paginated query for pharmacy/warehouse list
    PharmPageJsonVO::Wrapper getPage(const PharmQuery::Wrapper& query);
    // Pharmacy/warehouse name list: id + name, optional type filter
    PharmNameListJsonVO::Wrapper nameList(const oatpp::String& type);
    // Add a new pharmacy/warehouse
    StringJsonVO::Wrapper add(const PharmAddDTO::Wrapper& dto);
    // Modify pharmacy/warehouse info
    StringJsonVO::Wrapper modify(const PharmModifyDTO::Wrapper& dto);
    // Logical delete pharmacy/warehouse
    StringJsonVO::Wrapper remove(uint64_t id);
    // Update pharmacy/warehouse status
    StringJsonVO::Wrapper updateStatus(uint64_t id, int status);
};

#endif // !_PHARMSERVICE_H_