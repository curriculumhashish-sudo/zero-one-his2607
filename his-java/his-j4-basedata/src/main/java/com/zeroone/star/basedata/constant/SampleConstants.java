package com.zeroone.star.basedata.constant;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 样本管理共享枚举常量
 * <p>
 * 对应 adm_specimen_definition.type_enum / status_enum 的枚举 code → 中文名映射，
 * 以及 code 合法性校验，方便本地开发/测试使用。
 * </p>
 * <p>
 * 注意：正式环境中这些枚举以其他组维护的 MongoDB 枚举数据为准，
 * 本类仅作为本地测试与展示兜底，后续若与线上数据不一致需以线上为准。
 * </p>
 * @author j4组
 * @version 1.0.0
 */
public class SampleConstants {

    /**
     * 发布状态（PublicationStatus）枚举编码：草稿
     */
    public static final String STATUS_DRAFT = "DRAFT";

    /**
     * 发布状态（PublicationStatus）枚举编码：已启用（新增样本时由服务端默认设置）
     */
    public static final String STATUS_ACTIVE = "ACTIVE";

    /**
     * 发布状态（PublicationStatus）枚举编码：已停用
     */
    public static final String STATUS_INACTIVE = "INACTIVE";

    /**
     * 标本种类（SpecimenType）编码 → 中文名 映射
     * key   = typeEnum（存入 adm_specimen_definition.type_enum）
     * value = 标本种类中文名（显示用）
     */
    public static final Map<String, String> TYPE_MAP = new LinkedHashMap<>();

    /**
     * 发布状态（PublicationStatus）编码 → 中文名 映射
     * key   = statusEnum（存入 adm_specimen_definition.status_enum）
     * value = 发布状态中文名（显示用）
     */
    public static final Map<String, String> STATUS_MAP = new LinkedHashMap<>();

    static {
        // SpecimenType 枚举
        TYPE_MAP.put("BLOOD", "血液");
        TYPE_MAP.put("URINE", "尿液");
        TYPE_MAP.put("STOOL", "粪便");
        TYPE_MAP.put("SPUTUM", "痰液");

        // PublicationStatus 枚举
        STATUS_MAP.put("DRAFT", "草稿");
        STATUS_MAP.put("ACTIVE", "已启用");
        STATUS_MAP.put("INACTIVE", "已停用");
    }

    /**
     * 根据标本种类编码获取中文名
     *
     * @param code 标本种类编码（如 BLOOD）
     * @return 中文名，未找到返回 null
     */
    public static String getTypeName(String code) {
        return TYPE_MAP.get(code);
    }

    /**
     * 根据发布状态编码获取中文名
     *
     * @param code 发布状态编码（如 ACTIVE）
     * @return 中文名，未找到返回 null
     */
    public static String getStatusName(String code) {
        return STATUS_MAP.get(code);
    }

    /**
     * 校验标本种类编码是否合法（本地测试用）
     *
     * @param code 标本种类编码
     * @return 是否存在于 SpecimenType 约定值中
     */
    public static boolean isValidType(String code) {
        return TYPE_MAP.containsKey(code);
    }

    /**
     * 校验发布状态编码是否合法（本地测试用）
     *
     * @param code 发布状态编码
     * @return 是否存在于 PublicationStatus 约定值中
     */
    public static boolean isValidStatus(String code) {
        return STATUS_MAP.containsKey(code);
    }
}
