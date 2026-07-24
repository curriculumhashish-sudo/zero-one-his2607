package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "住院发退药记录查询条件")
public class InpatientDispenseRecordQuery extends PageQuery {

    public InpatientDispenseRecordQuery() {
        setPageIndex(1);
        setPageSize(20);
    }

    @Schema(description = "病区ID", example = "WARD-001")
    private String wardId;

    @Schema(description = "患者姓名或住院号", example = "张三")
    private String patientKeyword;

    @Schema(description = "床号", example = "08床")
    private String bedNo;

    @Schema(description = "发退药状态枚举值", example = "COMPLETED")
    private String statusEnum;

    @Schema(description = "记录类型枚举值：DISPENSE发药、RETURN退药", example = "DISPENSE")
    private String dispenseEnum;

    @Schema(description = "发退药开始时间", example = "2026-07-01 00:00:00")
    @Pattern(regexp = "^$|^\\d{4}-\\d{2}-\\d{2}( \\d{2}:\\d{2}:\\d{2})?$", message = "开始时间格式应为yyyy-MM-dd HH:mm:ss")
    private String startTime;

    @Schema(description = "发退药结束时间", example = "2026-07-31 23:59:59")
    @Pattern(regexp = "^$|^\\d{4}-\\d{2}-\\d{2}( \\d{2}:\\d{2}:\\d{2})?$", message = "结束时间格式应为yyyy-MM-dd HH:mm:ss")
    private String endTime;

    @Schema(description = "药品名称、拼音码或五笔码", example = "阿托伐他汀")
    private String drugKeyword;
}
