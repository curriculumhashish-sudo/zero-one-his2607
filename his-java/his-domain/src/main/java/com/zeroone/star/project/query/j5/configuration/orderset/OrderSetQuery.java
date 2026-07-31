package com.zeroone.star.project.query.j5.configuration.orderset;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 组套模板查询条件
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "组套模板查询条件")
public class OrderSetQuery extends PageQuery {

    @Schema(description = "模板名称", example = "普通感冒组套")
    private String name;

    @Schema(description = "模板编码", example = "OT-OUT-001")
    private String templateCode;

    @Schema(description = "绑定对象ID", example = "322663218269917184")
    private String objectId;

    @Schema(description = "绑定类型", example = "DEPARTMENT")
    private String bindingEnum;

    @Schema(description = "适用场景", example = "OUTPATIENT")
    private String sceneEnum;

    @Schema(description = "状态枚举", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "组套种类", example = "GENERAL")
    private String templateKindEnum;
}
