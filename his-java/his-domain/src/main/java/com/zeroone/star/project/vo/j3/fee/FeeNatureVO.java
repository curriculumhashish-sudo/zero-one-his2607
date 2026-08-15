package com.zeroone.star.project.vo.j3.fee;

import com.zeroone.star.project.enums.j3.fee.FeeCategoryEnum;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：费用性质视图对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
public class FeeNatureVO implements Serializable {

    @Schema(description = "费用性质ID", example = "abc123", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "费用性质编码（业务编号）", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer displayOrder;

    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer activeFlag;
}
