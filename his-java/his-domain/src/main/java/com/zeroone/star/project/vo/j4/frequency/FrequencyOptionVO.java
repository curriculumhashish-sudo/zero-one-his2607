package com.zeroone.star.project.vo.j4.frequency;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.io.Serializable;

/**
 * <p>
 * 描述：频次下拉项显示数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "频次下拉选项显示对象")
@Data
public class FrequencyOptionVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "排序号", example = "30", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sort;

    @Schema(description = "下拉框选定值（频次编码）", example = "TID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String value;

    @Schema(description = "下拉框文本（频次名称）", example = "每日三次", requiredMode = Schema.RequiredMode.REQUIRED)
    private String label;
}
