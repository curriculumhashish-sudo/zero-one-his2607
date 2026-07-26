package com.zeroone.star.basedata.constant;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class TreatmentConstants {

    public static final List<String> CATEGORY_LIST = Arrays.asList(
            "检验", "检查", "治疗", "手术", "麻醉", "护理级别", "护理项目", "膳食", "输血", "输氧",
            "其他", "转科", "术后", "出院", "转院", "死亡", "产后", "病情", "护理常规", "体位",
            "陪护", "隔离等级", "口腔检查", "内镜检查", "内镜治疗", "床位服务", "基础服务", "诊断费",
            "预防接种手续费", "口腔治疗"
    );

    private static final Map<String, String> CATEGORY_MAP = new HashMap<>();
    private static final Map<String, String> STATUS_MAP = new HashMap<>();
    private static final Map<String, String> TYPE_MAP = new HashMap<>();
    private static final Map<String, String> UNIT_MAP = new HashMap<>();
    private static final Map<String, String> ITEM_TYPE_MAP = new HashMap<>();

    static {
        CATEGORY_MAP.put("21", "治疗");
        CATEGORY_MAP.put("22", "检验");
        CATEGORY_MAP.put("23", "检查");
        CATEGORY_MAP.put("24", "手术");
        CATEGORY_MAP.put("25", "麻醉");
        CATEGORY_MAP.put("26", "护理级别");
        CATEGORY_MAP.put("50", "护理项目");
        CATEGORY_MAP.put("27", "膳食");
        CATEGORY_MAP.put("28", "输血");
        CATEGORY_MAP.put("29", "输氧");
        CATEGORY_MAP.put("31", "其他");
        CATEGORY_MAP.put("32", "转科");
        CATEGORY_MAP.put("33", "术后");
        CATEGORY_MAP.put("34", "出院");
        CATEGORY_MAP.put("35", "转院");
        CATEGORY_MAP.put("36", "死亡");
        CATEGORY_MAP.put("37", "产后");
        CATEGORY_MAP.put("38", "病情");
        CATEGORY_MAP.put("39", "护理常规");
        CATEGORY_MAP.put("40", "体位");
        CATEGORY_MAP.put("41", "陪护");
        CATEGORY_MAP.put("42", "隔离等级");
        CATEGORY_MAP.put("43", "口腔检查");
        CATEGORY_MAP.put("44", "内镜检查");
        CATEGORY_MAP.put("45", "内镜治疗");
        CATEGORY_MAP.put("51", "床位服务");
        CATEGORY_MAP.put("46", "基础服务");
        CATEGORY_MAP.put("47", "诊断费");
        CATEGORY_MAP.put("48", "预防接种手续费");
        CATEGORY_MAP.put("49", "口腔治疗");

        STATUS_MAP.put("DRAFT", "草稿");
        STATUS_MAP.put("ACTIVE", "已启用");
        STATUS_MAP.put("INACTIVE", "已停用");
        STATUS_MAP.put("DISCARDED", "已废弃");

        TYPE_MAP.put("1", "检验");
        TYPE_MAP.put("2", "检查");
        TYPE_MAP.put("3", "护理");
        TYPE_MAP.put("4", "手术");
        TYPE_MAP.put("5", "其他");
    }

    public static List<String> listCategoryOptions() {
        return CATEGORY_LIST;
    }

    public static String getStatusText(String code) {
        if (code == null) {
            return null;
        }
        return STATUS_MAP.getOrDefault(code, code);
    }

    public static String getTypeName(String code) {
        if (code == null) {
            return null;
        }
        return TYPE_MAP.getOrDefault(code, code);
    }

    public static String getCategoryName(String code) {
        if (code == null) {
            return null;
        }
        return CATEGORY_MAP.getOrDefault(code, code);
    }

    public static String getUnitName(String code) {
        if (code == null) {
            return null;
        }
        return UNIT_MAP.getOrDefault(code, code);
    }

    public static String getItemTypeName(String code) {
        if (code == null) {
            return null;
        }
        return ITEM_TYPE_MAP.getOrDefault(code, code);
    }
}
