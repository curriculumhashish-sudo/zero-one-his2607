package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

@Data
@Schema(description = "科室信息VO")
public class DepartmentVO {

    @Schema(description = "主键id")
    private String id;

    @Schema(description = "科室编码")
    private String code;

    @Schema(description = "科室名称")
    private String displayName;

    @Schema(description = "父节点id")
    private String parentId;

    @Schema(description = "排序号")
    private Integer sortOrder;

    @Schema(description = "是否启用 0禁用 1启用")
    private Integer isEnabled;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "子科室列表（树形结构使用）")
    private List<DepartmentVO> children;
}

