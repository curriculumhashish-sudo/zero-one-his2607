package com.zeroone.star.project.vo.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：样本定义下拉选项显示对象
 * 供其他组模块获取当前租户下已有样本名称，前端渲染为下拉选项，选择后存储样本ID
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "样本定义下拉选项显示对象")
@Data
public class SpecimenOptionVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "下拉框选定值（样本定义主键，其他组选择后存储该值）",
            example = "328090253175296000", requiredMode = Schema.RequiredMode.REQUIRED)
    private String value;

    @Schema(description = "下拉框文本（样本名称）",
            example = "血", requiredMode = Schema.RequiredMode.REQUIRED)
    private String label;
}
