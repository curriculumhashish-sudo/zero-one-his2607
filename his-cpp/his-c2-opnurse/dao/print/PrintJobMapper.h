#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: sqdxy
 @Date: 2026/07/20
*/
#ifndef _PRINT_JOB_MAPPER_
#define _PRINT_JOB_MAPPER_

#include "Mapper.h"
#include "../../domain/do/print/PrintJobDO.h"

class PrintJobMapper : public Mapper<PrintJobDO>
{
public:
	PrintJobDO mapper(ResultSet* resultSet) const override
	{
		PrintJobDO data;
		data.setId(resultSet->getString(1));
		data.setDocType(resultSet->getString(2));
		data.setPatientName(resultSet->getString(3));
		data.setTreatmentItem(resultSet->getString(4));
		data.setExecDept(resultSet->getString(5));
		data.setDoctorName(resultSet->getString(6));
		data.setOrderTime(resultSet->getString(7));
		data.setExecTime(resultSet->getString(8));
		data.setExecutor(resultSet->getString(9));
		data.setStatus(resultSet->getInt(10));
		data.setPrintCount(resultSet->getInt(11));
		data.setRemark(resultSet->getString(12));
		data.setCreateBy(resultSet->getString(13));
		data.setCreateTime(resultSet->getString(14));
		data.setUpdateBy(resultSet->getString(15));
		data.setUpdateTime(resultSet->getString(16));
		return data;
	}
};

class PtrPrintJobMapper : public Mapper<PtrPrintJobDO>
{
public:
	PtrPrintJobDO mapper(ResultSet* resultSet) const override
	{
		auto data = std::make_shared<PrintJobDO>();
		data->setId(resultSet->getString(1));
		data->setDocType(resultSet->getString(2));
		data->setPatientName(resultSet->getString(3));
		data->setTreatmentItem(resultSet->getString(4));
		data->setExecDept(resultSet->getString(5));
		data->setDoctorName(resultSet->getString(6));
		data->setOrderTime(resultSet->getString(7));
		data->setExecTime(resultSet->getString(8));
		data->setExecutor(resultSet->getString(9));
		data->setStatus(resultSet->getInt(10));
		data->setPrintCount(resultSet->getInt(11));
		data->setRemark(resultSet->getString(12));
		data->setCreateBy(resultSet->getString(13));
		data->setCreateTime(resultSet->getString(14));
		data->setUpdateBy(resultSet->getString(15));
		data->setUpdateTime(resultSet->getString(16));
		return data;
	}
};

#endif // !_PRINT_JOB_MAPPER_
