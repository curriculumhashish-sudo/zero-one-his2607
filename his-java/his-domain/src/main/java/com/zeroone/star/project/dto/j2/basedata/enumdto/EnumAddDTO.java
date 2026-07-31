package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Data;
import lombok.ToString;

/**
 * <p>
 * 描述: 枚举添加DTO
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@ToString
@Schema(description = "枚举添加DTO")
public class EnumAddDTO {

    @NotBlank
    @Schema(description = "枚举类型ID，和EnumType的id关联，不由用户输入", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumTypeId;

    @Size(min = 1, max = 100, message = "枚举键长度必须在1到100个字符之间")
    @Pattern(regexp = "^[a-zA-Z0-9_]+$", message = "枚举类型键必须由字母、数字或下划线组成")
    @Schema(description = "枚举键，必须唯一", example = "enumKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;

    @NotNull
    @Schema(description = "枚举码，必须唯一", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer enumCode;

    @Size(min = 1, max = 100, message = "标签长度必须在1到500个字符之间")
    @Schema(description = "标签，必须唯一", example = "男性", requiredMode = Schema.RequiredMode.REQUIRED)
    private String display;

    @Size(min = 1, max = 500, message = "描述长度必须在1到500个字符之间")
    @Schema(description = "描述", example = "描述", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;

    @Min(value = 0, message = "排序不能小于0")
    @NotNull
    @Schema(description = "排序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer order;

    @NotNull(message = "状态不能为空")
    @Schema(description = "枚举状态(0-激活, 1-未激活)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
