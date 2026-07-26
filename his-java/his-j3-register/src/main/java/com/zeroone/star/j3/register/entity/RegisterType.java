package com.zeroone.star.j3.register.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：挂号类型实体（映射 adm_charge_item_definition 表，type_code='1012'=挂号费）
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("adm_charge_item_definition")
public class RegisterType implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 收费项目定义ID（雪花ID） */
    @TableId("id")
    private String id;

    /** 租户ID */
    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /** 是否删除 */
    private Integer isDeleted;

    /** 关联业务表 */
    private String instanceTable;

    /** 关联业务ID */
    private String instanceId;

    /** 挂号类型名称（charge_name） */
    @TableField("charge_name")
    private String name;

    /** 状态 ACTIVE/INACTIVE */
    @TableField("status_enum")
    private String status;

    /** 机构ID */
    private String orgId;

    /** 业务编码/分类编码（type_code，1012=挂号费） */
    @TableField("type_code")
    private String businessCode;

    /** 医保编码（yb_type） */
    @TableField("yb_type")
    private String mdtrtCertNo;

    /** 价格类型 */
    private String priceTypeCode;

    /** 金额（price_value） */
    @TableField("price_value")
    private BigDecimal amount;

    /** 优先级 */
    private Integer priority;

    /** 业务编码（从 adm_healthcare_service.bus_no JOIN 获取，非持久化） */
    @TableField(exist = false)
    private String serviceBusNo;
}
