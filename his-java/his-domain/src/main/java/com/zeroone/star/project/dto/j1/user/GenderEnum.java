package com.zeroone.star.project.dto.j1.user;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonValue;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 *
 */
@Getter
@AllArgsConstructor
public enum GenderEnum {
    UNKNOWN(0),
    FEMALE(1),
    MALE(2);

    private final int code;

    /**
     * 序列化为 JSON 时返回数字（前端友好）
     */
    @JsonValue
    public int getCode() {
        return code;
    }

    /**
     * 反序列化时从数字构建枚举（支持前端传 0/1/2）
     */
    @JsonCreator
    public static GenderEnum fromCode(int code) {
        for (GenderEnum gender : GenderEnum.values()) {
            if (gender.code == code) {
                return gender;
            }
        }
        throw new IllegalArgumentException("Invalid gender code: " + code);
    }

    /**
     * 可选：从数据库字符串值（如 "FEMALE"）获取枚举
     */
    public static GenderEnum fromDbValue(String dbValue) {
        try {
            return GenderEnum.valueOf(dbValue.toUpperCase());
        } catch (IllegalArgumentException e) {
            throw new IllegalArgumentException("Invalid gender db value: " + dbValue);
        }
    }
}
