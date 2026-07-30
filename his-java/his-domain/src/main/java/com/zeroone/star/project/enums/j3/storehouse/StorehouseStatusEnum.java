package com.zeroone.star.project.enums.j3.storehouse;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * 库房药房启用状态枚举
 * @author mie
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum StorehouseStatusEnum implements BaseEnum {
    ACTIVE(1, "ACTIVE", "启用"),
    INACTIVE(0, "INACTIVE", "停用");

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

    public static String getDictTextByDesc(String desc) {
        if (desc == null) return null;
        for (StorehouseStatusEnum e : values()) {
            if (e.desc.equals(desc)) return e.dictText;
        }
        return null;
    }

    public static String getDescByValue(Integer value) {
        if (value == null) return null;
        for (StorehouseStatusEnum e : values()) {
            if (e.value.equals(value)) return e.desc;
        }
        return null;
    }
}
