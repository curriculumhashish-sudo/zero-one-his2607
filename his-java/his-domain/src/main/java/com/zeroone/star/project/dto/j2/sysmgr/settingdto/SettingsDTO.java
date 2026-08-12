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
    @Schema(description = "配置项ID", example = "123456")
    private String id;
    @Schema(description = "配置键名", example = "ybSwich")
    private String name;
    @Schema(description = "配置描述", example = "医保开关")
    private String description;
    @Schema(description = "提供者名称", example = "U", requiredMode = Schema.RequiredMode.REQUIRED)
    private String provider_name;

    @Schema(description = "提供者键", example = "U-user", requiredMode = Schema.RequiredMode.REQUIRED)
    private String provider_key;

    @Schema(description = "值", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String value;

    @Schema(description = "创建时间", example = "2026-07-26 18:57:06")
    private String createdAt;
    @Schema(description = "更新时间", example = "2026-08-11 10:30:00")
    private String updatedAt;
}
