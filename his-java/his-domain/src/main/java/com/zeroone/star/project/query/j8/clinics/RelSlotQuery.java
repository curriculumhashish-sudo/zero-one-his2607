package com.zeroone.star.project.query.j8.clinics;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDate;


/**
 * <p>
 * 描述：获取已放号源查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "获取以放号源查询对象")
public class RelSlotQuery{
    @NotBlank(message = "机构ID不能为空")
    @Schema(description = "机构ID", example = "322663218269917184",requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @NotNull(message = "日期不能为空")
    @Schema(description = "号源日期", example = "2026-08-27",requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate date;

    @NotBlank(message = "排班ID不能为空")
    @Schema(description = "排班ID", example = "329700000000000001",requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleId;

}
