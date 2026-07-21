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
 * 修改组套模板传输对象。
 */
@Data
@Schema(description = "修改组套模板")
public class OrderSetModifyDTO {

    @NotBlank(message = "组套模板编号不能为空")
    @Schema(description = "组套模板编号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotNull(message = "版本号不能为空")
    @PositiveOrZero(message = "版本号不能小于0")
    @Schema(description = "聚合乐观锁版本号", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer version;

    @NotBlank(message = "绑定对象编号不能为空")
    @Schema(description = "绑定对象编号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String objectId;

    @NotBlank(message = "绑定类型不能为空")
    @Size(max = 255, message = "绑定类型长度不能超过255")
    @Schema(description = "绑定类型", requiredMode = Schema.RequiredMode.REQUIRED)
    private String bindingEnum;

    @NotBlank(message = "适用场景不能为空")
    @Size(max = 255, message = "适用场景长度不能超过255")
    @Schema(description = "适用场景", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sceneEnum;

    @Size(max = 255, message = "模板编码长度不能超过255")
    private String templateCode;

    @NotBlank(message = "模板名称不能为空")
    @Size(max = 255, message = "模板名称长度不能超过255")
    private String name;

    @Size(max = 255, message = "模板描述长度不能超过255")
    private String description;

    @PositiveOrZero(message = "排序权重不能小于0")
    private Integer sortWeight;

    @Size(max = 255, message = "备注长度不能超过255")
    private String remark;

    @NotBlank(message = "组套种类不能为空")
    @Pattern(regexp = "GENERAL|TCM|PRICING", message = "组套种类只能是GENERAL、TCM或PRICING")
    private String templateKindEnum;

    @DecimalMin(value = "0", inclusive = false, message = "默认中药付数必须大于0")
    private BigDecimal defaultChineseHerbsDoseQuantity;

    private Boolean defaultSufferingFlag;

    @Valid
    @NotNull(message = "组套明细集合不能为空")
    private List<OrderSetLineDTO> lines = new ArrayList<>();
}
