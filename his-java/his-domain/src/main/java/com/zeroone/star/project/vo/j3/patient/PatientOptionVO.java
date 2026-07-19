package com.zeroone.star.project.vo.j3.patient;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>
 * 描述：患者选择器选项VO（仅关键识别字段，用于挂号/住院等选择患者场景）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 波小夏
 * @version 1.0.0
 */
@Data
public class PatientOptionVO {

    @Schema(description = "患者ID")
    private String id;

    @Schema(description = "患者姓名")
    private String name;

    @Schema(description = "性别编码")
    private String genderEnum;

    @Schema(description = "出生日期")
    private LocalDate birthDate;

    @Schema(description = "身份证号")
    private String idCard;

    @Schema(description = "联系电话")
    private String phone;

    @Schema(description = "患者状态编码")
    private String statusEnum;

    @Schema(description = "是否启用")
    private Boolean activeFlag;
}
