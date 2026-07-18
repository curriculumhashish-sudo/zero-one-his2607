package com.zeroone.star.project.j3.fee;

import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

/**
 * <p>
 * 描述：费用性质管理接口
 * @author 白文儿
 * @version 1.0.0
 */
@Tag(name = "费用性质管理", description = "费用性质相关操作接口")
public interface FeeNatureApis {

    /**
     * 获取费用性质列表（条件+分页）
     * @param query 查询条件
     * @return 分页数据
     */
    @Operation(summary = "获取费用性质列表（条件+分页）", description = "支持按名称或编码搜索，返回分页结果")
    JsonVO<PageDTO<FeeNatureVO>> getPageList(
            @Parameter(description = "查询条件", required = true)
            FeeNatureQuery query
    );

    /**
     * 修改费用性质
     * @param id 费用性质ID
     * @param dto 费用性质数据
     * @return 操作结果
     */
    @Operation(summary = "修改费用性质", description = "根据ID修改费用性质信息")
    JsonVO<String> update(
            @Parameter(description = "费用性质ID", example = "1", required = true)
            Long id,
            @Parameter(description = "费用性质数据", required = true)
            FeeNatureDTO dto
    );

    /**
     * 停/启用费用性质
     * @param id 费用性质ID
     * @param isEnabled 是否启用 1 启用 0 禁用
     * @return 操作结果
     */
    @Operation(summary = "停/启用费用性质", description = "切换费用性质的启用状态")
    JsonVO<String> updateStatus(
            @Parameter(description = "费用性质ID", example = "1", required = true)
            Long id,
            @Parameter(description = "是否启用 1 启用 0 禁用", example = "1", required = true)
            Integer isEnabled
    );
}
