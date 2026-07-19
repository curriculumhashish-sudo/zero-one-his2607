package com.zeroone.star.project.dto.j4.treatment;

import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：诊疗管理请求DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
public class TreatmentDTO {

    private String id;

    private String tenantId;

    @NotBlank(message = "诊疗项目类别编码不能为空")
    private String categoryCode;

    @NotBlank(message = "业务编码不能为空")
    private String busNo;

    @NotBlank(message = "项目名称不能为空")
    private String name;

    private String pyStr;

    private String wbStr;

    private String packageUnitCode;

    @NotBlank(message = "诊疗类型不能为空")
    private String typeEnum;

    @NotBlank(message = "发布状态不能为空")
    private String statusEnum;

    private String descriptionText;

    private String bodySiteCode;

    private String specimenCode;

    private String orgId;

    private String locationId;

    private Integer ruleId;

    private Integer pricingFlag;

    private Integer ybFlag;

    private String ybNo;

    private Integer ybMatchFlag;

    private Integer chrgitmLv;

    private String childrenJson;

    private Integer childrenFlag;

    private String activityVersion;

    private String merchandiseName;

    private String merchandisePyStr;

    private String merchandiseWbStr;

    private String defEncounterUnitCode;

    private String defInhospitalUnitCode;
}