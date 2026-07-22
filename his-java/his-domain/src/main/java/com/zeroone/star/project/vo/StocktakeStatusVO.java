package com.zeroone.star.project.vo;

import java.math.BigDecimal;
import java.time.LocalDateTime;

public class StocktakeStatusVO {
    /**
     * 盘点单号
     */
    private String busNo;
    /**
     * 类型枚举
     */
    private String typeEnum;
    /**
     * 状态枚举：DRAFT / PENDING_APPROVAL / AGREE / REJECT
     */
    private String statusEnum;

    /**
     * 制单人
     */
    private String createdBy;
    /**
     * 制单时间
     */
    private LocalDateTime createdAt;
    /**
     * 提交审批人
     */
    private String applicantId;
    /**
     * 提交审批时间
     */
    private LocalDateTime applyTime;
    /**
     * 审批人
     */
    private String approverId;
    /**
     * 审批时间
     */
    private LocalDateTime approvalTime;

    /**
     * 盘点总价（所有明细 total_price 之和）
     */
    private BigDecimal totalAmount;
}
