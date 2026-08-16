package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 电子病历 document
 * @TableName emr_document
 * @author wanfengzhiyunyi
 */
@TableName(value ="emr_document")
@Data
public class EmrDocument implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 电子病历文书实体
     */
    private String patientId;

    /**
     * 就诊 ID
     */
    private String encounterId;

    /**
     * 文书类型编码，如入院记录、病程记录等
     */
    private String documentTypeCode;

    /**
     * 文档标题
     */
    private String title;

    /**
     * 文档正文内容，Tiptap/ProseMirror JSON
     */
    private String content;

    /**
     * 文档状态：DRAFT / SUBMITTED / LOCKED / ARCHIVED 等
     */
    private String status;

    /**
     * 创建文书时使用的模板 ID
     */
    private String sourceTemplateId;

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
     * 租户标识
     */
    private String tenantId;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}