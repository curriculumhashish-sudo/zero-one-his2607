package com.zeroone.star.statdata.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
/**
 * <p>描述: 物资申请表</p>
 * <p>版权: &copy;01星球</p>
 * <p>地址: 01星球总部</p>
 *
 * @author highxin
 * @version 1.0.0
 */
@Data
@TableName("wkf_supply_request")
public class WkfSupplyRequest {

    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Integer isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
    private Integer version;
    private String busNo;
    private String originalBusNo;
    private String typeEnum;
    private String statusEnum;
    private String intentEnum;
    private String categoryEnum;
    private String priorityEnum;
    private String summaryId;
    private String deliverId;
    private String patientId;
    private String itemTable;
    private BigDecimal itemQuantity;
    private BigDecimal totalQuantity;
    private String itemId;
    private String unitCode;
    private LocalDateTime occurrenceTime;
    private String practitionerId;
    private String supplierId;
    private String reason;
    private String sourceTypeEnum;
    private String sourceLocationId;
    private String purposeTypeEnum;
    private String purposeLocationId;
    private String approverId;
    private LocalDateTime approvalTime;
    private String applicantId;
    private LocalDateTime applyTime;
    private LocalDateTime startTime;
    private LocalDateTime endTime;
    private String lotNumber;
    private String traceNo;
    private BigDecimal price;
    private BigDecimal totalPrice;
    private String remake;
    private BigDecimal profitLossPrice;
}