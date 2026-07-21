package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.ToString;

import java.util.Date;

/**
 * <p>
 * 描述：枚举类型DTO
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@ToString
@Schema(description = "枚举类型DTO，包含ID、租户ID、创建时间、更新时间、创建人、更新人，用于表示枚举类型的基本信息")
public class EnumTypeDTO extends EnumTypeModifyDTO {

    @Schema(description = "租户ID", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;

    @Schema(description = "创建时间", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Date createdAt;

    @Schema(description = "更新时间", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Date updatedAt;

    @Schema(description = "创建人", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String createdBy;

    @Schema(description = "更新人", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String updatedBy;
}
