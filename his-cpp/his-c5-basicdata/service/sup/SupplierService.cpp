#include "stdafx.h"
#include "SupplierService.h"
#include "dao/sup/SupplierDAO.h"
#include "simpleDateTimeFormat.h"

StringJsonVO::Wrapper SupplierService::batchEnableDisable(const SupplierBatchEnableDisableDTO :: Wrapper& dto){
	auto result = StringJsonVO::createShared();

	//ids参数校验
	if (!dto->ids || dto->ids->size() == 0) {
		result->fail(oatpp::String("供应商列表不能为空"));
		return result;
	}

	//StatusEnum参数校验
	std::string status = dto->statusEnum.getValue("");
	if (status != "NORMAL" && status != "DISABLED") {
		result->fail(oatpp::String("状态值无效，仅支持NORMAL(启用)或DISABLED(停用)"));
		return result;
	}

	//oatpp::ids转换为list
	std::list<std::string> idList;
	for (const auto& id : *dto->ids) {
		idList.push_back(id.getValue(""));
	}

	//dao调用
	SupplierDAO dao;
	int affectedRows = dao.batchUpdateStatus(idList, status);

	if (affectedRows > 0)
	{
		result->success(oatpp::String("操作成功，共更新 " + std::to_string(affectedRows) + " 条记录"));
	}
	else
	{
		result->fail(oatpp::String("操作失败，未找到匹配的供应商记录"));
	}

	return result;
}

ListJsonVO<String>::Wrapper SupplierService::batchRemove(const oatpp::List<oatpp::String>& ids){
    auto result = ListJsonVO<String>::createShared();

    // 参数校验
    if (!ids || ids->size() == 0){
        result->fail(oatpp::List<oatpp::String>::createShared());
        return result;
    }

    // oatpp List → std::list 转换
    std::list<std::string> idList;
    for (const auto& id : *ids){
        idList.push_back(id.getValue(""));
    }

    // 获取操作信息
    std::string deletedBy = "system";
    std::string deletedAt = SimpleDateTimeFormat::format();

    // 调用 DAO
    SupplierDAO dao;
    int affectedRows = dao.batchSoftDelete(idList, deletedBy, deletedAt);

    if (affectedRows > 0){
        // 返回成功删除的ID列表
        auto successIds = oatpp::List<oatpp::String>::createShared();
        for (const auto& id : idList){
            successIds->push_back(oatpp::String(id));
        }
        result->success(successIds);
    }else{
        result->fail(oatpp::List<oatpp::String>::createShared());
	}

    return result;
}

