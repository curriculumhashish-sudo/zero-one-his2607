package com.zeroone.star.project.query.j7.outpharm;

import com.zeroone.star.project.query.PageQuery;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 患者队列查询条件（数据库层）
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class PatientQueueQuery extends PageQuery {
    // 租户
    private String tenantId;
    // 患者业务号
    private String busNo;
    // 姓名
    private String name;
    // 性别编码
    private String genderEnum;
    // 身份证
    private String idCard;
    // 手机号
    private String phone;
    // 就诊状态
    private String statusEnum;
    // 有效标识
    private Integer activeFlag;
    // 机构ID
    private String organizationId;
}
