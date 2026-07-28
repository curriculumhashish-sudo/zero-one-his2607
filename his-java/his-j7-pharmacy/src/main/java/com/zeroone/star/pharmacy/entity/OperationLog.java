package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：操作日志实体类，对应数据库表 {@code whale_security_logs}
 * </p>
 * <p>
 * 功能说明：该表为安全审计日志表，记录用户在系统中的操作行为。
 * 包含操作人信息、操作动作、客户端IP、UserAgent、操作结果等核心字段。
 * 在药房管理业务中，可用于记录发药、退药、盘点等关键操作的审计日志，
 * 便于追溯和安全审计。操作结果（{@code result}）为 SUCCESS 表示成功，
 * FAILURE 表示失败。失败时通过 {@code failureReason} 记录失败原因。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("whale_security_logs")
public class OperationLog implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 用户标识
     * 对应表字段：user_id
     */
    private String userId;

    /**
     * 用户名称
     * 对应表字段：user_name
     */
    private String userName;

    /**
     * 操作动作
     * 对应表字段：action，如 INSERT/UPDATE/DELETE/QUERY 等
     */
    private String action;

    /**
     * 客户端IP
     * 对应表字段：client_ip
     */
    private String clientIp;

    /**
     * 用户agent
     * 对应表字段：user_agent，记录浏览器或客户端信息
     */
    private String userAgent;

    /**
     * 操作结果
     * 对应表字段：result
     * SUCCESS-成功 FAILURE-失败
     */
    private String result;

    /**
     * 失败原因
     * 对应表字段：failure_reason，操作失败时记录原因
     */
    private String failureReason;

    /**
     * 租户标识
     * 对应表字段：tenant_id
     */
    private String tenantId;

    /**
     * 扩展属性 JSON
     * 对应表字段：extra_properties，存储结构化附加信息
     */
    private String extraProperties;

    /**
     * 创建时间
     * 对应表字段：created_at，操作发生时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     * 对应表字段：updated_at
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     * 对应表字段：created_by
     */
    private String createdBy;

    /**
     * 最后更新人
     * 对应表字段：updated_by
     */
    private String updatedBy;

    /**
     * 软删标记
     * 对应表字段：is_deleted
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     * 对应表字段：deleted_at
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     * 对应表字段：deleted_by
     */
    private String deletedBy;

    /**
     * 判断操作是否成功
     * @return true-成功，false-失败或未知
     */
    public boolean isSuccess() {
        return "SUCCESS".equalsIgnoreCase(result);
    }

    /**
     * 判断操作是否失败
     * @return true-失败，false-成功或未知
     */
    public boolean isFailure() {
        return "FAILURE".equalsIgnoreCase(result);
    }

    /**
     * 标记操作成功
     */
    public void markSuccess() {
        this.result = "SUCCESS";
        this.failureReason = null;
    }

    /**
     * 标记操作失败并记录原因
     * @param reason 失败原因
     */
    public void markFailure(String reason) {
        this.result = "FAILURE";
        this.failureReason = reason;
    }
}