package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：医师下拉选项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "医师列表")
public class PractitionerOptionDTO {

    @Schema(description = "医师主键", example = "322963602326687744")
    private String practitionerId;

    @Schema(description = "医师姓名", example = "门诊医生")
    private String practitionerName;
}
