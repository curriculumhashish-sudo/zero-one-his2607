package com.zeroone.star.project.dto.j7.outpharm;

import com.zeroone.star.project.dto.PageDTO;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 获取患者队列 前端请求入参DTO
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class PatientQueueDTO extends PageDTO {
    /** 租户ID */
    private String tenantId;
    /** 患者业务编号 */
    private String busNo;
    /** 患者姓名 */
    private String name;
    /** 性别编码 */
    private String genderEnum;
    /** 身份证号 */
    private String idCard;
    /** 联系电话 */
    private String phone;
    /** 患者状态编码 */
    private String statusEnum;
    /** 有效标识 0/1 */
    private Integer activeFlag;
    /** 所属机构ID */
    private String organizationId;
}
