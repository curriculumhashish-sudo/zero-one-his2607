package com.zeroone.star.project.vo.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：供应商列表分页VO（列表展示精简关键字段，完整数据走详情接口）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商列表分页视图对象")
public class SupplierListVO implements Serializable {

    @Schema(description = "主键", example = "1234567890123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "业务编号", example = "SUP20250001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型枚举值", example = "ENTERPRISE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "统一社会信用代码", example = "91110000100000XXXX", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creditCode;

    @Schema(description = "法定代表人", example = "张三")
    private String legalRepresentative;

    @Schema(description = "联系电话", example = "010-88888888")
    private String phone;

    @Schema(description = "状态枚举值（ENABLED-启用/DISABLED-停用）", example = "ENABLED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "最后更新时间", example = "2025-01-10 15:30:00")
    private String updatedAt;
}
