package com.zeroone.star.configuration.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

@Data
public class OrderTemplateDetailVO {

    @Schema(description = "组套模板 ID")
    private String id;

    @Schema(description = "模板名称")
    private String name;

    @Schema(description = "模板编码")
    private String templateCode;

    @Schema(description = "绑定类型")
    private String bindingEnum;

    @Schema(description = "适用场景")
    private String sceneEnum;

    @Schema(description = "组套种类")
    private String templateKindEnum;

    @Schema(description = "状态")
    private String statusEnum;

    @Schema(description = "模板来源")
    private String templateSourceEnum;

    @Schema(description = "描述")
    private String description;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "排序权重")
    private Integer sortWeight;

    @Schema(description = "TCM 默认付数")
    private BigDecimal defaultChineseHerbsDoseQuantity;

    @Schema(description = "TCM 默认代煎标志")
    private Boolean defaultSufferingFlag;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "组套明细行列表")
    private List<OrderTemplateLineVO> items;
}
