package com.zeroone.star.project.vo.j7.outpharm;

import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * 患者队列列表返回VO
 */
@Data
public class PatientQueueVO {
    // 主键
    private String id;
    // 租户
    private String tenantId;
    // 患者业务号
    private String busNo;
    // 姓名
    private String name;
    // 性别编码
    private String genderEnum;
    // 出生日期
    private LocalDate birthDate;
    // 身份证
    private String idCard;
    // 手机号
    private String phone;
    // 就诊状态
    private String statusEnum;
    // 有效标识
    private Integer activeFlag;
    // 地址-省市区街道
    private String addressProvince;
    private String addressCity;
    private String addressDistrict;
    private String addressStreet;
    // 民族编码
    private String nationalityCode;
    // ABO血型
    private String bloodTypeAbo;
    // RH血型
    private String bloodTypeRh;
    // 工作单位
    private String workCompany;
    // 联系人姓名
    private String linkName;
    // 联系人电话
    private String linkTelcom;
    // 创建时间
    private LocalDateTime createdAt;
    // 所属机构
    private String organizationId;
}
