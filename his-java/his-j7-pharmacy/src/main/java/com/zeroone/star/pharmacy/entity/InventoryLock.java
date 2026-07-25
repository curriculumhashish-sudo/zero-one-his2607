package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：库存锁定实体类，对应数据库表 {@code inventory_lock}
 * </p>
 * <p>
 * 功能说明：记录库存锁定信息，包含库存项标识、锁定标识、锁定状态、
 * 业务单号、顺序号等。用于在发药、退药等操作时对库存进行锁定/解锁。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("inventory_lock")
public class InventoryLock implements Serializable {

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

    /** 库存锁定记录 */
    private String inventoryId;

    /** 锁定标识：true 已锁定、false 未锁定 */
    private Integer lockFlag;

    /** 锁定状态：见 LockStatus */
    private String status;

    /** 业务单号 */
    private String busNo;

    /** 同一业务单内的顺序号 */
    private Integer orderFlag;
}
