package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 实体变更审计日志
 * @TableName whale_audit_logs
 * @author wanfengzhiyunyi
 */
@TableName(value ="whale_audit_logs")
@Data
public class WhaleAuditLogs implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 实体类型
     */
    private String entityType;

    /**
     * 实体 ID
     */
    private String entityId;

    /**
     * 操作类型
     */
    private String operationType;

    /**
     * 操作人
     */
    private String userId;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * IP 地址
     */
    private String ipAddress;

    /**
     * 用户agent
     */
    private String userAgent;

    /**
     * 变更内容
     */
    private String changes;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}