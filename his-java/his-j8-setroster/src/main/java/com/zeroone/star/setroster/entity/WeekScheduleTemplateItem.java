package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：周排班模板明细
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("bkg_week_schedule_template_item")
public class WeekScheduleTemplateItem implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    @TableField("week_schedule_template_id")
    private String weekScheduleTemplateId;

    @TableField("weekday")
    private Integer weekday;

    @TableField("day_schedule_template_id")
    private String dayScheduleTemplateId;

    @TableField("sort_no")
    private Integer sortNo;

    @TableField("enabled")
    private Boolean enabled;

    @TableField(fill = FieldFill.INSERT)
    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    private String deletedBy;

    @Version
    private Integer version;
}
