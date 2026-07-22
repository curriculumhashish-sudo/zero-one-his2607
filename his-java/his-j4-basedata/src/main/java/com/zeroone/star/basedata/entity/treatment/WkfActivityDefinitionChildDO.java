package com.zeroone.star.basedata.entity.treatment;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

/**
 * @Description:
 * @Created: with IntelliJ IDEA.
 * @author: yc
 * @createTime: 2026-07-21 19:42
 **/
@Getter
@Setter
@TableName("wkf_activity_definition_children")
public class WkfActivityDefinitionChildDO {
    /** 父项标识（外键：wkf_activity_definition.id） */
    @TableField("activity_id")
    @Schema(description = "父项标识（外键：wkf_activity_definition.id）", example = "123456", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long activityId;

    /** 子项诊疗项目标识（被引用项目主键） */
    @TableField("children_id")
    @Schema(description = "子项诊疗项目标识（被引用项目主键）", example = "654321", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long childrenId;

    /** 子项数量 */
    @TableField("children_num")
    @Schema(description = "子项数量", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer childrenNum;

    /** 子项名称快照 */
    @TableField("children_name")
    @Schema(description = "子项名称快照", example = "常规体检子项目", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String childrenName;
}
