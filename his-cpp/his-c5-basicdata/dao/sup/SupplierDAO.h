#ifndef _SUPPLIER_DAO_H_
#define _SUPPLIER_DAO_H_

#include "BaseDAO.h"
#include <list>
#include <string>

class SupplierDAO : public BaseDAO {
public:
	//批量更新供应商状态
	int batchUpdateStatus(const std::list<std::string>& ids, const std::string& statusEnum);
	//批量删除供应商
	int batchSoftDelete(const std::list<std::string>& ids, const std::string& deleteBy, const std::string& deleteAt);
};

#endif //!_SUPPLIER_DAO_H_