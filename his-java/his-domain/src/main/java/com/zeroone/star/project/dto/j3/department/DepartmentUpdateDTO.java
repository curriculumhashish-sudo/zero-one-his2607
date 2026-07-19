package com.zeroone.star.project.dto.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：修改科室传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 很帅大王
 * @version 1.0.0
 */
@Data
@Schema(description = "修改科室DTO")
public class DepartmentUpdateDTO {

    @Schema(description = "名称", example = "门诊内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "类型", example = "DEPARTMENT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "类别", example = "CLINIC", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classEnum;

    @Schema(description = "上级组织", example = "322663105141149696", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String parentId;

    @Schema(
            description = "上级组织是否发生变更，true且parentId为空表示调整为根节点",
            example = "true",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED
    )
    private Boolean parentIdChanged;

    @Schema(description = "医保编码", example = "HIS001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybNo;

    @Schema(description = "医保名称", example = "门诊内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ybName;

    @Schema(description = "机构标识", example = "H12345678", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsId;

    @Schema(description = "医保区划", example = "440100", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsAdmdvs;

    @Schema(description = "医疗机构类型", example = "HOSPITAL", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsType;

    @Schema(description = "医疗机构等级", example = "LEVEL_3_A", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsLv;

    @Schema(description = "拼音", example = "MZNK", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String pyStr;

    @Schema(description = "五笔", example = "UYMT", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String wbStr;

    @Schema(description = "默认医师ID", example = "322663105141149700", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defDoctorId;

    @Schema(description = "科别", example = "01", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String caty;

    @Schema(description = "显示顺序", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer displayOrder;
}
