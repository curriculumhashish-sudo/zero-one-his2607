package com.zeroone.star.project.enums.j3.storehouse;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * 库房药房运营状态枚举
 * @author mie
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum OperationalEnum {
    IN_USE("IN_USE", "使用中"),
    NOT_IN_USE("NOT_IN_USE", "未使用");

    private final String desc;
    private final String dictText;

    public static String getDictTextByDesc(String desc) {
        if (desc == null) return null;
        for (OperationalEnum e : values()) {
            if (e.desc.equals(desc)) return e.dictText;
        }
        return null;
    }
}
