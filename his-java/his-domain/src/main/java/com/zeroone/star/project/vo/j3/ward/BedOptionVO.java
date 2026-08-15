package com.zeroone.star.project.vo.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：床位下拉选项VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "床位下拉选项")
public class BedOptionVO implements Serializable {

    @Schema(description = "床位ID", example = "322663218269917184", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "床位名称", example = "1号床", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "状态 0-停用 1-启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
