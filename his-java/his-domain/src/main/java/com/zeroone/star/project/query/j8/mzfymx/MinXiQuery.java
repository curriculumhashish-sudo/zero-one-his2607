package com.zeroone.star.project.query.j8.mzfymx;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细查询对象")
public class MinXiQuery extends PageQuery {
    @NotBlank(message = "结算日期-开始日期不能为空")
    @Pattern(regexp = "^\\d{4}[-/](0?[1-9]|1[0-2])[-/](0?[1-9]|[12][0-9]|3[01])$", message = "开始日期格式不正确，支持 yyyy-MM-dd 或 yyyy/M/d")
    @Schema(description = "结算日期-开始日期",example = "2026-7-1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateFrom;

    @NotBlank(message = "结算日期-结束日期不能为空")
    @Pattern(regexp = "^\\d{4}[-/](0?[1-9]|1[0-2])[-/](0?[1-9]|[12][0-9]|3[01])$", message = "结束日期格式不正确，支持 yyyy-MM-dd 或 yyyy/M/d")
    @Schema(description = "结算日期-结束日期",example ="2026-7-31", requiredMode = Schema.RequiredMode.REQUIRED )
    private String dateTo;

    @Schema(description = "挂号科室主键（雪花ID）",example ="1", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private String registrationDepartmentId;

    @Schema(description = "费用大类编码列表",example ="[\"1001\"]", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private List<String> feeCategoryCodes;

    @Schema(description = "细分类编码列表",example ="[\"100101\"]", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private List<String> subCategoryCodes;

    @Schema(description = "患者",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String patientKeyword;

    @Schema(description = "项目名称关键字",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String productNameKeyword;

    @Schema(description = "院内编码关键字",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String productCodeKeyword;

    @Pattern(regexp = "^(ALL|CHARGE|REFUND)$", message = "收退方向只能是 ALL(全部) / CHARGE(收费) / REFUND(退费)")
    @Schema(description = "收退方向",example ="ALL",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tradeDirection;

    @Schema(description = "收费员主键（雪花ID，跨 whale_users + adm_practitioner 两张表）",example ="1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String cashierId;

    @Schema(description = "租户ID（后端注入，无需前端传）", hidden = true)
    private String tenantId;

}