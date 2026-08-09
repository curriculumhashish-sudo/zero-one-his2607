#ifndef PAGEDIAGNOSISDICTIONARYMAPPER_H_
#define PAGEDIAGNOSISDICTIONARYMAPPER_H_

#include "Mapper.h"
#include "domain/do/pageDiagnosisDictionary/ClnConditionDefinitionDO.h"

class PageDiagnosisDictionaryMapper : public Mapper<PtrClnConditionDefinitionDO>
{
public:
	PtrClnConditionDefinitionDO mapper(ResultSet* resultSet) const override
	{
		PtrClnConditionDefinitionDO conditionDO = std::make_shared<ClnConditionDefinitionDO>();

		conditionDO->setId(resultSet->getString("id"));
		conditionDO->setTenantId(resultSet->getString("tenant_id"));
		conditionDO->setConditionCode(resultSet->getString("condition_code"));//’Ô∂œ±‡¬Î
		conditionDO->setName(resultSet->getString("name"));//’Ô∂œ√˚≥∆
		conditionDO->setTypeCode(resultSet->getString("type_code"));//’Ô∂œ¿‡–Õ

		return conditionDO;
	}
};





#endif // !
