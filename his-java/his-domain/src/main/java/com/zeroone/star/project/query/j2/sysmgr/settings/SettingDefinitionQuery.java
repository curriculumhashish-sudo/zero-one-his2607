package com.zeroone.star.project.query.j2.sysmgr.settings;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：设置管理查询配置
 * </p>
 * @author L
 * @version 1.0.0
 */
@Data
@Schema(description = "设置管理查询配置")
public class SettingDefinitionQuery extends PageQuery {
    @Schema(description = "关键字", example = "医保开关",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String settingDefinitionKeywords;
}
