package com.zeroone.star.project.vo.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：下拉选项VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "通用下拉选项")
public class OptionVO implements Serializable {

    @Schema(description = "选项ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "选项名称", example = "内科疗区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
}
