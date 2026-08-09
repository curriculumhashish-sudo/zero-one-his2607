package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "门诊发退药记录查询参数")
public class DispenseQuery extends PageQuery {

    @Schema(description = "发药开始日期 (yyyy-MM-dd)", example = "2026-07-01")
    private String startDate;

    @Schema(description = "发药结束日期 (yyyy-MM-dd)", example = "2026-07-31")
    private String endDate;

    @Schema(description = "记录类型: dispense-发药, return-退药", example = "dispense")
    private String recordType;

    @Schema(description = "药房ID", example = "LOC001")
    private String locationId;

    @Schema(description = "药品关键字 (名称/编码/拼音)", example = "阿托伐他汀")
    private String drugKeyword;

    @Schema(description = "患者关键字 (姓名/拼音/五笔)", example = "马化腾")
    private String patientKeyword;

    @Schema(description = "发药人关键字 (姓名/拼音/五笔)", example = "李药师")
    private String practitionerKeyword;

    @Schema(description = "项目类型", example = "西药")
    private String projectType;
}