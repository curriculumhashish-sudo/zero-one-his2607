package com.zeroone.star.project.dto.j2.sysmgr.settingdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "删除配置项数据传输对象")
public class SettingDeleteDTO {

    @Schema(description = "配置项ID", example = "322347974589943808", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
