package com.zeroone.star.project.enums.j3.fee;

import com.baomidou.mybatisplus.annotation.EnumValue;
import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonValue;
import com.zeroone.star.project.enums.BaseEnum;
import lombok.Getter;
@Getter
public enum FeeCategoryEnum implements BaseEnum {

    SELF(1, "自费"),
    PUBLIC(2, "公费"),
    PROVINCIAL(3, "省医保"),
    MUNICIPAL(4, "市医保");

    @EnumValue
    @JsonValue
    private final int value;
    private final String desc;

    FeeCategoryEnum(int value, String desc) {
        this.value = value;
        this.desc = desc;
    }

    @Override
    public int getValue() {
        return value;
    }

    @Override
    public String getDesc() {
        return desc;
    }

    public static FeeCategoryEnum of(Integer value) {  // 内部工具方法，不加 @JsonCreator
        if (value == null) return null;
        for (FeeCategoryEnum item : values()) {
            if (item.value == value) {
                return item;
            }
        }
        return null;
    }

    // 唯一的反序列化入口，支持多种格式
    @JsonCreator
    public static FeeCategoryEnum fromString(String input) {
        if (input == null || input.isBlank()) {
            return null;
        }

        // 1. 尝试按数字字符串解析（支持 "1" 和 1）
        try {
            int intValue = Integer.parseInt(input);
            return of(intValue);
        } catch (NumberFormatException ignored) {}

        // 2. 按中文描述匹配
        for (FeeCategoryEnum item : values()) {
            if (item.desc.equals(input)) {
                return item;
            }
        }

        // 3. 按枚举名匹配（兜底）
        for (FeeCategoryEnum item : values()) {
            if (item.name().equals(input)) {
                return item;
            }
        }

        return null;
    }
}
