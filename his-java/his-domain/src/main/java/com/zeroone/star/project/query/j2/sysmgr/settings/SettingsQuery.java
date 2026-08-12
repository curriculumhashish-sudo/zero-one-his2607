package com.zeroone.star.project.query.j2.sysmgr.settings;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：配置项查询配置
 * </p>
 * @author L
 * @version 1.0.0
 */

@Data
@Schema(description = "配置项查询配置")
public class SettingsQuery extends PageQuery {
    @Schema(description = "配置键名", example = "ybSwich", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String name;
    @Schema(description = "提供者名称", example = "U-用户",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String provider_name;
    @Schema(description = "提供者键", example = "kuguan - 库管",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String provider_key;
    @Schema(description = "值", example = "1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String value;
}
