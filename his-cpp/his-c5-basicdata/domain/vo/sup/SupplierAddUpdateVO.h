#ifndef _SUPPLIER_ADD_UPDATE_VO_H_
#define _SUPPLIER_ADD_UPDATE_VO_H_

#include "../../GlobalInclude.h"
#include "domain/vo/BaseJsonVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 供应商创建返回
 *
 * 数据对象为供应商ID
 */
class SupplierAddJsonVO : public StringJsonVO
{
    DTO_INIT(SupplierAddJsonVO, StringJsonVO);
};

/**
 * 供应商更新返回
 */
class SupplierUpdateJsonVO : public NoDataJsonVO
{
    DTO_INIT(SupplierUpdateJsonVO, NoDataJsonVO);
};

#include OATPP_CODEGEN_END(DTO)

#endif
