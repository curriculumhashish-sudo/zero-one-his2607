package com.zeroone.star.project.dto.j3.patient;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：修改患者数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 照夜清
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "修改患者数据传输对象")
public class PatientUpdateDTO extends PatientCreateDTO {

    @Schema(
            description = "患者状态编码",
            example = "ACTIVE",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private String statusEnum;

    @Schema(
            description = "启用标记",
            example = "true",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Boolean activeFlag;

    @Schema(
            description = "临时建档标记",
            example = "false",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Boolean tempFlag;

    @Schema(
            description = "乐观锁版本号",
            example = "0",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private Integer version;
}