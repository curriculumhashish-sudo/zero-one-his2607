

#include "domain/do/ward/WardLocationDO.h"
#include <list>
#include "BaseDAO.h"
#include "domain/do/ward/WardDO.h"
#include "domain/query/ward/WardListQuery.h"
#include "domain/do/ward/WardLocationDO.h"
class WardDAO : public BaseDAO
{
public:
    std::list<WardDO> selectNameList(
        const std::string& organizationId);

    std::list<WardDO> selectWardList(
        const WardListQuery::Wrapper& query);

    int insertLocation(
        const WardLocationDO& data);



    std::string selectTenantIdByOrganizationId(
        const std::string& organizationId);

    PtrWardLocationDO selectLocationById(
        const std::string& id,
        const std::string& organizationId);

    std::string generateLocationBusNo(
        const std::string& tenantId,
        const std::string& parentBusNo);

    int insertLocation(
        const WardLocationDO& data);

    PtrWardLocationDO selectLocationById(
        const std::string& id
    );
    std::string selectOrganizationTenantIdForUpdate(
        const std::string& organizationId);

    PtrWardLocationDO selectLocationByIdForUpdate(
        const std::string& id);

    bool existsSiblingName(
        const std::string& organizationId,
        const std::string& formEnum,
        const std::string& parentId,
        const std::string& name);

    std::string generateLocationBusNo(
        const std::string& tenantId,
        const std::string& parentBusNo);
};