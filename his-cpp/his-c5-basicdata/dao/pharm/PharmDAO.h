#ifndef _PHARMDAO_H_
#define _PHARMDAO_H_

#include "BaseDAO.h"
#include "domain/do/pharm/PharmDO.h"
#include "domain/query/pharm/PharmQuery.h"
#include <list>

class PharmDAO : public BaseDAO
{
public:
    // Paginated query for pharmacy/warehouse list
    std::list<PtrPharmDO> selectPage(const PharmQuery::Wrapper& query);
    // Count records matching filter conditions
    uint64_t count(const PharmQuery::Wrapper& query);
    // Pharmacy/warehouse name list: query id + name, with optional type filter
    std::list<PtrPharmDO> nameList(const std::string& type);
    // Insert a new pharmacy/warehouse
    int insert(const PharmDO& data);
    // Modify pharmacy/warehouse info
    int update(const PharmDO& data);
    // Logical delete of pharmacy/warehouse
    int remove(uint64_t id);
    // Update pharmacy/warehouse status
    int updateStatus(uint64_t id, int status);

private:
    // ��̬������ѯ�������������
    std::string queryConditionBuilder(const PharmQuery::Wrapper& query, SqlParams& params);
};

#endif // !_PHARMDAO_H_