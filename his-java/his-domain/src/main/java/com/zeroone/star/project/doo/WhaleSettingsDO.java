package com.zeroone.star.project.doo;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：系统配置项领域模型，对应 whale_settings 表
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
@Data
@TableName("whale_settings")
public class WhaleSettingsDO {

    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    private String name;

    private String value;

    private String providerName;

    private String providerKey;

    private String tenantId;

    private String description;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    private Integer isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;
}
