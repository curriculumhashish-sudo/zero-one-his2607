package com.zeroone.star.j3.register.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：医疗服务实体（映射 adm_healthcare_service 表）
 * </p>
 * <p>新增挂号类型时，如果名称匹配不到已有服务则同步新建一条记录</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("adm_healthcare_service")
public class HealthcareService implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId("id")
    private String id;

    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    private Integer isDeleted;

    private Integer version;

    /** 服务名称 */
    private String name;

    /** 服务类型编码 */
    private String typeCode;

    /**
     * 服务类别编码（挂号类型统一标记）
     */
    private String categoryCode;

    /** 服务状态 */
    private String statusEnum;

    /** 激活标志 */
    private Integer activeFlag;

    /** 业务编号 */
    private String busNo;

    /** 医保编号 */
    private String ybNo;

    /** 备注 */
    private String comment;
}
