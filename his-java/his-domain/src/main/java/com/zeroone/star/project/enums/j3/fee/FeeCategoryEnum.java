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

    // 按数字反序列化
    @JsonCreator(mode = JsonCreator.Mode.DELEGATING)
    public static FeeCategoryEnum of(Integer value) {
        if (value == null) {
            return null;
        }
        for (FeeCategoryEnum item : values()) {
            if (item.value == value) {
                return item;
            }
        }
        return null;
    }

    // 按描述反序列化（可选，支持前端传中文）
    @JsonCreator
    public static FeeCategoryEnum of(String desc) {
        if (desc == null || desc.isEmpty()) {
            return null;
        }
        for (FeeCategoryEnum item : values()) {
            if (item.desc.equals(desc)) {
                return item;
            }
        }
        return null;
    }
}
