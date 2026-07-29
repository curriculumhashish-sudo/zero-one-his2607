package com.zeroone.star.project.enums.j3.storehouse;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

import java.util.Arrays;
import java.util.List;

/**
 * 库房药房形态枚举
 * @author mie
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum FormEnum implements BaseEnum {
    WAREHOUSE(1, "WAREHOUSE", "仓库"),
    PHARMACY(2, "PHARMACY", "药房"),
    DEPT_PHARMACY(3, "DEPT_PHARMACY", "科室药房");

    private final Integer value;
    private final String desc;
    private final String dictText;

    /** 库房药房模块管理的 formEnum 值集合 */
    public static final List<String> STOREHOUSE_FORMS = Arrays.asList(
            WAREHOUSE.desc, PHARMACY.desc, DEPT_PHARMACY.desc
    );

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
        for (FormEnum e : values()) {
            if (e.desc.equals(desc)) return e.dictText;
        }
        return null;
    }

    public static String getDescByValue(Integer value) {
        if (value == null) return null;
        for (FormEnum e : values()) {
            if (e.value.equals(value)) return e.desc;
        }
        return null;
    }
}
