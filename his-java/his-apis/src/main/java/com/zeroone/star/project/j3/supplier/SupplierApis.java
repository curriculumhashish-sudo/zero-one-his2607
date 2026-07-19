package com.zeroone.star.project.j3.supplier;

import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierVO;
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
     * 新增供应商
     * @param dto 供应商数据
     * @return
     */
    @Operation(summary = "新增供应商", description = "新增一条供应商记录，包含信用代码、联系方式、资质等信息")
    JsonVO<String> add(
            @Parameter(description = "供应商数据", required = true)
            SupplierDTO dto
    );

    /**
     * 修改供应商
     * @param id 供应商ID
     * @param dto 供应商数据
     * @return
     */
    @Operation(summary = "修改供应商", description = "根据ID修改供应商信息，部分关键字段有限制")
    JsonVO<String> update(
            @Parameter(description = "供应商ID", example = "1", required = true)
            Long id,
            @Parameter(description = "供应商数据", required = true)
            SupplierDTO dto
    );

    /**
     * 删除供应商
     * @param id 供应商ID
     * @return
     */
    @Operation(summary = "删除供应商", description = "根据ID删除供应商记录，需检查是否被采购单引用")
    JsonVO<String> delete(
            @Parameter(description = "供应商ID", example = "1", required = true)
            Long id
    );

    /**
     * 批量启/停用供应商
     * @param batchDTO 批量操作参数
     * @return
     */
    @Operation(summary = "批量启/停用供应商", description = "支持批量操作供应商的启用状态，影响采购业务")
    JsonVO<String> batchUpdateStatus(
            @Parameter(description = "批量操作参数", required = true)
            SupplierBatchDTO batchDTO
    );
}