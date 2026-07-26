package com.zeroone.star.project.dto.j5.configuration.orderset;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.PositiveOrZero;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

/**
 * 新增组套模板传输对象。
 */
@Data
@Schema(description = "新增组套模板")
public class OrderSetAddDTO {

    @NotBlank(message = "绑定对象编号不能为空")
    @Schema(description = "绑定对象编号，根据绑定类型表示个人、科室、医院或协定处方归属对象", example = "322663218269917184", requiredMode = Schema.RequiredMode.REQUIRED)
    private String objectId;

    @NotBlank(message = "绑定类型不能为空")
    @Size(max = 255, message = "绑定类型长度不能超过255")
    @Schema(description = "绑定类型；现有数据包含PERSONAL、DEPARTMENT、HOSPITAL、AGREEMENT", example = "DEPARTMENT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String bindingEnum;

    @NotBlank(message = "适用场景不能为空")
    @Size(max = 255, message = "适用场景长度不能超过255")
    @Schema(description = "适用场景", example = "OUTPATIENT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sceneEnum;

    @Size(max = 255, message = "模板编码长度不能超过255")
    @Schema(description = "模板编码", example = "OT-OUT-001")
    private String templateCode;

    @NotBlank(message = "模板名称不能为空")
    @Size(max = 255, message = "模板名称长度不能超过255")
    @Schema(description = "模板名称", example = "普通感冒组套", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Size(max = 255, message = "模板描述长度不能超过255")
    @Schema(description = "模板描述", example = "门诊普通感冒常用医嘱")
    private String description;

    @PositiveOrZero(message = "排序权重不能小于0")
    @Schema(description = "排序权重", example = "0", defaultValue = "0")
    private Integer sortWeight = 0;

    @Size(max = 255, message = "备注长度不能超过255")
    @Schema(description = "备注", example = "仅供门诊使用")
    private String remark;

    @NotBlank(message = "组套种类不能为空")
    @Pattern(regexp = "GENERAL|TCM|PRICING", message = "组套种类只能是GENERAL、TCM或PRICING")
    @Schema(description = "组套种类", example = "GENERAL", defaultValue = "GENERAL", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateKindEnum = "GENERAL";

    @DecimalMin(value = "0", inclusive = false, message = "默认中药付数必须大于0")
    @Schema(description = "TCM组套默认中药付数", example = "2")
    private BigDecimal defaultChineseHerbsDoseQuantity;

    @Schema(description = "TCM组套默认代煎标志", example = "false", defaultValue = "false")
    private Boolean defaultSufferingFlag = false;

    @Valid
    @NotNull(message = "组套明细集合不能为空")
    @Schema(description = "组套明细；允许为空，为空时新增结果保留为DRAFT且不会发布")
    private List<OrderSetLineDTO> lines = new ArrayList<>();
}
