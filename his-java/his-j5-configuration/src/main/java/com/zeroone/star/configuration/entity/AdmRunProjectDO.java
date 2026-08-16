package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：执行项目DO（映射adm_healthcare_service表）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("adm_healthcare_service")
public class AdmRunProjectDO extends BaseEntity {

    /**
     * 项目名称
     */
    private String name;

    /**
     * 类型编码
     */
    private String typeCode;

    /**
     * 分类编码
     */
    private String categoryCode;

    /**
     * 执行机构ID
     */
    private String offeredOrgId;

    /**
     * 专科编码
     */
    private String specialtyCode;

    /**
     * 执行地点ID
     */
    private String locationId;

    /**
     * 状态枚举
     */
    private String statusEnum;

    /**
     * 启用标志
     */
    private Integer activeFlag;

    /**
     * 是否需要预约
     */
    private Integer appointmentRequiredFlag;

    /**
     * 医保编号
     */
    private String ybNo;

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 备注
     */
    private String comment;

    /**
     * 扩展详情
     */
    private String extraDetails;

    /**
     * 联系方式
     */
    private String contact;
}
