package com.zeroone.star.project.dto.j2.basedata.bodypartdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

/**
 * <p>
 * 描述：启停用部位传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * <p>
 * 状态值与 dia_body_structure.status_enum 保持一致：ACTIVE 表示启用，INACTIVE 表示停用。
 * </p>
 *
 * @author 吴志洲
 * @version 1.0.0
 */
@Data
@Schema(description = "启停用部位传输对象")
public class BodyPartStatusDTO {

    @NotBlank(message = "部位ID不能为空")
    @Schema(description = "部位ID", example = "322663105141149696", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "目标状态不能为空")
    @Pattern(regexp = "ACTIVE|INACTIVE", message = "目标状态只能为ACTIVE或INACTIVE")
    @Schema(description = "目标状态", example = "ACTIVE", allowableValues = {"ACTIVE", "INACTIVE"}, requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @NotNull(message = "版本号不能为空")
    @Schema(description = "乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;
}
