package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
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

    @Schema(description = "明细主键ID（更新时传入，新增时不传）")
    private Long id;

    @Schema(description = "行号", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer seqNo;

    @Schema(description = "药品名称", requiredMode = Schema.RequiredMode.REQUIRED)
    private String drugName;

    @Schema(description = "药品规格")
    private String spec;

    @Schema(description = "生产厂家")
    private String manufacturer;

    @Schema(description = "入库数量", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer quantity;

    @Schema(description = "单价", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal unitPrice;

    @Schema(description = "行合计金额（后台自动计算，前端可忽略）")
    private BigDecimal totalAmount;

    @Schema(description = "药品批号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String batchNo;

    @Schema(description = "有效期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate expireDate;

    @Schema(description = "明细备注")
    private String itemRemark;
}