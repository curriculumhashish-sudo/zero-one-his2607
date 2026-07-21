package com.zeroone.star.project.j3.supplier;

import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import java.util.List;

/**
 * <p>
 * 描述：供应商管理接口（只有新增供应商，修改供应商，删除供应商，批量启/停用供应商）
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Tag(name = "供应商管理", description = "供应商相关操作接口")
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

    /**
     * 新增供应商
     * @param dto 供应商数据
     * @return
     */
    @Operation(summary = "新增供应商", description = "新增一条供应商记录，包含信用代码、联系方式、资质等信息")
    JsonVO<String> addSupplier(
            @Parameter(description = "供应商数据", required = true)
            SupplierFullDTO dto
    );

    /**
     * 修改供应商
     * @param id 供应商ID
     * @param dto 供应商数据
     * @return
     */
    @Operation(summary = "修改供应商", description = "根据ID修改供应商信息，部分关键字段有限制")
    JsonVO<String> updateSupplier(
            @Parameter(description = "供应商ID", example = "1234567890123456789", required = true)
            String id,
            @Parameter(description = "供应商数据", required = true)
            SupplierFullDTO dto
    );

    /**
     * 删除供应商
     * @param id 供应商ID
     * @return
     */
    @Operation(summary = "删除供应商", description = "根据ID删除供应商记录，需检查是否被采购单引用")
    JsonVO<String> deleteSupplier(
            @Parameter(description = "供应商ID", example = "1234567890123456789", required = true)
            String id
    );

    /**
     * 批量启/停用供应商
     * @param batchDTO 批量操作参数
     * @return
     */
    @Operation(summary = "批量启/停用供应商", description = "支持批量操作供应商的启用状态，影响采购业务")
    JsonVO<String> batchUpdateSuppliers(
            @Parameter(description = "批量操作参数", required = true)
            SupplierBatchDTO batchDTO
    );
}