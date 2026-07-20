package com.zeroone.star.project.vo.j7.outpharm;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * 患者队列列表返回VO
 */
@Data
@Schema(description = "患者队列返回结果")
public class PatientQueueVO {

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "租户ID")
    private String tenantId;

    @Schema(description = "患者业务编号")
    private String busNo;

    @Schema(description = "患者姓名")
    private String name;

    @Schema(description = "性别编码(gender_enum)")
    private String genderEnum;

    @Schema(description = "出生日期")
    private LocalDate birthDate;

    @Schema(description = "身份证号")
    private String idCard;

    @Schema(description = "联系电话")
    private String phone;

    @Schema(description = "就诊状态(status_enum)")
    private String statusEnum;

    @Schema(description = "有效标识(0-无效 1-有效)")
    private Integer activeFlag;

    @Schema(description = "省")
    private String addressProvince;

    @Schema(description = "市")
    private String addressCity;

    @Schema(description = "区")
    private String addressDistrict;

    @Schema(description = "街道")
    private String addressStreet;

    @Schema(description = "民族编码")
    private String nationalityCode;

    @Schema(description = "ABO血型")
    private String bloodTypeAbo;

    @Schema(description = "RH血型")
    private String bloodTypeRh;

    @Schema(description = "工作单位")
    private String workCompany;

    @Schema(description = "联系人姓名")
    private String linkName;

    @Schema(description = "联系人电话")
    private String linkTelcom;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "所属机构ID")
    private String organizationId;
}