#ifndef _PHARM_MAPPER_H_
#define _PHARM_MAPPER_H_

#include "Mapper.h"
#include "domain/do/pharm/PharmDO.h"

class PharmMapper : public Mapper<PharmDO>
{
private:
	// 处理数据库空值字段，空值返回空字符串
	std::string readNullableString(ResultSet* resultSet, const std::string& column) const;

public:
	PharmDO mapper(ResultSet* resultSet) const override;
};

#endif // !_PHARM_MAPPER_H_