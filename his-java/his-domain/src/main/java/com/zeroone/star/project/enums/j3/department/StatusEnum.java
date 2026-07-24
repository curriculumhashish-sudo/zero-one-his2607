package com.zeroone.star.project.enums.j3.department;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter
@AllArgsConstructor
public enum StatusEnum implements BaseEnum {
    ACTIVE(1, "ACTIVE", "已启用"),
    INACTIVE(2, "INACTIVE", "已停用");

    private final Integer value;
    private final String desc;
    private final String dictText;

    @Override
    public int getValue() {
        return this.value;
    }

    @Override
    public String getDesc() {
        return this.desc;
    }

    public static Integer getValueByDesc(String desc) {
        if (desc == null) {
            return null;
        }
        for (StatusEnum e : values()) {
            if (e.desc.equals(desc)) {
                return e.value;
            }
        }
        return null;
    }

    public static String getDictTextByDesc(String desc) {
        if (desc == null) {
            return null;
        }
        for (StatusEnum e : values()) {
            if (e.desc.equals(desc)) {
                return e.dictText;
            }
        }
        return null;
    }

    public static String getDescByValue(Integer value) {
        if (value == null) {
            return null;
        }
        for (StatusEnum e : values()) {
            if (e.value.equals(value)) {
                return e.desc;
            }
        }
        return null;
    }
}
