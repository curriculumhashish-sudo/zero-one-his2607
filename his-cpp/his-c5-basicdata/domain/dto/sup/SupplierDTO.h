#pragma once
/*@Auther:shelly
* @Date:2026/7/20
*/
#ifndef _SUPPLIER_DTO_
#define _SUPPLIER_DTO_

#include "../../GlobalInclude.h"
#include "domain/dto/PageDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/**
 * SupplierDTO —— 供应商完整信息的数据传输对象
 *
 * 【作用】
 * 当前端查询单条供应商详情时，后端用这个对象返回完整数据。
 * 新增/修改供应商时，前端也用这个对象传表单数据。
 *
 * 【字段设计原则】
 * 所有字段名与数据库 cus_supplier 表的列名保持一致。
 * 这样后续用 ZO_STAR_DOMAIN_DO_TO_DTO 宏做 DO→DTO 转换时，
 * 字段名能自动匹配，不用手动一个个赋值。
 */
class SupplierDTO : public oatpp::DTO {
	DTO_INIT(SupplierDTO, DTO);

	//id 供应商唯一标识
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("sup.field.id");
	}

    // 业务编号 —— 供应商的业务编码/流水号
    DTO_FIELD(String, busNo);
    DTO_FIELD_INFO(busNo) {
        info->description = ZH_WORDS_GETTER("sup.field.busNo");
    }

    // 供应商名称
    DTO_FIELD(String, name);
    DTO_FIELD_INFO(name) {
        info->description = ZH_WORDS_GETTER("sup.field.name");
    }

    // 类型枚举值 —— 如 "DRUG"（药品供应商）、"MATERIAL"（物资供应商）
    DTO_FIELD(String, typeEnum);
    DTO_FIELD_INFO(typeEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.typeEnum");
    }

    // 统一社会信用代码
    DTO_FIELD(String, creditCode);
    DTO_FIELD_INFO(creditCode) {
        info->description = ZH_WORDS_GETTER("sup.field.creditCode");
    }

    // 法定代表人姓名
    DTO_FIELD(String, legalRepresentative);
    DTO_FIELD_INFO(legalRepresentative) {
        info->description = ZH_WORDS_GETTER("sup.field.legalRepresentative");
    }

    // 地址
    DTO_FIELD(String, address);
    DTO_FIELD_INFO(address) {
        info->description = ZH_WORDS_GETTER("sup.field.address");
    }

    // 拼音码 —— 用于快速检索，如"华瑞"的拼音码为"hr"
    DTO_FIELD(String, pyStr);
    DTO_FIELD_INFO(pyStr) {
        info->description = ZH_WORDS_GETTER("sup.field.pyStr");
    }

    // 五笔码 —— 另一种快速检索编码方式
    DTO_FIELD(String, wbStr);
    DTO_FIELD_INFO(wbStr) {
        info->description = ZH_WORDS_GETTER("sup.field.wbStr");
    }

    // 联系电话
    DTO_FIELD(String, phone);
    DTO_FIELD_INFO(phone) {
        info->description = ZH_WORDS_GETTER("sup.field.phone");
    }

    // 邮箱
    DTO_FIELD(String, email);
    DTO_FIELD_INFO(email) {
        info->description = ZH_WORDS_GETTER("sup.field.email");
    }

    // 开户行名称
    DTO_FIELD(String, bankName);
    DTO_FIELD_INFO(bankName) {
        info->description = ZH_WORDS_GETTER("sup.field.bankName");
    }

    // 银行账号
    DTO_FIELD(String, bankAccount);
    DTO_FIELD_INFO(bankAccount) {
        info->description = ZH_WORDS_GETTER("sup.field.bankAccount");
    }

    // 所属机构ID —— 供应商隶属于哪个机构
    DTO_FIELD(String, orgId);
    DTO_FIELD_INFO(orgId) {
        info->description = ZH_WORDS_GETTER("sup.field.orgId");
    }

    // 显示顺序 —— 列表排序用的序号
    DTO_FIELD(Int32, displayOrder);
    DTO_FIELD_INFO(displayOrder) {
        info->description = ZH_WORDS_GETTER("sup.field.displayOrder");
    }

    // 状态枚举值 —— "NORMAL"=启用, "DISABLED"=停用
    // 这就是启/停用功能要修改的字段
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
    }

    // 备注说明
    DTO_FIELD(String, note);
    DTO_FIELD_INFO(note) {
        info->description = ZH_WORDS_GETTER("sup.field.note");
    }
};

/**
 * SupplierPageDTO —— 供应商分页列表的包装
 *
 * 【作用】
 * 把多条 SupplierDTO 装进 rows 列表里，
 * 加上 total（总条数）、pages（总页数）、pageIndex（当前页）、pageSize（每页条数）等信息。
 * 前端拿到后直接渲染分页表格。
 */
class SupplierPageDTO : public PageDTO<SupplierDTO::Wrapper>
{
    DTO_INIT(SupplierPageDTO, PageDTO<SupplierDTO::Wrapper>);
};

/**
 * SupplierBatchEnableDisableDTO —— 批量启/停用供应商的请求体
 *
 * 【作用】
 * 这是"批量启停"功能的核心 DTO。
 * 前端传 JSON: {"ids":["id1","id2"], "statusEnum":"DISABLED"}
 * 后端收到后，把 IDs 对应的所有供应商的 status_enum 统一更新为目标值。
 *
 * 【注意】
 * 批量删除不需要这个 DTO，直接用 List<String> 作为请求体即可。
 */
class SupplierBatchEnableDisableDTO : public oatpp::DTO {
    DTO_INIT(SupplierBatchEnableDisableDTO, DTO);

    //要操作的供应商列表
    DTO_FIELD(List<String>, ids);
    DTO_FIELD_INFO(ids) {
        info->description = ZH_WORDS_GETTER("sup.field.ids");
    }

    // 目标状态值 —— 传 "NORMAL" 就是启用，传 "DISABLED" 就是停用
    // 这对应批量启停接口的参数
    DTO_FIELD(String, statusEnum);
    DTO_FIELD_INFO(statusEnum) {
        info->description = ZH_WORDS_GETTER("sup.field.statusEnum");
    }
};

#include OATPP_CODEGEN_END(DTO)
#endif