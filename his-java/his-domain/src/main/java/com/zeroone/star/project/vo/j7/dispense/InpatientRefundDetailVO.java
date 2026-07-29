package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：住院退药-患者退药明细显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药-患者退药明细显示对象")
public class InpatientRefundDetailVO {

    /**
     * 行主键（= 发药/退药发放记录ID，对齐前端 row-key）
     */
    @Schema(description = "明细行ID", example = "329906419476205568", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 发药/退药发放记录ID（确认退药勾选主键，与 id 同值）
     */
    @Schema(description = "发药记录ID", example = "329906419476205568", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispenseId;

    /**
     * 医嘱/申请ID
     */
    @Schema(description = "医嘱申请ID", example = "329891428408758272")
    private String requestId;

    /**
     * 药品ID
     */
    @Schema(description = "药品ID", example = "322997883812974592", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemId;

    /**
     * 药品名称
     */
    @Schema(description = "药品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemName;

    /**
     * 批号
     */
    @Schema(description = "批号", example = "20260501")
    private String lotNumber;

    /**
     * 应退数量
     */
    @Schema(description = "应退数量", example = "1.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal quantity;

    /**
     * 单位编码
     */
    @Schema(description = "单位编码", example = "8")
    private String unitCode;

    /**
     * 单位文本
     */
    @Schema(description = "单位文本", example = "瓶")
    private String unitText;

    /**
     * 开单医生
     */
    @Schema(description = "开单医生", example = "王医生")
    private String doctorName;

    /**
     * 退药状态码（PENDING_REFUND / REFUNDED / DECLINED；前置条件 dispense_enum=REFUND）
     */
    @Schema(description = "退药状态码", example = "PENDING_REFUND")
    private String refundEnum;

    /**
     * 退药状态文案
     */
    @Schema(description = "退药状态文案", example = "待退药")
    private String refundEnumText;

    /**
     * 追溯码
     */
    @Schema(description = "追溯码", example = "TRACE-001")
    private String traceNo;

    /**
     * 是否可勾选
     */
    @Schema(description = "是否可勾选", example = "true")
    private Boolean selectable;
}
