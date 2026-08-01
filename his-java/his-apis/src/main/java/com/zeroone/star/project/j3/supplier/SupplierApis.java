package com.zeroone.star.project.j3.supplier;

import com.zeroone.star.project.dto.j3.supplier.SupplierAddDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierUpdateDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

import java.util.List;

@Tag(name = "供应商管理", description = "供应商基础信息管理接口")
public interface SupplierApis {

    @Operation(summary = "获取供应商名称", description = "用于下拉框或业务选择框，默认只返回启用且未删除的供应商，支持按名称、拼音码、五笔码搜索")
    JsonVO<List<SupplierNameVO>> querySupplierNames(
            @Parameter(description = "状态枚举：ENABLED-启用，DISABLED-停用；不传默认查询启用供应商", example = "ENABLED")
            String statusEnum,
            @Parameter(description = "搜索关键字，支持供应商名称、拼音码、五笔码模糊匹配", example = "HRYY")
            String keyword
    );

    @Operation(summary = "获取供应商列表", description = "分页查询供应商管理列表，支持按关键字、名称、业务编号、信用代码、类型、状态、机构、拼音码、五笔码、电话筛选")
    JsonVO<PageDTO<SupplierListVO>> querySupplierList(
            @Parameter(description = "供应商查询条件，包含分页参数和筛选字段", required = true)
            SupplierQuery query
    );

    @Operation(summary = "获取供应商详情", description = "根据供应商ID查询单条完整信息，用于详情查看和编辑表单回显，已删除供应商不会返回")
    JsonVO<SupplierDetailVO> querySupplierDetail(
            @Parameter(description = "供应商主键ID", example = "1234567890123456789", required = true)
            String id
    );

    @Operation(summary = "新增供应商", description = "新增一条供应商基础档案")
    JsonVO<String> addSupplier(
            @Parameter(description = "供应商完整数据", required = true)
            SupplierAddDTO dto
    );

    @Operation(summary = "修改供应商", description = "根据供应商ID修改供应商基础档案")
    JsonVO<String> updateSupplier(
            @Parameter(description = "供应商主键ID", example = "1234567890123456789", required = true)
            String id,
            @Parameter(description = "供应商完整数据", required = true)
            SupplierUpdateDTO dto
    );

    @Operation(summary = "删除供应商", description = "根据供应商ID删除供应商基础档案")
    JsonVO<String> deleteSupplier(
            @Parameter(description = "供应商主键ID", example = "1234567890123456789", required = true)
            String id
    );

    @Operation(summary = "批量启停供应商", description = "批量启用或停用供应商，影响后续业务选择")
    JsonVO<String> batchUpdateSuppliers(
            @Parameter(description = "批量状态操作数据", required = true)
            SupplierBatchDTO batchDTO
    );
}
