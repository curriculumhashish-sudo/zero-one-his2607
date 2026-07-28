
#ifndef DIAGNOSE_SERVICE_H
#define DIAGNOSE_SERVICE_H

#include <string>
#include "../../domain/dto/Diagnose/DiagnoseDTO.h"
//<<<<<<< HEAD
#include "../../domain/dto/Diagnose/DiagnoseSelectPageDTO.h"
#include "../../domain/query/Diagnose/DiagnoseSelectQuery.h"
//=======
#include "dto/diagnose/DiagnoseEditDTO.h"
#include "dto/PayloadDTO.hpp"

class DiagnoseService
{
public:
//<<<<<<< HEAD
	DiagnoseSelectPageDTO::Wrapper listSelectableDiagnoses(
		const DiagnoseSelectQuery::Wrapper& query);
//=======
	// 单条新增诊断记录
	std::string saveData(const DiagnoseAddDTO::Wrapper& dto);

	// 根据ID修改诊断记录
	bool updateData(const DiagnoseDTO::Wrapper& dto);

	// 逻辑批量删除（停用）诊断记录
	bool removeData(const oatpp::List<oatpp::String>& ids);
//<<<<<<< HEAD
//	bool removeOne(const std::string& id);
	bool batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, oatpp::String status);
	bool execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto);
//=======
//逻辑单条删除
	bool removeOne(const string& ids);
	//批量停用和启用
	bool batchChangeStatus(const oatpp::List<oatpp::String>& ids, oatpp::Int32 status);

};

#endif