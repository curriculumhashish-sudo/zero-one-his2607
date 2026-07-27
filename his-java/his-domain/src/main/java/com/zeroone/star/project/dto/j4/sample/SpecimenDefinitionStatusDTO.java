package com.zeroone.star.project.dto.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

/**
 * <p>
 * 描述：样本定义 启用/停用 请求参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "样本定义启用/停用请求参数")
public class SpecimenDefinitionStatusDTO {

    /**
     * 样本定义主键
     */
    @NotBlank(message = "样本定义主键不能为空")
    @Pattern(regexp = "^\\d{1,20}$", message = "样本定义主键格式不正确")
    @Schema(description = "样本定义主键（雪花ID，取查询结果中的 id）",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "1945012345678901234")
    private String id;

    /**
     * 乐观锁版本号（用于并发控制，取查询结果中的 version）
     */
    @NotNull(message = "乐观锁版本号不能为空")
    @Min(value = 0, message = "乐观锁版本号不能小于0")
    @Schema(description = "乐观锁版本号，用于并发控制，取查询结果中的 version",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "0", minimum = "0")
    private Integer version;

    /**
     * 目标发布状态编码（对应 PublicationStatus 枚举）
     */
    @NotBlank(message = "目标状态不能为空")
    @Pattern(regexp = "^(ACTIVE|INACTIVE)$", message = "目标状态只能为 ACTIVE 或 INACTIVE")
    @Schema(description = "目标发布状态编码，对应 PublicationStatus 枚举（ACTIVE-启用、INACTIVE-停用）",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "INACTIVE",
            allowableValues = {"ACTIVE", "INACTIVE"})
    private String statusEnum;
}
