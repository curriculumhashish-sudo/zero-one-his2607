package com.zeroone.star.j3.ward.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.experimental.Accessors;

/**
 * <p>
 * 描述：管理域机构实体（科室/机构）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Accessors
@Builder
@TableName("adm_organization")
@Schema(description = "管理域机构实体", name = "AdmOrganization")
public class AdmOrganization {

    @Schema(description = "主键ID", example = "322663218269917184", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "租户标识", example = "1")
    private Long tenantId;

    @Schema(description = "机构/科室名称", example = "住院内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "业务编号", example = "OR0007.OR0008", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;
}