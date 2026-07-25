package com.zeroone.star.project.dto.j3.fee;

import com.zeroone.star.project.enums.j3.fee.FeeCategoryEnum;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;

/**
 * 描述：费用性质数据传输对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
@Schema(description = "费用性质数据对象")
public class FeeNatureDTO implements Serializable {

    @Schema(description = "费用性质ID（新增时不传，修改时必传）", example = "abc123", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String id;

    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "费用性质编码（业务编号）", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer displayOrder;

    @Schema(description = "费用性质分类(可选值：自费、公费、省医保、市医保)", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private FeeCategoryEnum categoryEnum;

    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer activeFlag;
}
