package com.zeroone.star.project.emr.configmanage.dto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：执行地点新增/修改传输数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Schema(description = "执行地点传输数据对象")
@Data
public class ExecPlaceDTO {

    /**
     * 主键（修改时必传）
     */
    @Schema(description = "主键（修改时必传）", example = "1")
    private Long id;

    /**
     * 执行地点编码
     */
    @Schema(description = "执行地点编码", example = "EP001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String placeCode;

    /**
     * 执行地点名称
     */
    @Schema(description = "执行地点名称", example = "门诊注射室", requiredMode = Schema.RequiredMode.REQUIRED)
    private String placeName;

    /**
     * 排序号
     */
    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    /**
     * 是否启用 0-禁用 1-启用
     */
    @Schema(description = "是否启用 0-禁用 1-启用", example = "1")
    private Integer activeFlag;
}
