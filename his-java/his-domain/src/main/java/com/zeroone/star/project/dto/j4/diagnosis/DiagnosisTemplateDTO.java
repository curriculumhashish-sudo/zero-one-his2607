package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * <p>
 * 描述：诊断模板增/改传输数据对象，对应 cus_condition_template 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "诊断模板增改数据对象")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DiagnosisTemplateDTO {

    /**
     * 主键（编辑时必填，新增时不填）
     */
    @Schema(description = "模板主键（编辑时必填）")
    private String id;

    /**
     * 模板归属主体（医生 ID / 科室 ID / 全院标识）
     */
    @Schema(description = "模板归属主体（医生ID / 科室ID / 全院标识）", example = "322963602154721280",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String objectId;

    /**
     * 绑定类型枚举：个人 / 科室 / 全院
     */
    @Schema(description = "绑定类型枚举：个人 / 科室 / 全院", example = "个人", requiredMode = Schema.RequiredMode.REQUIRED)
    private String bindingEnum;

    /**
     * 备注
     */
    @Schema(description = "备注", example = "内科常用诊断模板")
    private String remark;

    /**
     * 模板诊断项列表
     */
    @Schema(description = "模板诊断项列表（不可为空）", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<TemplateItem> items;

    /**
     * 模板诊断项
     */
    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    @Schema(description = "模板诊断项")
    public static class TemplateItem {

        /**
         * 诊断定义主键 → 关联 cln_condition_definition.id
         */
        @Schema(description = "诊断定义主键", example = "317932056501817344", requiredMode = Schema.RequiredMode.REQUIRED)
        private String definitionId;

        /**
         * 排序权重
         */
        @Schema(description = "排序权重（越小越靠前）", example = "1")
        private Integer sortWeight;
    }
}
