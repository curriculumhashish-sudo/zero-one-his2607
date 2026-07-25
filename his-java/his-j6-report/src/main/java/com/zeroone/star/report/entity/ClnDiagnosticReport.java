package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * PEIS/临床总检 DiagnosticReport 事实（非 LIS/PACS 报告）
 * @TableName cln_diagnostic_report
 * @author wanfengzhiyunyi
 */
@TableName(value ="cln_diagnostic_report")
@Data
public class ClnDiagnosticReport implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 机构标识
     */
    private String orgId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Integer isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    private Integer version;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * 患者标识
     */
    private String patientId;

    /**
     * 状态枚举
     */
    private String statusEnum;

    /**
     * 摘要文本
     */
    private String summaryText;

    /**
     * 结论文本
     */
    private String conclusionText;

    /**
     * 结果引用 JSON
     */
    private String resultRefsJson;

    /**
     * PDF 存储键
     */
    private String pdfStorageKey;

    /**
     * 定稿时间
     */
    private LocalDateTime finalizedAt;

    /**
     * 定稿人标识
     */
    private String finalizedBy;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}