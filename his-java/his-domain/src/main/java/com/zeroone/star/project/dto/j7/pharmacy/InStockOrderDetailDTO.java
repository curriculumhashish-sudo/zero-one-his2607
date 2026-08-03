package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
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
 * 功能说明：封装一条药品入库明细信息，用于前后端数据交互。
 * 行合计金额（totalAmount）由服务端根据单价和数量自动计算，前端无需传递。
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

    @Schema(description = "明细主键ID（更新时传入，新增时不传）", example = "1")
    private Long id;

    @NotNull(message = "行号不能为空")
    @Schema(description = "行号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer seqNo;

    @NotBlank(message = "药品ID不能为空")
    @Size(max = 64, message = "药品ID长度不能超过64个字符")
    @Schema(description = "药品/物资ID", example = "MED_001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    @NotBlank(message = "药品名称不能为空")
    @Schema(description = "药品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String drugName;

    @Schema(description = "药品规格", example = "0.5g*24粒/盒")
    private String spec;

    @Schema(description = "生产厂家", example = "哈药集团")
    private String manufacturer;

    @NotNull(message = "入库数量不能为空")
    @Min(value = 1, message = "入库数量必须大于0")
    @Schema(description = "入库数量", example = "100", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer quantity;

    @NotNull(message = "单价不能为空")
    @Min(value = 0, message = "单价不能为负数")
    @Schema(description = "单价", example = "15.50", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal unitPrice;

    @Schema(description = "行合计金额（后台自动计算，前端可忽略）", example = "1550.00")
    private BigDecimal totalAmount;

    @NotBlank(message = "药品批号不能为空")
    @Schema(description = "药品批号", example = "B20260701", requiredMode = Schema.RequiredMode.REQUIRED)
    private String batchNo;

    @Size(max = 255, message = "追溯码长度不能超过255个字符")
    @Schema(description = "追溯码", example = "TRACE-001")
    private String traceNo;

    @Schema(description = "有效期", example = "2028-07-01")
    private LocalDate expireDate;

    @Schema(description = "明细备注", example = "需冷藏保存")
    private String itemRemark;
}