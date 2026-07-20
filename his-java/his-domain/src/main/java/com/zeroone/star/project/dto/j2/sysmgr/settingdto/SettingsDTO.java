package com.zeroone.star.project.dto.j2.sysmgr.settingdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：配置项数据传输对象
 * </p>
 * @author L
 * @version 1.0.0
 */

@Data
@Schema(description = "配置项数据传输对象")
public class SettingsDTO {
    @Schema(description = "提供者名称", example = "U",requiredMode = Schema.RequiredMode.REQUIRED)
    private String provider_name;
    @Schema(description = "提供者键", example = "U-user",requiredMode = Schema.RequiredMode.REQUIRED)
    private String provider_key;
    @Schema(description = "值",example = "1",requiredMode = Schema.RequiredMode.REQUIRED)
    private String value;
}
