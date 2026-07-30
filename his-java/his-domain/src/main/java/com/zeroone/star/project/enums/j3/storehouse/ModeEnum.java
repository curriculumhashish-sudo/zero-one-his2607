package com.zeroone.star.project.enums.j3.storehouse;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * 库房药房模式枚举
 * @author mie
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum ModeEnum {
    INSTANCE("INSTANCE", "具体实例"),
    CLASS("CLASS", "分类");

    private final String desc;
    private final String dictText;

    public static String getDictTextByDesc(String desc) {
        if (desc == null) return null;
        for (ModeEnum e : values()) {
            if (e.desc.equals(desc)) return e.dictText;
        }
        return null;
    }
}
