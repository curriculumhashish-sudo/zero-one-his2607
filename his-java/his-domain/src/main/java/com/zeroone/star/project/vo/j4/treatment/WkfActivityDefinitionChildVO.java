package com.zeroone.star.project.vo.j4.treatment;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 诊疗项目子项显示对象
 *
 * 【前端对接说明】
 * 1. 本对象用于组合项目（如套餐）的子项列表展示
 * 2. 当TreatmentDetailVO.childrenFlag=1时，children数组包含本对象
 * 3. childrenId为子项目的主键ID，可通过该ID再次调用get-treatment接口获取子项详情
 * 4. childrenName为创建组合项目时的名称快照，可能与当前子项实际名称不一致
 * 5. childrenNum为该子项在组合项目中的数量（如一次套餐包含2次静脉注射）
 */
@Data
@Schema(description = "诊疗项目子项显示对象")
public class WkfActivityDefinitionChildVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @JsonSerialize(using = ToStringSerializer.class)
    @Schema(description = "主键", example = "1890000000000000002")
    private String id;

    @JsonSerialize(using = ToStringSerializer.class)
    @Schema(description = "父项标识（外键：wkf_activity_definition.id）", example = "1890000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String activityId;

    @JsonSerialize(using = ToStringSerializer.class)
    @Schema(description = "子项诊疗项目标识（被引用项目主键）", example = "1890000000000000003", requiredMode = Schema.RequiredMode.REQUIRED)
    private String childrenId;

    @Schema(description = "子项数量", example = "1")
    private Integer childrenNum;

    @Schema(description = "子项名称快照", example = "静脉注射")
    private String childrenName;
}
