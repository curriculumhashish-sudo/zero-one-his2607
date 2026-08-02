package com.zeroone.star.basedata.constant;

import java.util.HashMap;
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

    /**
     * 采购单位编码
     */
    public static final Map<String, String> UNIT_CODE = new HashMap<>();

    /**
     * 收费类型编码
     */
    public static final Map<String, String> FIN_TYPE_CODE = new HashMap<>();

    /**
     * 医保类型编码
     */
    public static final Map<String, String> YB_TYPE_CODE = new HashMap<>();

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

        UNIT_CODE.put("1", "片");
        UNIT_CODE.put("2", "盒");
        UNIT_CODE.put("3", "ml");
        UNIT_CODE.put("4", "mg");
        UNIT_CODE.put("5", "粒");
        UNIT_CODE.put("6", "次");
        UNIT_CODE.put("7", "箱");
        UNIT_CODE.put("8", "瓶");
        UNIT_CODE.put("9", "支");
        UNIT_CODE.put("10", "g");
        UNIT_CODE.put("11", "袋");

        FIN_TYPE_CODE.put("1001", "西药");
        FIN_TYPE_CODE.put("1002", "外购药品");
        FIN_TYPE_CODE.put("1003", "中草药");
        FIN_TYPE_CODE.put("1004", "化验");
        FIN_TYPE_CODE.put("1005", "手术");
        FIN_TYPE_CODE.put("1007", "诊断");
        FIN_TYPE_CODE.put("1008", "治疗");
        FIN_TYPE_CODE.put("1009", "护理");
        FIN_TYPE_CODE.put("1010", "床位");
        FIN_TYPE_CODE.put("1011", "挂号费");
        FIN_TYPE_CODE.put("1012", "诊察费");
        FIN_TYPE_CODE.put("1013", "其他费");
        FIN_TYPE_CODE.put("2001", "麻醉");
        FIN_TYPE_CODE.put("2002", "膳食");
        FIN_TYPE_CODE.put("2003", "输血费");
        FIN_TYPE_CODE.put("2004", "自备医嘱");
        FIN_TYPE_CODE.put("2005", "医用耗材");
        FIN_TYPE_CODE.put("2006", "循环物质");
        FIN_TYPE_CODE.put("2007", "物资");
        FIN_TYPE_CODE.put("2008", "办公用品");
        FIN_TYPE_CODE.put("2009", "检查");
        FIN_TYPE_CODE.put("2010", "中成药");
        FIN_TYPE_CODE.put("2011", "CT");
        FIN_TYPE_CODE.put("2012", "电诊");

        YB_TYPE_CODE.put("1", "自费");
        YB_TYPE_CODE.put("2", "甲类");
        YB_TYPE_CODE.put("3", "乙类");
        YB_TYPE_CODE.put("4", "丙类");
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
