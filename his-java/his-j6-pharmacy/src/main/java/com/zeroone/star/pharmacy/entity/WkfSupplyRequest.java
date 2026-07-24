package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@AllArgsConstructor
@NoArgsConstructor
@TableName("wkf_supply_request")
public class WkfSupplyRequest implements Serializable {
    private static final long serialVersionUID = 1L;

    @TableId("id")
    private String id;

    @TableField("bus_no")
    private String busNo;

    @TableField("type_enum")
    private String typeEnum;

    @TableField("status_enum")
    private String statusEnum;

    @TableField("item_table")
    private String itemTable;

    @TableField("item_id")
    private String itemId;

    @TableField("unit_code")
    private String unitCode;

    @TableField("item_quantity")
    private BigDecimal itemQuantity;

    @TableField("total_quantity")
    private BigDecimal totalQuantity;

    @TableField("purpose_location_id")
    private String purposeLocationId;

    @TableField("lot_number")
    private String lotNumber;

    @TableField("applicant_id")
    private String applicantId;

    @TableField("apply_time")
    private LocalDateTime applyTime;

    @TableField("approver_id")
    private String approverId;

    @TableField("approval_time")
    private LocalDateTime approvalTime;

    private String reason;

    @TableField("updated_at")
    private LocalDateTime updatedAt;

    @TableField("updated_by")
    private String updatedBy;

    @TableField("is_deleted")
    private Integer isDeleted;
}