package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.util.List;

/**
 * <p>描述：新增删除单据对象</p>
 * <p>版权: ©copy;01星球</p>
 * <p>地址: 01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "删除单据")
public class  PharmacyInventoryDeleteDTO{

    @Schema(description = "盘点单据ID集合", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"CST2026-07-230003\",\"CST2026-07-230004\"]")
    private List<String> inventoryIdList;
}
