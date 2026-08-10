package com.zeroone.star.project.query.j7.dispense;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.Pattern;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.time.format.ResolverStyle;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "住院发退药记录查询条件")
public class InpatientDispenseRecordQuery extends PageQuery {

    public InpatientDispenseRecordQuery() {
        setPageIndex(1);
        setPageSize(20);
    }

    @Schema(description = "病区ID", example = "322955305339588608")
    private String wardId;

    @Schema(description = "患者姓名或住院号", example = "周女士")
    private String patientKeyword;

    @Schema(description = "床号", example = "1号床")
    private String bedNo;

    @Schema(description = "发退药状态枚举值", example = "COMPLETED")
    private String statusEnum;

    @Schema(description = "数据库发退药类型：INPATIENT发药、REFUND退药；兼容DISPENSE、RETURN", example = "INPATIENT")
    private String dispenseEnum;

    @Schema(description = "发退药开始时间，支持日期或日期时间", example = "2026-07-01 00:00:00")
    @Pattern(regexp = "^$|^\\d{4}-\\d{2}-\\d{2}( \\d{2}:\\d{2}:\\d{2})?$", message = "开始时间格式应为yyyy-MM-dd或yyyy-MM-dd HH:mm:ss")
    private String startTime;

    @Schema(description = "发退药结束时间，支持日期或日期时间", example = "2026-07-31 23:59:59")
    @Pattern(regexp = "^$|^\\d{4}-\\d{2}-\\d{2}( \\d{2}:\\d{2}:\\d{2})?$", message = "结束时间格式应为yyyy-MM-dd或yyyy-MM-dd HH:mm:ss")
    private String endTime;

    @Schema(description = "药品名称、拼音码或五笔码", example = "当归")
    private String drugKeyword;

    @JsonIgnore
    @AssertTrue(message = "开始或结束时间不是有效日期")
    @Schema(hidden = true)
    public boolean isTimeValueValid() {
        try {
            if (startTime != null && !startTime.isBlank()) {
                parseTime(startTime, false);
            }
            if (endTime != null && !endTime.isBlank()) {
                parseTime(endTime, true);
            }
            return true;
        } catch (DateTimeParseException exception) {
            return false;
        }
    }

    @JsonIgnore
    @AssertTrue(message = "开始时间不能晚于结束时间")
    @Schema(hidden = true)
    public boolean isTimeRangeValid() {
        if (startTime == null || startTime.isBlank() || endTime == null || endTime.isBlank()) {
            return true;
        }
        try {
            return !parseTime(startTime, false).isAfter(parseTime(endTime, true));
        } catch (DateTimeParseException exception) {
            return true;
        }
    }

    private LocalDateTime parseTime(String value, boolean endOfDay) {
        String normalized = value.strip();
        if (normalized.length() == 10) {
            LocalDate date = LocalDate.parse(normalized);
            return endOfDay ? date.atTime(23, 59, 59) : date.atStartOfDay();
        }
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("uuuu-MM-dd HH:mm:ss")
                .withResolverStyle(ResolverStyle.STRICT);
        return LocalDateTime.parse(normalized, formatter);
    }
}
