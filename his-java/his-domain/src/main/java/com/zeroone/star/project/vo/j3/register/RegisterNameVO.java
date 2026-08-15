package com.zeroone.star.project.vo.j3.register;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：挂号类型名称下拉VO（下拉选择组件专用，仅返回 id + name）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "挂号类型名称下拉视图对象")
public class RegisterNameVO implements Serializable {

    @Schema(description = "挂号类型唯一标识", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "挂号类型名称", example = "普通号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
}
