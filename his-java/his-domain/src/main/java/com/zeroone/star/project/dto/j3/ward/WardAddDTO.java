package com.zeroone.star.project.dto.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：病区添加数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@Schema(description = "病区添加数据传输对象")
public class WardAddDTO {
    @Schema(description = "病区名称", example = "内科疗区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String wardName;
    @Schema(description = "关联科室", example = "住院内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String departmentCode;
}
