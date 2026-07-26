package com.zeroone.star.project.dto.j3.patient;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;

import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;

/**
 * 兼容患者原型日期控件和标准日期请求的出生日期解析器。
 */
public class FlexibleLocalDateDeserializer
        extends JsonDeserializer<LocalDate> {

    private static final DateTimeFormatter DATE_TIME_FORMATTER =
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    @Override
    public LocalDate deserialize(
            JsonParser parser,
            DeserializationContext context
    ) throws IOException {
        String value = parser.getValueAsString();
        if (value == null || value.trim().isEmpty()) {
            return null;
        }

        String normalized = value.trim();
        try {
            return LocalDate.parse(normalized);
        } catch (DateTimeParseException ignored) {
            try {
                return LocalDateTime.parse(
                        normalized,
                        DATE_TIME_FORMATTER
                ).toLocalDate();
            } catch (DateTimeParseException exception) {
                return (LocalDate) context.handleWeirdStringValue(
                        LocalDate.class,
                        normalized,
                        "出生日期格式应为yyyy-MM-dd或yyyy-MM-dd HH:mm:ss"
                );
            }
        }
    }
}
