package com.zeroone.star.project.dto.j2.sys.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * <p>
 * 描述：枚举类型添加DTO
 * TODO: 由于还没有设计实际的mongodb集合结构，这里的类型名字暂定，EnumTypeDTO也是如此
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@Schema(description = "枚举类型添加DTO")
public class EnumTypeAddDTO {

    @Size(min = 1, max = 100, message = "枚举类型键长度必须在1到100个字符之间")
    @Pattern(regexp = "^[a-zA-Z0-9_]+$", message = "枚举类型键必须由字母、数字或下划线组成")
    @Schema(description = "枚举类型键，必须唯一", example = "enumTypeKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumTypeKey;

    @Size(min = 1, max = 500, message = "枚举类型名称长度必须在1到500个字符之间")
    @Schema(description = "枚举类型名称", example = "enumTypeName", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Schema(description = "枚举类型描述", example = "enumTypeDescription", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;

    @NotNull(message = "必须指定其是否为系统内置枚举类型，默认是false")
    @Schema(description = "是否系统枚举类型", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isSystem;

    // TODO: 该类型计划也由枚举系统管理，本身为系统内置的枚举类型
    @NotNull(message = "枚举类型状态枚举值不能为空")
    @Schema(description = "枚举类型状态(0-激活, 1-未激活)", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
