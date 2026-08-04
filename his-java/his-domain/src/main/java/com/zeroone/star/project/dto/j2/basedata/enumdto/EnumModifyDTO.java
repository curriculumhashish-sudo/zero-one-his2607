package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Data;
import lombok.ToString;

/**
 * <p>
 * 描述: 枚举修改DTO，枚举值的key和code不允许二次修改（这会改变其它数据的映射结果）
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@Schema(description = "枚举修改DTO")
public class EnumModifyDTO {

    @NotBlank
    @Schema(description = "枚举类型ID，和EnumType的id关联", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumTypeId;

    @Schema(description = "枚举键，仅用于提供渲染", example = "enumKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;

    @Schema(description = "枚举码，仅用于提供渲染", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer enumCode;

    @Size(min = 1, max = 100, message = "标签长度必须在1到100个字符之间")
    @Schema(description = "标签，必须唯一", example = "label", requiredMode = Schema.RequiredMode.REQUIRED)
    private String display;

    @Size(min = 1, max = 500, message = "描述长度必须在1到500个字符之间")
    @Schema(description = "描述", example = "描述", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;

    @Min(value = 0, message = "排序必须大于0")
    @NotNull
    @Schema(description = "排序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer order;

    @NotNull(message = "状态不能为空")
    @Schema(description = "枚举状态(0-激活, 1-未激活)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;

    @NotBlank
    @Schema(description = "枚举ID", example = "exdat1234123047", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
