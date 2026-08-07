package com.zeroone.star.project.dto.j5.bindingMapping;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.time.LocalTime;

/**
 * <p>
 * 描述：用法绑定数据传输对象（映射cus_service_location_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@Schema(description = "用法绑定数据传输对象")
public class UsageBindingDTO {

    @Schema(description = "主键ID（编辑时必传）")
    private String id;

    @NotBlank(message = "机构ID不能为空")
    @Schema(description = "机构ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @NotBlank(message = "项目表不能为空")
    @Schema(description = "项目表（如：healthcare_service）", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemTable;

    @NotBlank(message = "分配分类编码不能为空")
    @Schema(description = "分配分类编码", requiredMode = Schema.RequiredMode.REQUIRED)
    private String distributionCategoryCode;

    @NotBlank(message = "默认执行地点ID不能为空")
    @Schema(description = "默认执行地点ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String defLocationId;

    @Schema(description = "项目定义ID")
    private String itemDefinitionId;

    @Schema(description = "方法编码")
    private String methodCode;

    @Schema(description = "开始时间")
    private LocalTime startTime;

    @Schema(description = "结束时间")
    private LocalTime endTime;

    @Schema(description = "优先级")
    private Integer priority;

    @Schema(description = "显示顺序")
    private Integer displayOrder;

    @Schema(description = "备注")
    private String remark;
}
