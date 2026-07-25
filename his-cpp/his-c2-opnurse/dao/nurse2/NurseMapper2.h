#ifndef _NURSE_MAPPER2_
#define _NURSE_MAPPER2_
#include "Mapper.h"
#include "domain/do/nurse2/nurseDO2.h"

/**
 * wkf_service_request 表：将数据库查询结果集（ResultSet）映射转换为 NurseDO2 实体类
 */
class NurseMapper2 : public Mapper<NurseDO2>
{
public:
	NurseDO2 mapper(ResultSet* resultSet) const override    //wkf_service_requeset 表 字段匹配
	{
		NurseDO2 data;
		data.setId(resultSet->getString("id"));
		data.setName(resultSet->getString("bus_no"));
		data.setState(resultSet->getString("status_enum"));
		data.setContent(resultSet->getString("content_json"));
		data.setAmount(resultSet->getInt("execution_count"));
		data.setPerson(resultSet->getString("performer_id"));
		data.setOperate(resultSet->getString("category_enum"));
		return data;
	}
};

#endif // !_NURSE_MAPPER2_
