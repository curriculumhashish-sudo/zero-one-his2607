package com.zeroone.star.project.dto.j2.sysmgr.settingdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：配置数据传输对象
 * </p>
 * @author L
 * @version 1.0.0
 */

@Data
@Schema(description = "配置数据传输对象")
public class SettingDefinitionDTO {
    @Schema(description = "序号", example = "1",requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
    @Schema(description = "标题", example = "医保开关",requiredMode = Schema.RequiredMode.REQUIRED)
    private String description;
    @Schema(description = "名称", example = "ybSwich",requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
}
