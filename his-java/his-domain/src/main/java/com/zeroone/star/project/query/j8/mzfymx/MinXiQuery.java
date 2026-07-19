package com.zeroone.star.project.query.j8.mzfymx;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
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
    @Schema(description = "结算日期-开始日期",example = "2026-7-1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateFrom;

    @Schema(description = "结算日期-结束日期",example ="2026-7-31", requiredMode = Schema.RequiredMode.REQUIRED )
    private String dateTo;

    @Schema(description = "挂号科室主键",example ="1", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private Integer deptId;

    @Schema(description = "费用大类",example ="药品", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private List<String> feeType;

    @Schema(description = "细分类",example ="西药", requiredMode = Schema.RequiredMode.NOT_REQUIRED )
    private List<String> feeTypeDetail;

    @Schema(description = "患者",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String patientKeyword;

    @Schema(description = "项目名称",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String projectName;

    @Schema(description = "院内编码",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer internalCode;

    @Schema(description = "收退方向",example ="ALL",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String shouTui;

}
