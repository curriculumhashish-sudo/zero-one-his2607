package com.zeroone.star.project.j4.basedata.dto;

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

    private String id;

    private String statusEnum;
}