package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：停用诊断定义的校验结果视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Schema(description = "停用校验结果视图对象")
@Data
public class DisableCheckResultVO {

    /**
     * 是否可停用
     */
    @Schema(description = "是否可停用（true 无引用可直接停用 / false 存在引用不可停用）", example = "false")
    private Boolean canDisable;

    /**
     * 引用明细列表（不可停用时返回）
     */
    @Schema(description = "引用明细列表（不可停用时返回）")
    private List<ReferenceItem> references;

    /**
     * 引用明细项
     */
    @Data
    @Schema(description = "引用明细项")
    public static class ReferenceItem {

        @Schema(description = "引用业务类型（收费项目 / 药品医嘱 / 处置服务 / 耗材请求 / 诊断模板）", example = "收费项目")
        private String businessType;

        @Schema(description = "引用记录主键", example = "329701000000000000")
        private String recordId;

        @Schema(description = "引用记录名称/描述", example = "血常规检查")
        private String recordName;
    }
}
