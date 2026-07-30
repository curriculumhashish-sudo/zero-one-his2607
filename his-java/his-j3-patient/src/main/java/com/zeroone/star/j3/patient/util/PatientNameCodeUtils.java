package com.zeroone.star.j3.patient.util;

import net.sourceforge.pinyin4j.PinyinHelper;

import java.util.Locale;

/**
 * 患者姓名检索码工具。
 *
 * @author 照夜清
 */
public final class PatientNameCodeUtils {

    private PatientNameCodeUtils() {
    }

    /**
     * 按产品原型规则获取每个汉字的首个拼音首字母。
     * 非汉字字符原样保留，最终统一为大写。
     *
     * @param name 患者姓名
     * @return 拼音首字母检索码
     */
    public static String toPinyinInitials(String name) {
        if (name == null || name.isBlank()) {
            return null;
        }

        StringBuilder result = new StringBuilder();
        for (int index = 0; index < name.length(); index++) {
            char current = name.charAt(index);
            String[] pinyinValues =
                    PinyinHelper.toHanyuPinyinStringArray(current);
            if (pinyinValues == null || pinyinValues.length == 0) {
                result.append(current);
            } else {
                result.append(pinyinValues[0].charAt(0));
            }
        }
        return result.toString().toUpperCase(Locale.ROOT);
    }
}
