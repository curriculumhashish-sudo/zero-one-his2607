package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：医保对照专用传输数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "医保对照数据对象")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class YbMappingDTO {

    /**
     * 诊断定义主键
     */
    @Schema(description = "诊断定义主键", example = "317932056501817344", requiredMode = Schema.RequiredMode.REQUIRED)
    private String definitionId;

    /**
     * 医保编号
     */
    @Schema(description = "医保编号", example = "YB00123456")
    private String ybNo;

    /**
     * 医保标记：0 否 / 1 是
     */
    @Schema(description = "医保标记：0 否 / 1 是", example = "1")
    private Boolean ybFlag;

    /**
     * 医保匹配标记：0 未匹配 / 1 已匹配
     */
    @Schema(description = "医保匹配标记：0 未匹配 / 1 已匹配", example = "1")
    private Boolean ybMatchFlag;
}
