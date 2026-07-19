package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：诊断模板列表项视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "诊断模板列表项视图对象")
@Data
public class DiagnosisTemplateVO {

    @Schema(description = "模板主键")
    private String id;

    @Schema(description = "模板归属主体（医生ID / 科室ID / 全院标识）", example = "322963602154721280")
    private String objectId;

    @Schema(description = "绑定类型枚举：个人 / 科室 / 全院", example = "个人")
    private String bindingEnum;

    @Schema(description = "排序权重", example = "1")
    private Integer sortWeight;

    @Schema(description = "备注", example = "内科常用诊断模板")
    private String remark;

    @Schema(description = "模板诊断项列表")
    private List<TemplateItemVO> items;

    /**
     * 模板诊断项视图
     */
    @Data
    @Schema(description = "模板诊断项视图")
    public static class TemplateItemVO {

        @Schema(description = "诊断定义主键", example = "317932056501817344")
        private String definitionId;

        @Schema(description = "诊断名称", example = "感冒")
        private String definitionName;

        @Schema(description = "诊断编码", example = "CDF00000003")
        private String conditionCode;

        @Schema(description = "排序权重", example = "1")
        private Integer sortWeight;
    }
}
