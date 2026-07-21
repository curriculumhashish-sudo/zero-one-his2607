package com.zeroone.star.project.vo.j5.configuration_management.medical_record_template;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：获取文书类型名称
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author yjyl
 * @version 1.0.0
 */

@Schema(description = "获取文书类型名称")
@Data
public class EmrDocumentTypeNameVO {

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
}
