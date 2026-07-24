package com.zeroone.star.configuration.orderset.enums;

import com.zeroone.star.project.exception.BusinessException;

import java.util.Locale;

/**
 * 组套模板生命周期操作。
 */
public enum OrderSetActionEnum {
    PUBLISH,
    RETIRE;

    /**
     * 将旧版ENABLE/DISABLE动作兼容为领域动作。
     */
    public static OrderSetActionEnum from(String value) {
        if (value == null) {
            throw new BusinessException("组套生命周期操作不能为空");
        }
        return switch (value.toUpperCase(Locale.ROOT)) {
            case "PUBLISH", "ENABLE" -> PUBLISH;
            case "RETIRE", "DISABLE" -> RETIRE;
            default -> throw new BusinessException("不支持的组套生命周期操作：" + value);
        };
    }
}
