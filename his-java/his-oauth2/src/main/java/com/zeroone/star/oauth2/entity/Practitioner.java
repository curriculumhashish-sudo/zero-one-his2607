package com.zeroone.star.oauth2.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;

/**
 * <p>
 * 医务人员表（adm_practitioner）
 * </p>
 *
 * @author 阿伟
 */
@Getter
@Setter
@ToString
@TableName("adm_practitioner")
public class Practitioner implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 医务人员编号（adm_practitioner.id），字符串形式与 User.practitionerId 保持一致
     */
    @TableId(value = "id")
    private String id;

    /**
     * 关联用户编号（adm_practitioner.user_id）
     */
    @TableField("user_id")
    private String userId;

    /**
     * 姓名（adm_practitioner.name）
     */
    @TableField("name")
    private String name;

    /**
     * 职称编码（adm_practitioner.dr_profttl_code）
     */
    @TableField("dr_profttl_code")
    private Integer drProfttlCode;

    /**
     * 逻辑删除标志（adm_practitioner.is_deleted）
     */
    @TableField("is_deleted")
    private Integer isDeleted;
}