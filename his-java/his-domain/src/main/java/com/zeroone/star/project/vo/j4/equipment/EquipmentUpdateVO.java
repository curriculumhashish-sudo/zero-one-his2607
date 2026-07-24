package com.zeroone.star.project.vo.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "修改器材响应")
public class EquipmentUpdateVO {

    @Schema(description = "主键", example = "320010001000000003", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "器材名称", example = "一次性无菌注射器", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "器材业务编码（院内唯一）", example = "DEV-2026-0015926", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "器材分类名称（字典转义后）", example = "低值耗材")
    private String categoryName;

    @Schema(description = "器材类型名称（字典转义后）", example = "注射穿刺类")
    private String typeName;

    @Schema(description = "修改后的状态中文名", example = "已发布")
    private String statusName;

    @Schema(description = "更新时间", example = "2026-07-19 21:30:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String updateTime;
}
