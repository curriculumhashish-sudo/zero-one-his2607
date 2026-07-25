package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 收费员交账头表（只增不改）
 * @TableName fin_cashier_settlement
 * @author wanfengzhiyunyi
 */
@TableName(value ="fin_cashier_settlement")
@Data
public class FinCashierSettlement implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Integer isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    private Integer version;

    /**
     * 交账单号
     */
    private String busNo;

    /**
     * 类型枚举值
     */
    private String typeEnum;

    /**
     * HANDED_OVER（已交账）
     */
    private String statusEnum;

    /**
     * 交账提交时刻
     */
    private LocalDateTime settledAt;

    /**
     * 交账所属科室
     */
    private String orgId;

    /**
     * 收费窗口（可选）
     */
    private String locationId;

    /**
     * 交账经办人
     */
    private String cashierId;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}