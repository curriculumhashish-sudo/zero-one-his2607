package com.zeroone.star.project.vo.j5.configuration_management.medical_record_template;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：获取文书类型详情
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */

@Schema(description = "获取文书类型详情")
@Data
public class EmrDocumentTypeDetailVO {

    /**
     * 主键
     */
    @Schema(description = "主键", example = "323100000000000101", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer id;

    /**
     * 文书类型编码
     */
    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    /**
     * 文书类型名称
     */
    @Schema(description = "文书类型名称", example = "门诊病历", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 默认模板ID
     */
    @Schema(description = "默认模板ID", example = "323100000000000201", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defaultTemplateId;

    /**
     * 是否启用
     */
    @Schema(description = "是否启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isEnabled;

    /**
     * 排序
     */
    @Schema(description = "排序", example = "2", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer sortOrder;
}
