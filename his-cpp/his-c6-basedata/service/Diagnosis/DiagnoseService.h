
#ifndef DIAGNOSE_SERVICE_H
#define DIAGNOSE_SERVICE_H

#include <string>
#include "../../domain/dto/Diagnosis/DiagnoseDTO.h"
//#include "dto/diagnose/DiagnoseEditDTO.h"
//#include "dto/PayloadDTO.hpp"

class DiagnoseService
{
public:
	// 单条新增诊断记录
	std::string saveData(const DiagnoseAddDTO::Wrapper& dto);

	// 根据ID修改诊断记录
	bool updateData(const DiagnoseDTO::Wrapper& dto);

	// 逻辑批量删除（停用）诊断记录
	bool removeData(const oatpp::List<oatpp::String>& ids);
//逻辑单条删除
	bool removeOne(const string& ids);
	//批量停用和启用
	bool batchChangeStatus(const oatpp::List<oatpp::String>& ids, oatpp::Int32 status);

};

#endif