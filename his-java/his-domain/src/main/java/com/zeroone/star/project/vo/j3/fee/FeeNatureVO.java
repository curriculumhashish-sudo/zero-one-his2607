package com.zeroone.star.project.vo.j3.fee;

import com.zeroone.star.project.enums.j3.FeeCategoryEnum;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
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

    @Schema(description = "费用性质ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "费用性质名称", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "费用性质编码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @Schema(description = "是否启用 1 启用 0 禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isEnabled;

    @Schema(description = "费用性质显示顺序", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer sortOrder;

    @Schema(description = "费用性质分类", example = "自费", requiredMode = Schema.RequiredMode.REQUIRED)
    private FeeCategoryEnum category;

//    // 不能确定前端传输的是数字还是字符串
//    @Schema(description = "费用性质分类", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
//    private Integer category;
}
