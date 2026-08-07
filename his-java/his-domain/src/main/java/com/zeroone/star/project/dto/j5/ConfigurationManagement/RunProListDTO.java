package com.zeroone.star.project.dto.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：执行项目列表DTO（映射adm_healthcare_service表）
 * </p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "执行项目列表DTO")
public class RunProListDTO {

    @Schema(description = "项目ID")
    private String id;

    @Schema(description = "项目名称")
    private String name;

    @Schema(description = "类型编码")
    private String typeCode;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "执行机构ID")
    private String offeredOrgId;

    @Schema(description = "专科编码")
    private String specialtyCode;

    @Schema(description = "执行地点ID")
    private String locationId;

    @Schema(description = "状态枚举")
    private String statusEnum;

    @Schema(description = "启用标志")
    private Integer activeFlag;

    @Schema(description = "医保编号")
    private String ybNo;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "备注")
    private String comment;
}
