package com.zeroone.star.configuration.domain;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 组套模板头数据库对象，对应 {@code cus_order_template}。
 */
@Data
@TableName("cus_order_template")
public class OrderSetDO {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    @TableLogic(value = "0", delval = "1")
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
