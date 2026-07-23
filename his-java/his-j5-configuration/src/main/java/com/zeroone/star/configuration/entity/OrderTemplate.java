package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@AllArgsConstructor
@Data
@TableName("cus_order_template")
public class OrderTemplate implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    private Integer version;

    private String objectId;

    private String bindingEnum;

    private String sceneEnum;

    private String templateCode;

    private String name;

    private String description;

    private Integer sortWeight;

    private String statusEnum;

    private String remark;

    private String templateKindEnum;

    private BigDecimal defaultChineseHerbsDoseQuantity;

    private Boolean defaultSufferingFlag;

    private String templateSourceEnum;
}
