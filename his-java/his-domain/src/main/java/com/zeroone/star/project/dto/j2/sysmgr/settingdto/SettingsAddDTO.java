package com.zeroone.star.project.dto.j2.sysmgr.settingdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：新增配置项数据传输对象
 * </p>
 * @author L
 * @version 1.0.0
 */

@Data
@Schema(description = "新增配置项数据传输对象")
public class SettingsAddDTO {
    @Schema(description = "配置键名", example = "ybSwich", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "配置键名不能为空")
    private String name;

    @Schema(description = "提供者名称", example = "G-全局", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "提供者名称不能为空")
    private String provider_name;

    @Schema(description = "提供者键", example = "kuguan - 库管", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "提供者键不能为空")
    private String provider_key;

    @Schema(description = "值", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "配置值不能为空")
    private String value;
}
