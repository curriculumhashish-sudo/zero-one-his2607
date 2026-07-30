package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：供应交付实体类，对应数据库表 {@code wkf_supply_delivery}
 * </p>
 * <p>
 * 功能说明：记录供应发放/交付信息，包含关联的供应请求ID、汇总ID、
 * 发放状态、发放类型、物品表名和标识、数量、批号、供应商、
 * 发放时间、接收人等。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("wkf_supply_delivery")
public class WkfSupplyDelivery implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 */
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    private Integer version;

    /** 供应发放聚合根 */
    private String requestId;

    /** summary标识 */
    private String summaryId;

    /** 发放状态持久化码 */
    private String statusEnum;

    /** 患者标识 */
    private String patientId;

    /** 发放类型/业务子类型持久化码 */
    private String typeEnum;

    /** 项table */
    private String itemTable;

    /** 项标识 */
    private String itemId;

    /** 依据表名 */
    private String basedOnTable;

    /** 依据ID列表 */
    private String basedOnIds;

    /** 单元编码 */
    private String unitCode;

    /** 数量 */
    private BigDecimal quantity;

    /** 病情（condition 是 MySQL 保留字，需反引号转义） */
    @TableField("`condition`")
    private String condition;

    /** 批号 */
    private String lotNumber;

    /** 追踪编号 */
    private String traceNo;

    /** 供应商标识 */
    private String supplierId;

    /** 医护人员标识 */
    private String practitionerId;

    /** occurrence时间 */
    private LocalDateTime occurrenceTime;

    /** receiver标识 */
    private String receiverId;

    /** receive时间 */
    private LocalDateTime receiveTime;
}
