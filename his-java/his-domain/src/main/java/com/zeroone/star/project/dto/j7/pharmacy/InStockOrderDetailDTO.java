package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：入库单明细数据传输对象
 * </p>
 * <p>
 * 功能说明：封装一条药品入库明细信息，包含行号、药品标识、药品名称、规格、厂家、数量、单价、
 * 有效期、批号、追溯码（跟踪号）等。行合计金额由服务端自动计算，前端无需传递。
 * 字段命名与实体类 {@code InStockOrderDetail} 严格对应。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@Schema(description = "入库单明细DTO")
public class InStockOrderDetailDTO implements Serializable {

    @Schema(description = "明细行ID（保存时传入，新增时不传）", example = "1001")
    private Long id;

    @NotNull(message = "行号不能为空")
    @Schema(description = "行号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer seqNo;

    @Schema(description = "药品/物资项标识（可选）", example = "ITEM001")
    @Size(max = 64, message = "项目标识长度不能超过64个字符")
    private String itemId;

    @NotBlank(message = "药品名称不能为空")
    @Size(max = 120, message = "药品名称长度不能超过120个字符")
    @Schema(description = "药品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String drugName;

    @Size(max = 64, message = "规格长度不能超过64个字符")
    @Schema(description = "药品规格", example = "0.25g*20粒")
    private String spec;

    @Size(max = 120, message = "生产厂家长度不能超过120个字符")
    @Schema(description = "生产厂家", example = "XX制药")
    private String manufacturer;

    @NotNull(message = "数量不能为空")
    @Positive(message = "数量必须大于0")
    @Schema(description = "入库数量", example = "100", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer quantity;

    @NotNull(message = "单价不能为空")
    @DecimalMin(value = "0.01", message = "单价必须大于0")
    @Schema(description = "单价", example = "5.20", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal unitPrice;

    @Schema(description = "行合计金额（后台自动计算）", example = "520.00")
    private BigDecimal totalAmount;

    @NotBlank(message = "批号不能为空")
    @Size(max = 32, message = "批号长度不能超过32个字符")
    @Schema(description = "药品批号", example = "20250101", requiredMode = Schema.RequiredMode.REQUIRED)
    private String batchNo;

    @NotNull(message = "有效期不能为空")
    @Schema(description = "有效期", example = "2027-01-01", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate expireDate;

    @Size(max = 64, message = "追溯码长度不能超过64个字符")
    @Schema(description = "追溯码（跟踪号）", example = "TRC-123456789")
    private String traceNo;

    @Size(max = 300, message = "明细备注长度不能超过300个字符")
    @Schema(description = "明细备注")
    private String itemRemark;
}