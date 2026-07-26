package com.zeroone.star.basedata.constant;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 器材分类常量
 * 对应 adm_device_definition.category_code 的字典映射。
 * 分类编码规则：11 = 有源的，12 = 治疗设备，依此类推。
 */
public class EquipmentCategoryConstants {

    /**
     * 器材分类编码 → 分类名称 映射
     * <p>
     * key   = categoryCode（存入 adm_device_definition.category_code）
     * value = 分类中文名（显示用）
     */
    public static final Map<String, String> CATEGORY_MAP = new LinkedHashMap<>();

    static {
        CATEGORY_MAP.put("11", "有源的");
        CATEGORY_MAP.put("12", "治疗设备");
        CATEGORY_MAP.put("13", "通讯类");
        CATEGORY_MAP.put("14", "非院内使用");
        CATEGORY_MAP.put("15", "植入类");
        CATEGORY_MAP.put("16", "试管类");
        CATEGORY_MAP.put("17", "床旁类");
        CATEGORY_MAP.put("18", "单次消耗类");
        CATEGORY_MAP.put("19", "可重用的");
        CATEGORY_MAP.put("20", "软件类");
        CATEGORY_MAP.put("21", "医用耗材");
        CATEGORY_MAP.put("22", "检验试剂");
    }

    /**
     * 根据分类编码获取分类名称
     *
     * @param code 分类编码
     * @return 分类中文名，未找到返回 null
     */
    public static String getCategoryName(String code) {
        return CATEGORY_MAP.get(code);
    }
}
