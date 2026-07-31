package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * 患者耗材项目VO
 */
@Data
@Schema(description = "患者耗材项目VO")
public class PatientConsumableItemVO {

    @Schema(description = "记录ID", example = "1816800000000000001")
    private String id;

    @Schema(description = "业务编号", example = "BIZ20250101001")
    private String bus_no;

    @Schema(description = "项目ID", example = "ITEM001")
    private String item_id;

    @Schema(description = "项目名称", example = "一次性注射器")
    private String item_name;

    @Schema(description = "项目编码", example = "SYRINGE001")
    private String item_code;

    @Schema(description = "耗材类型", example = "INJECTION")
    private String consumable_type;

    @Schema(description = "耗材类型名称", example = "注射类")
    private String consumable_type_name;

    @Schema(description = "数量", example = "2")
    private BigDecimal quantity;

    @Schema(description = "单位代码", example = "AMP")
    private String unit_code;

    @Schema(description = "单位名称", example = "安瓿")
    private String unit_name;

    @Schema(description = "单价", example = "0.50")
    private BigDecimal unit_price;

    @Schema(description = "总价", example = "1.00")
    private BigDecimal total_price;

    @Schema(description = "库存数量", example = "100")
    private BigDecimal stock_quantity;

    @Schema(description = "库存单位", example = "BOX")
    private String stock_unit;

    @Schema(description = "生产厂家", example = "XX制药")
    private String manufacturer;

    @Schema(description = "批号", example = "LOT20250101")
    private String batch_no;

    @Schema(description = "有效期")
    private String expiry_date;

    @Schema(description = "执行科室ID", example = "1816800000000000001")
    private String exec_dept_id;

    @Schema(description = "执行科室名称", example = "门诊注射室")
    private String exec_dept_name;

    @Schema(description = "状态", example = "PENDING")
    private String status;

    @Schema(description = "状态名称", example = "待执行")
    private String status_name;

    @Schema(description = "执行状态", example = "0")
    private Integer execution_status;

    @Schema(description = "备注")
    private String remark;
}
