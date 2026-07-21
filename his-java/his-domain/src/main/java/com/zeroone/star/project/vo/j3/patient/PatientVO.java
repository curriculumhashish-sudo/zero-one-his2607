package com.zeroone.star.project.vo.j3.patient;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：患者详情VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 波小夏
 * @version 1.0.0
 */
@Data
public class PatientVO {

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

    @Schema(description = "启用标记")
    private Boolean activeFlag;

    @Schema(description = "临时建档标记")
    private Boolean tempFlag;

    @Schema(description = "地址")
    private String address;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "血型ABO")
    private Integer bloodTypeAbo;

    @Schema(description = "血型RH")
    private Integer bloodTypeRh;

    @Schema(description = "国家地区编码")
    private String countryCode;

    @Schema(description = "联系人姓名")
    private String linkName;

    @Schema(description = "联系人关系编码")
    private Integer linkRelationCode;

    @Schema(description = "联系人电话")
    private String linkTelcom;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "患者编号")
    private String busNo;

    @Schema(description = "建档机构ID")
    private String organizationId;

    @Schema(description = "乐观锁版本号")
    private Integer version;
}
