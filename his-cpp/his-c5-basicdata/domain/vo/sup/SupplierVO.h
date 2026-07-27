#pragma once
/*
* @Auther:shelly
* @Date:2026/7/20
*/

#ifndef _SUPPLIER_VO_
#define _SUPPLIER_VO_

#include "../../GlobalInclude.h"
#include "../../dto/sup/SupplierDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * SupplierJsonVO —— 单条供应商数据的响应包装
 *
 * 【作用】
 * 查询单条供应商时（GET /sup/query-one），
 * 用这个类把 SupplierDTO 包进标准 JSON 响应格式。
 * 
 * 【返回示例】
 * {
 *   "code": 200,
 *   "message": "success",
 *   "data": {
 *     "id": "xxx",
 *     "name": "华瑞医药",
 *     "statusEnum": "NORMAL"
 *     // ... 其他字段
 *   }
 * }
 */
class SupplierJsonVO : public JsonVO<SupplierDTO::Wrapper> {
    DTO_INIT(SupplierJsonVO, JsonVO<SupplierDTO::Wrapper>);
};

/**
 * SupplierPageJsonVO —— 供应商分页列表的响应包装
 *
 * 【作用】
 * 分页查询供应商列表时（GET /sup/query-all），
 * 用这个类把 SupplierPageDTO 包进标准 JSON 响应格式。
 * 前端拿到后直接渲染表格和分页器。
 * 
 * 【返回示例】
 * {
 *   "code": 200,
 *   "message": "success",
 *   "data": {
 *     "pageIndex": 1,
 *     "pageSize": 10,
 *     "total": 100,
 *     "pages": 10,
 *     "rows": [
 *       { "id": "xxx", "name": "华瑞医药", ... },
 *       { "id": "yyy", "name": "康泰医疗", ... }
 *     ]
 *   }
 * }
 */
class SupplierPageJsonVO : public JsonVO<SupplierPageDTO::Wrapper> {
    DTO_INIT(SupplierPageJsonVO, JsonVO<SupplierPageDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif