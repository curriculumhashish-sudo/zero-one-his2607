#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: shelly
 @Date: 2026/7/20

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#ifndef _SUPLIER_CONTROLLER_
#define _SUPLIER_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/sup/SupplierQuery.h"
#include "domain/dto/sup/SupplierDTO.h"
#include "domain/vo/sup/SupplierVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG ZH_WORDS_GETTER("sup.tag")
/**
 * SupplierController —— 供应商管理控制器
 *
 * 这是前后端交互的"接口合同"。
 * 定义 6 个 API 端点，涵盖 CRUD + 批量启停 + 批量删除。
 * 每个端点用宏声明，同时生成 Swagger 文档。
 */
class SupplierController : public oatpp::web::server::api::ApiController {
    API_ACCESS_DECLARE(SupplierController);

public:
    // ============================================================
    // 1. GET /sup/query-one —— 根据 ID 查询单个供应商
    // ============================================================
    /* Swagger 文档定义：
       - 接口标题从词典读取
       - 响应类型 SupplierJsonVO
       - 标签 API_TAG
       - 添加一个必填的查询参数 id
    */
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.query-one.summary"),   // 接口描述
        queryOne,                                    // 方法名
        SupplierJsonVO::Wrapper,                     // 响应类型
        API_TAG,                                     // Swagger 标签
        API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("sup.field.id"), "", true);
    );
    /* 实际接口声明：
       - HTTP GET
       - 路径 /sup/query-one
       - 从 URL 取 id 参数，传给 execQueryOne
    */
    API_HANDLER_ENDPOINT_AUTH(
        API_M_GET,
        "/sup/query-one",
        queryOne,
        QUERY(String, id),
        execQueryOne(id)
    );

    // ============================================================
    // 2. GET /sup/query-all —— 分页查询供应商列表（带筛选）
    // ============================================================
    /* 分页查询专用的文档宏，自动把 SupplierQuery 的字段生成文档 */
    API_DEF_ENDPOINT_INFO_QUERY_AUTH(
        ZH_WORDS_GETTER("sup.query-all.summary"),
        queryAll,
        SupplierQuery,                           // 查询参数类型
        SupplierPageJsonVO::Wrapper,             // 分页响应类型
        API_TAG
    );
    API_HANDLER_ENDPOINT_QUERY_AUTH(
        API_M_GET,
        "/sup/query-all",
        queryAll,
        SupplierQuery,                           // 自动解析查询参数到 SupplierQuery 对象
        execQueryAll(query)
    );

    // ============================================================
    // 3. POST /sup/add —— 新增供应商
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.add.summary"),
        addSupplier,
        StringJsonVO::Wrapper,    // 返回新增的 ID
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_POST,
        "/sup/add",
        addSupplier,
        BODY_DTO(SupplierDTO::Wrapper, dto),  // 从请求体 JSON 反序列化成 SupplierDTO
        execAddSupplier(dto)
    );

    // ============================================================
    // 4. PUT /sup/modify —— 修改供应商
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.modify.summary"),
        modifySupplier,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/sup/modify",
        modifySupplier,
        BODY_DTO(SupplierDTO::Wrapper, dto),  // 请求体传完整的 SupplierDTO（含 id）
        execModifySupplier(dto)
    );

    // ============================================================
    // 5. PUT /sup/batch-enable-disable —— 批量启/停用供应商
    //    请求体: {"ids": ["id1","id2",...], "statusEnum": "NORMAL"}
    //    statusEnum = "NORMAL" 表示启用，"DISABLED" 表示停用
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-enable-disable.summary"),
        batchEnableDisable,
        StringJsonVO::Wrapper,
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_PUT,
        "/sup/batch-enable-disable",
        batchEnableDisable,
        BODY_DTO(SupplierBatchEnableDisableDTO::Wrapper, dto),  // 自定义 DTO：ids + statusEnum
        execBatchEnableDisable(dto)
    );

    // ============================================================
    // 6. DELETE /sup/batch-remove —— 批量删除供应商
    //    请求体: ["id1", "id2", "id3"]
    //    直接传字符串数组，不需要额外 DTO
    // ============================================================
    API_DEF_ENDPOINT_INFO_AUTH(
        ZH_WORDS_GETTER("sup.batch-remove.summary"),
        batchRemove,
        ListJsonVO<String>::Wrapper,   // 返回被删除的 ID 列表
        API_TAG
    );
    API_HANDLER_ENDPOINT_AUTH(
        API_M_DEL,
        "/sup/batch-remove",
        batchRemove,
        BODY_DTO(List<String>, ids),   // 请求体直接传 List<String>
        execBatchRemove(ids)
    );

private:
        // ----------------------------------------------------------
        // 私有 execute 方法 —— 真正的业务逻辑在这里
        // 目前返回 {} 空对象占位，后续集成 Service 后替换
        // ----------------------------------------------------------
        SupplierJsonVO::Wrapper execQueryOne(const String& id);
        SupplierPageJsonVO::Wrapper execQueryAll(const SupplierQuery::Wrapper& query);
        StringJsonVO::Wrapper execAddSupplier(const SupplierDTO::Wrapper& dto);
        StringJsonVO::Wrapper execModifySupplier(const SupplierDTO::Wrapper& dto);
        StringJsonVO::Wrapper execBatchEnableDisable(const SupplierBatchEnableDisableDTO::Wrapper& dto);
        ListJsonVO<String>::Wrapper execBatchRemove(const List<String>& ids);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)
#endif // !_SUPPLIER_CONTROLLER_