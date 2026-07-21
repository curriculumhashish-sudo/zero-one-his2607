package com.zeroone.star.project.dto.j4.treatment;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：诊疗启/停用请求DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
public class TreatmentStatusDTO {

    @NotBlank(message = "诊疗项目ID不能为空")
    private String id;

    @NotBlank(message = "状态不能为空")
    private String statusEnum;
}