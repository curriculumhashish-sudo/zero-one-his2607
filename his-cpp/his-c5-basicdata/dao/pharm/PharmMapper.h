#ifndef _PHARMMAPPER_H_
#define _PHARMMAPPER_H_

#include "Mapper.h"
#include "domain/do/pharm/PharmDO.h"

// 基类模板参数改为 PtrPharmDO，和 DAO 层、项目规范对齐
class PharmMapper : public Mapper<PtrPharmDO>
{
private:
	// 处理数据库空值字段，空值返回空字符串
	std::string readNullableString(ResultSet* resultSet, const std::string& column) const;

public:
	// 返回值同步改为 PtrPharmDO
	PtrPharmDO mapper(ResultSet* resultSet) const override;
};

#endif // !_PHARMMAPPER_H_