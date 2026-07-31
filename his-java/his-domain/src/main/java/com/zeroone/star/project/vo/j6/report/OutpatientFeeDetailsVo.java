package com.zeroone.star.project.vo.j6.report;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

@Schema(description = "门诊费用列表数据")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class OutpatientFeeDetailsVo extends PageDTO<FeeDetailVO> {
    @Schema(description = "列表门诊费用汇总")
    FeeSummaryVo summary;
}


