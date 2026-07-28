
#ifndef DIAGNOSE_SERVICE_H
#define DIAGNOSE_SERVICE_H

#include <string>
#include "../../domain/dto/Diagnose/DiagnoseDTO.h"
#include "../../domain/dto/Diagnose/DiagnoseSelectPageDTO.h"
#include "../../domain/query/Diagnose/DiagnoseSelectQuery.h"
//#include "dto/Diagnose/DiagnoseEditDTO.h"
#include "domain/dto/PayloadDTO.h"

class DiagnoseService
{
public:
	// 查询当前用户可选择的诊疗分页列表。
	DiagnoseSelectPageDTO::Wrapper listSelectableDiagnoses(
		const DiagnoseSelectQuery::Wrapper& query,
		const PayloadDTO& payload);
	//新增
	std::string saveData(const DiagnoseAddDTO::Wrapper& dto);
	//更新
	bool updateData(const DiagnoseDTO::Wrapper& dto);
	//逻辑批量删不带鉴权
	bool removeData(const oatpp::List<oatpp::String>& ids);
	//物理删除单条（前端有，不知道要不要，先写上）
	bool removeOne(const std::string& id);
	//停启用
	bool batchChangeStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, oatpp::String status);
	//带鉴权逻辑批量删除
	bool execRemoveData(const DiagnoseRemoveDTO::Wrapper& dto);
	//物理批量删除
	bool DiagnoseService::RemoveData(const oatpp::List<oatpp::String>& ids);

};

#endif
