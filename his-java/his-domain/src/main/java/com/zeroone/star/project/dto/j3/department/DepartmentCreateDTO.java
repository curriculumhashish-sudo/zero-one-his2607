package com.zeroone.star.project.dto.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：新增科室传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Schema(description = "新增科室DTO")
public class DepartmentCreateDTO {
    @Schema(description = "名称", example = "门诊内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "类别", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classEnum;

    @Schema(description = "上级组织", example = "322663105141149696", requiredMode = Schema.RequiredMode.REQUIRED)
    private String parentId;

    @Schema(description = "医保编码", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    @Schema(description = "医保名称", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybName;

    @Schema(description = "机构标识", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsId;

    @Schema(description = "医保区划", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsAdmdvs;

    @Schema(description = "拼音", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    @Schema(description = "五笔", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    @Schema(description = "默认医师", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defDoctorId;

    @Schema(description = "科别", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String caty;

    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer displayOrder;
}