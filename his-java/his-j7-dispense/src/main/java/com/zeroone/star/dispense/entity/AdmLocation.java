package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("adm_location")
public class AdmLocation implements Serializable {
    private static final long serialVersionUID = 1L;
    @TableId
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
    private String name;
    private String busNo;
    private String statusEnum;
    private String operationalEnum;
    private String formEnum;
    private String modeEnum;
    private String typeJson;
    private String organizationId;
    private Integer displayOrder;
    private String pyStr;
    private String wbStr;
    private String parentId;
}