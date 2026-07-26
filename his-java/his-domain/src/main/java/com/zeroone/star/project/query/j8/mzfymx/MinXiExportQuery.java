package com.zeroone.star.project.query.j8.mzfymx;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细导出查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细导出查询对象")
public class MinXiExportQuery {

    @NotBlank(message = "结算日期-开始日期不能为空")
    @Pattern(regexp = "^\\d{4}[-/](0?[1-9]|1[0-2])[-/](0?[1-9]|[12][0-9]|3[01])$", message = "开始日期格式不正确，支持 yyyy-MM-dd 或 yyyy/M/d")
    @Schema(description = "结算日期-开始日期", example = "2026-7-1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateFrom;

    @NotBlank(message = "结算日期-结束日期不能为空")
    @Pattern(regexp = "^\\d{4}[-/](0?[1-9]|1[0-2])[-/](0?[1-9]|[12][0-9]|3[01])$", message = "结束日期格式不正确，支持 yyyy-MM-dd 或 yyyy/M/d")
    @Schema(description = "结算日期-结束日期", example = "2026-7-31", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateTo;

    @Min(value = 1, message = "挂号科室ID必须为正整数")
    @Schema(description = "挂号科室主键", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer deptId;

    @Schema(description = "费用大类", example = "[\"药品\"]", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private List<String> feeType;

    @Schema(description = "细分类", example = "[\"西药\"]", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private List<String> feeTypeDetail;

    @Schema(description = "患者", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String patientKeyword;

    @Schema(description = "项目名称", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String projectName;

    @Min(value = 1, message = "院内编码必须为正整数")
    @Schema(description = "院内编码", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer internalCode;

    @Pattern(regexp = "^(ALL|CHARGE|REFUND)$", message = "收退方向只能是 ALL(全部) / CHARGE(收费) / REFUND(退费)")
    @Schema(description = "收退方向", example = "ALL", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String shouTui;

}