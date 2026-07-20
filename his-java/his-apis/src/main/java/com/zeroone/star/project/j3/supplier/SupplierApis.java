package com.zeroone.star.project.j3.supplier;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;

import java.util.List;

/**
 * 描述：供应商管理 API 接口定义
 *
 * @author zzcsz
 * @version 1.0.0
 */
public interface SupplierApis {

    /**
     * 获取供应商名称列表，用于下拉框。
     *
     * @param statusEnum 状态枚举值，传入则按状态过滤
     * @return 供应商名称列表
     */
    @Operation(summary = "获取供应商名称列表", description = "获取供应商名称，用于下拉框选择")
    JsonVO<List<SupplierNameVO>> querySupplierNames(
            @Parameter(description = "状态枚举值，传入 ENABLED 仅获取启用供应商，不传获取全部", example = "ENABLED")
            String statusEnum
    );

    /**
     * 获取供应商列表，支持条件查询和分页。
     *
     * @param query 查询条件
     * @return 分页数据
     */
    @Operation(summary = "获取供应商列表（条件+分页）", description = "支持按名称、信用代码、状态、类型筛选，返回分页结果")
    JsonVO<PageDTO<SupplierListVO>> querySupplierList(
            @Parameter(description = "查询条件", required = true)
            SupplierQuery query
    );

    /**
     * 获取供应商详情。
     *
     * @param id 供应商 ID
     * @return 供应商完整数据
     */
    @Operation(summary = "获取供应商详情", description = "根据 ID 查询单条供应商的完整资质信息，用于编辑表单回填和详情弹窗")
    JsonVO<SupplierDetailVO> querySupplierDetail(
            @Parameter(description = "供应商 ID", example = "1234567890123456789", required = true)
            String id
    );
}