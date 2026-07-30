package com.zeroone.star.project.vo.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * 上级科室简要信息（下拉列表用）
 * @author mie
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "上级科室简要信息")
public class DepartmentSimpleVO implements Serializable {

    @Schema(description = "科室ID", example = "322663218269917184")
    private String id;

    @Schema(description = "科室名称", example = "门诊内科")
    private String name;
}
