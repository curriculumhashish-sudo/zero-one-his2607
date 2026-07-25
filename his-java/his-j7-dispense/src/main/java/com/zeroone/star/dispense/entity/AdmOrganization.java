package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("adm_organization")
public class AdmOrganization implements Serializable {
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
    private String parentId;
    private String typeEnum;
    private String classEnum;
    private String statusEnum;
    private Boolean activeFlag;
    private Integer displayOrder;
    private String defDoctorId;
    private String pyStr;
    private String wbStr;
    private String ybNo;
    private String ybName;
    private String medinsId;
    private String medinsAdmdvs;
    private String medinsType;
    private String medinsLv;
    private String caty;
}