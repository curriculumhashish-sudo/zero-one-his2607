package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.InpatientDispenseRecordMapper;
import com.zeroone.star.dispense.service.IInpatientDispenseRecordService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.time.format.ResolverStyle;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Objects;

@Service
public class InpatientDispenseRecordServiceImpl implements IInpatientDispenseRecordService {

    private static final int EXPORT_LIMIT = 10_000;
    private static final DateTimeFormatter DATE_TIME_FORMATTER =
            DateTimeFormatter.ofPattern("uuuu-MM-dd HH:mm:ss")
                    .withResolverStyle(ResolverStyle.STRICT);

    @Resource
    private InpatientDispenseRecordMapper mapper;

    @Override
    public PageDTO<InpatientDispenseRecordVO> queryPage(InpatientDispenseRecordQuery query) {
        normalizeAndValidate(query);
        long pageIndex = Math.max(query.getPageIndex(), 1);
        long pageSize = Math.max(query.getPageSize(), 1);
        Page<InpatientDispenseRecordDTO> page = new Page<>(pageIndex, pageSize);
        mapper.selectInpatientDispensePage(page, query);
        PageDTO<InpatientDispenseRecordVO> result = PageDTO.create(page, InpatientDispenseRecordVO::new);
        if (result.getRows() == null) {
            result.setRows(Collections.emptyList());
        }
        return result;
    }

    @Override
    public List<InpatientDispenseRecordDTO> listForExport(InpatientDispenseRecordQuery query) {
        normalizeAndValidate(query);
        List<InpatientDispenseRecordDTO> records = mapper.selectInpatientDispenseList(query, EXPORT_LIMIT + 1);
        if (records.size() > EXPORT_LIMIT) {
            throw new IllegalStateException("导出记录不能超过10000条，请缩小筛选范围");
        }
        if (records.isEmpty()) {
            throw new IllegalStateException("没有可导出的住院发退药记录");
        }
        return records;
    }

    private void normalizeAndValidate(InpatientDispenseRecordQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        query.setWardId(trimToNull(query.getWardId()));
        query.setPatientKeyword(trimToNull(query.getPatientKeyword()));
        query.setBedNo(trimToNull(query.getBedNo()));
        query.setDrugKeyword(trimToNull(query.getDrugKeyword()));
        query.setStatusEnum(toUpperCase(query.getStatusEnum()));
        query.setDispenseEnum(normalizeDispenseEnum(query.getDispenseEnum()));

        LocalDateTime startTime = parseTime(query.getStartTime(), false);
        LocalDateTime endTime = parseTime(query.getEndTime(), true);
        if (startTime != null && endTime != null && startTime.isAfter(endTime)) {
            throw new IllegalArgumentException("开始时间不能晚于结束时间");
        }
        query.setStartTime(formatTime(startTime));
        query.setEndTime(formatTime(endTime));
    }

    private String normalizeDispenseEnum(String value) {
        String normalized = toUpperCase(value);
        if ("DISPENSE".equals(normalized)) {
            return "INPATIENT";
        }
        if ("RETURN".equals(normalized)) {
            return "REFUND";
        }
        return normalized;
    }

    private LocalDateTime parseTime(String value, boolean endOfDay) {
        String normalized = trimToNull(value);
        if (normalized == null) {
            return null;
        }
        try {
            if (normalized.length() == 10) {
                LocalDate date = LocalDate.parse(normalized);
                return endOfDay ? date.atTime(23, 59, 59) : date.atStartOfDay();
            }
            return LocalDateTime.parse(normalized, DATE_TIME_FORMATTER);
        } catch (DateTimeParseException exception) {
            throw new IllegalArgumentException("时间格式应为yyyy-MM-dd或yyyy-MM-dd HH:mm:ss", exception);
        }
    }

    private String formatTime(LocalDateTime value) {
        return value == null ? null : value.format(DATE_TIME_FORMATTER);
    }

    private String toUpperCase(String value) {
        String normalized = trimToNull(value);
        return normalized == null ? null : normalized.toUpperCase(Locale.ROOT);
    }

    private String trimToNull(String value) {
        if (value == null) {
            return null;
        }
        String trimmed = value.strip();
        return trimmed.isEmpty() ? null : trimmed;
    }
}
