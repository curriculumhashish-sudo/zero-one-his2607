package com.zeroone.star.project.enums.j3.department;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * <p>
 * 描述：科室管理类别枚举类
 * 其中，value为前段给后段传递的值，desc为前数据库中的值，dictText为字典文本，同时也是前端展示的文本
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum ClassEnum implements BaseEnum {

    CLINIC(1, "CLINIC", "门诊"),
    INPATIENT(2, "INPATIENT", "住院"),
    PHARMACY(3, "PHARMACY", "药房"),
    STORAGE(4, "STORAGE", "库房"),
    FIN(5, "FIN", "财务"),
    NS(6, "NS", "护士站"),
    MANAGER(7, "MANAGER", "管理"),
    SUPPORT(8, "SUPPORT", "后勤支持"),
    OTHER(9, "OTHER", "其他"),
    TECH(10, "TECH", "医技");

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
        for (ClassEnum e : values()) {
            if (e.desc.equals(desc)) {
                return e.value;
            }
        }
        throw new IllegalArgumentException("类别不存在");
    }

    public static String getDictTextByDesc(String desc) {
        if (desc == null) {
            return null;
        }
        for (ClassEnum e : values()) {
            if (e.desc.equals(desc)) {
                return e.dictText;
            }
        }
        throw new IllegalArgumentException("类别不存在");
    }

    public static String getDescByValue(Integer value) {
        if (value == null) {
            return null;
        }
        for (ClassEnum e : values()) {
            if (e.value.equals(value)) {
                return e.desc;
            }
        }
        throw new IllegalArgumentException("类别不存在");
    }
}
