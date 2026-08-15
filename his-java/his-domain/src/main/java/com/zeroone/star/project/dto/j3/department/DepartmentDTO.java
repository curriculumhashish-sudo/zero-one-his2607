package com.zeroone.star.project.dto.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：科室传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Schema(description = "科室DTO")
public class DepartmentDTO extends DepartmentCreateDTO {
    @Schema(description = "主键ID", example = "322663105141149696", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Long id;

    @Schema(description = "业务编号", example = "OR0007.OR0008", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String busNo;

    @Schema(description = "状态显示文本", example = "已启用", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnumDictText;

    @Schema(description = "类型显示文本", example = "科室", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String typeEnumDictText;

    @Schema(description = "分类显示文本", example = "门诊", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classEnumDictText;

    @Schema(description = "医疗机构类型", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsType;

    @Schema(description = "医疗机构等级", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String medinsLv;

    @Schema(description = "组织单元ID", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String ouId;

    @Schema(description = "电子发票部门编码", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String eleInvoiceDeptCode;

    @Schema(description = "子节点列表", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private List<DepartmentDTO> children;
}
