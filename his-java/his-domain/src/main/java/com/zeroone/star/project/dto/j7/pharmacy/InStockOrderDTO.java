package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：药房入库单数据传输对象
 * </p>
 * <p>
 * 功能说明：用于新增、编辑和提交审批入库单。包含主表信息及药品明细列表。
 * 整单总金额由服务端自动汇总明细行的合计金额，前端无需传递。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@Schema(description = "入库单DTO")
public class InStockOrderDTO implements Serializable {

    @Schema(description = "入库单号（业务号），新增时不传，服务端自动生成", example = "PSI2026-06-300001")
    @Size(max = 255, message = "入库单号长度不能超过255个字符")
    private String orderNo;

    @NotBlank(message = "源药库不能为空")
    @Size(max = 64, message = "源药库标识长度不能超过64个字符")
    @Schema(description = "源药库标识", example = "322954949549363200", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceLocationId;

    @NotBlank(message = "目的药房不能为空")
    @Size(max = 64, message = "目的药房标识长度不能超过64个字符")
    @Schema(description = "目的药房标识", example = "322955305339588608", requiredMode = Schema.RequiredMode.REQUIRED)
    private String warehouseId;

    @Schema(description = "项目类型（药品/物资）", example = "DRUG")
    @Size(max = 64, message = "项目类型长度不能超过64个字符")
    private String projectType;

    @Schema(description = "供应商ID", example = "322954949549363200")
    private Long supplierId;

    @Schema(description = "入库日期", example = "2026-07-30")
    private LocalDate inDate;

    @Schema(description = "主表备注", example = "常规入库")
    @Size(max = 255, message = "备注长度不能超过255个字符")
    private String remark;

    @NotEmpty(message = "入库单明细列表不能为空")
    @Valid
    @Schema(description = "入库单明细列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<InStockOrderDetailDTO> items;
}