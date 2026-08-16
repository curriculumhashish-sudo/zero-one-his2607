#pragma once
#ifndef _SETTINGSMAPPER_H_
#define _SETTINGSMAPPER_H_

#include "Mapper.h"
#include "../../domain/do/settings/SettingsDO.h"

class SettingsMapper : public Mapper<SettingsDO> {
public:
	SettingsDO mapper(ResultSet* resultSet) const override {
		SettingsDO data;
		data.setId(resultSet->getString(1));
		data.setName(resultSet->getString(2));
		data.setValue(resultSet->getString(3));
		data.setDescription(resultSet->getString(4));
		data.setProviderName(resultSet->getString(5));
		data.setProviderKey(resultSet->getString(6));
		data.setTenantId(resultSet->getString(7));
		return data;
	}
};

class PtrSettingsMapper : public Mapper<PtrSettingsDO> {
public:
	PtrSettingsDO mapper(ResultSet* resultSet) const override {
		auto data = std::make_shared<SettingsDO>();
		data->setName(resultSet->getString(1));
		data->setValue(resultSet->getString(2));
		data->setDescription(resultSet->getString(3));
		data->setProviderName(resultSet->getString(4));
		data->setProviderKey(resultSet->getString(5));
		return data;
	}
};

#endif