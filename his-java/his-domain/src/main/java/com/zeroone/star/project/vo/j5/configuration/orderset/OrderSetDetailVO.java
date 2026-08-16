package com.zeroone.star.project.vo.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 组套模板详情VO
 */
@Data
@Schema(description = "组套模板详情VO")
public class OrderSetDetailVO {

    @Schema(description = "组套模板ID", example = "329888224518344704")
    private String id;

    @Schema(description = "绑定对象ID", example = "322663218269917184")
    private String objectId;

    @Schema(description = "绑定类型", example = "DEPARTMENT")
    private String bindingEnum;

    @Schema(description = "适用场景", example = "OUTPATIENT")
    private String sceneEnum;

    @Schema(description = "模板编码", example = "OT-OUT-001")
    private String templateCode;

    @Schema(description = "模板名称", example = "普通感冒组套")
    private String name;

    @Schema(description = "模板描述", example = "门诊普通感冒常用医嘱")
    private String description;

    @Schema(description = "排序权重", example = "0")
    private Integer sortWeight;

    @Schema(description = "状态枚举", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "组套种类", example = "GENERAL")
    private String templateKindEnum;

    @Schema(description = "TCM组套默认中药付数", example = "2")
    private BigDecimal defaultChineseHerbsDoseQuantity;

    @Schema(description = "TCM组套默认代煎标志", example = "false")
    private Boolean defaultSufferingFlag;

    @Schema(description = "模板来源", example = "MANUAL")
    private String templateSourceEnum;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人")
    private String createdBy;

    @Schema(description = "更新人")
    private String updatedBy;

    @Schema(description = "版本号（编辑时必须回传，用于乐观锁）", example = "0")
    private Integer version;

    @Schema(description = "明细列表")
    private List<OrderSetLineVO> lines;

    /**
     * 组套明细VO
     */
    @Data
    @Schema(description = "组套明细VO")
    public static class OrderSetLineVO {

        @Schema(description = "明细ID", example = "329888224518344705")
        private String id;

        @Schema(description = "组套模板ID", example = "329888224518344704")
        private String orderTemplateId;

        @Schema(description = "明细类型", example = "MEDICATION")
        private String lineKindEnum;

        @Schema(description = "药品定义ID")
        private String medicationDefinitionId;

        @Schema(description = "活动定义ID")
        private String activityDefinitionId;

        @Schema(description = "设备定义ID")
        private String deviceDefinitionId;

        @Schema(description = "分组Key")
        private String groupKey;

        @Schema(description = "排序号")
        private Integer sortOrder;

        @Schema(description = "默认数量")
        private BigDecimal defaultQuantity;

        @Schema(description = "默认单位")
        private String defaultUnitCode;

        @Schema(description = "默认天数")
        private Integer defaultDays;

        @Schema(description = "频次ID")
        private String frequencyId;

        @Schema(description = "给药途径")
        private String routeCode;

        @Schema(description = "剂量文本")
        private String dosageText;

        @Schema(description = "默认执行机构ID")
        private String defaultPerformOrgId;

        @Schema(description = "明细备注")
        private String lineRemark;
    }
}
