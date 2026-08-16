package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>描述：新增单据对象</p>
 * <p>版权: ©copy;01星球</p>
 * <p>地址: 01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "删除单据")
public class PharmacyInventoryDeleteVO {


    @Schema(description = "单据删除结果", requiredMode = Schema.RequiredMode.REQUIRED, example = "true")
    @NotBlank(message = "单据删除结果不能为空")
    private Boolean success;
}
