package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：机构/科室DO（执行地点）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("adm_organization")
public class AdmOrganizationDO extends BaseEntity {

    /**
     * 机构/科室编码
     */
    private String orgCode;

    /**
     * 机构/科室名称
     */
    private String orgName;

    /**
     * 机构类型：ORG-机构, DEPT-科室
     */
    private String orgType;

    /**
     * 父级ID
     */
    private String parentId;

    /**
     * 排序号
     */
    private Integer sortOrder;

    /**
     * 是否启用：0-禁用，1-启用
     */
    private Integer isEnabled;

    /**
     * 联系电话
     */
    private String telephone;

    /**
     * 地址
     */
    private String address;

    /**
     * 备注
     */
    private String remark;
}
