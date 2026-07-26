package com.zeroone.star.project.dto.j2.sysmgr.settingdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "更新配置项数据传输对象")
public class SettingUpdateDTO {

    @Schema(description = "配置项ID", example = "322347974589943808", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "配置项ID不能为空")
    private String id;

    @Schema(description = "配置值", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "配置值不能为空")
    private String value;

}
