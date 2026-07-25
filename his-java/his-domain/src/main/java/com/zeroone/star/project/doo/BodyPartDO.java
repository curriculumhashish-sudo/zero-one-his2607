package com.zeroone.star.project.doo;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("dia_body_structure")
public class BodyPartDO {
    //创建实体类，对应数据库表dia_body_structure
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
    private String name;
    private String statusEnum;
    private String pyStr;
    private String wbStr;
    private String parentId;
}