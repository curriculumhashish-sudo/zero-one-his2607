#ifndef _SUPPLIER_SERVICE_H_
#define _SUPPLIER_SERVICE_H_

#include "domain/dto/sup/SupplierDTO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/GlobalInclude.h"

class SupplierService
{
public:
    // 批量启用/停用供应商
    StringJsonVO::Wrapper batchEnableDisable(
        const SupplierBatchEnableDisableDTO::Wrapper& dto);

    // 批量删除供应商（软删除），返回被删除的ID列表
    ListJsonVO<String>::Wrapper batchRemove(const oatpp::List<oatpp::String>& ids);
};

#endif // !_SUPPLIER_SERVICE_H_