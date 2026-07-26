package com.zeroone.star.project.enums.j3.department;

import com.zeroone.star.project.enums.BaseEnum;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * <p>
 * 描述：科室管理类型枚举类
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
