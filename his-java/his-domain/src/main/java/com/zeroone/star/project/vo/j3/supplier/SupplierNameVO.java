package com.zeroone.star.project.vo.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：供应商名称下拉VO（下拉选择组件专用，仅返回 id + name）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "供应商名称下拉视图对象")
public class SupplierNameVO implements Serializable {

    @Schema(description = "供应商主键ID", example = "1234567890123456789", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "供应商名称", example = "华润医药集团有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
}
