package com.zeroone.star.project.vo.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 组套模板列表VO
 */
@Data
@Schema(description = "组套模板列表VO")
public class OrderSetListVO {

    @Schema(description = "组套模板ID", example = "329888224518344704")
    private String id;

    @Schema(description = "模板编码", example = "OT-OUT-001")
    private String templateCode;

    @Schema(description = "模板名称", example = "普通感冒组套")
    private String name;

    @Schema(description = "模板描述")
    private String description;

    @Schema(description = "绑定类型", example = "DEPARTMENT")
    private String bindingEnum;

    @Schema(description = "适用场景", example = "OUTPATIENT")
    private String sceneEnum;

    @Schema(description = "状态枚举", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "状态名称", example = "启用")
    private String statusName;

    @Schema(description = "组套种类", example = "GENERAL")
    private String templateKindEnum;

    @Schema(description = "排序权重")
    private Integer sortWeight;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "明细数量")
    private Integer lineCount;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;
}
