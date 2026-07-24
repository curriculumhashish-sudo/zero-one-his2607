package com.zeroone.star.project.enums.j3.department;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter
@AllArgsConstructor
public enum TypeEnum implements BaseEnum {
    HOSPITAL(1, "HOSPITAL", "医院"),
    DEPARTMENT(2, "DEPARTMENT", "科室");

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
        for (TypeEnum e : values()) {
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
        for (TypeEnum e : values()) {
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
        for (TypeEnum e : values()) {
            if (e.value.equals(value)) {
                return e.desc;
            }
        }
        return null;
    }
}
