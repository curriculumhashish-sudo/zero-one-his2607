package com.zeroone.star.project.vo.j8.statdata;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "小时访问趋势视图")
public class HourTrendVO {

    @Schema(description = "数据点列表")
    private List<HourTrendDTO> points;

    @Schema(description = "单位", example = "次")
    private String unit;
}