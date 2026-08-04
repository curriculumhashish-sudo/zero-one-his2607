package com.zeroone.star.basedata.constant;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 诊断管理共享枚举常量
 * <p>
 * 对应 cln_condition_definition.source_enum / type_code / status_enum
 * 的枚举 code → 中文名映射，以及 code 合法性校验，方便本地开发/测试使用。
 * </p>
 * <p>
 * 注意：正式环境中这些枚举以其他组维护的 MongoDB 枚举数据为准，
 * 本类仅作为本地测试与展示兜底，后续若与线上数据不一致需以线上为准。
 * </p>
 * @author j4组
 * @version 1.0.0
 */
public class DiagnosisConstants {

    // ======================== 诊断目录（condition_definition_source）========================

    /**
     * 诊断目录（condition_definition_source）编码 → 中文名 映射
     * key   = sourceEnum（存入 cln_condition_definition.source_enum）
     * value = 诊断目录中文名（显示用）
     */
    public static final Map<String, String> SOURCE_MAP = new LinkedHashMap<>();

    // ======================== 诊断类别（diag_type）========================

    /**
     * 诊断类别（diag_type）编码 → 中文名 映射
     * key   = typeCode（存入 cln_condition_definition.type_code）
     * value = 诊断类别中文名（显示用）
     */
    public static final Map<String, String> TYPE_MAP = new LinkedHashMap<>();

    // ======================== 发布状态（PublicationStatus）========================

    /**
     * 发布状态（PublicationStatus）枚举编码：草稿
     */
    public static final String STATUS_DRAFT = "DRAFT";

    /**
     * 发布状态（PublicationStatus）枚举编码：已启用
     */
    public static final String STATUS_ACTIVE = "ACTIVE";

    /**
     * 发布状态（PublicationStatus）枚举编码：已停用
     */
    public static final String STATUS_INACTIVE = "INACTIVE";

    /**
     * 发布状态（PublicationStatus）枚举编码：已废弃
     */
    public static final String STATUS_DISCARDED = "DISCARDED";

    /**
     * 发布状态（PublicationStatus）编码 → 中文名 映射
     * key   = statusEnum（存入 cln_condition_definition.status_enum）
     * value = 发布状态中文名（显示用）
     */
    public static final Map<String, String> STATUS_MAP = new LinkedHashMap<>();

    static {
        // condition_definition_source 枚举
        SOURCE_MAP.put("general_dx", "疾病与诊断");
        SOURCE_MAP.put("chronic_spec", "特慢病诊断目录");
        SOURCE_MAP.put("disease_pkg", "按病种目录");
        SOURCE_MAP.put("paid_dx", "付费诊断");
        SOURCE_MAP.put("day_surgery_dx", "日间手术治疗病种");
        SOURCE_MAP.put("tcm_dx", "中医诊断");
        SOURCE_MAP.put("tcm_syndrome", "中医证候目录");
        SOURCE_MAP.put("tumor_morph", "肿瘤形态学目录");

        // diag_type 枚举
        TYPE_MAP.put("1", "西医诊断");
        TYPE_MAP.put("2", "中医主病诊断");
        TYPE_MAP.put("3", "中医主证诊断");

        // PublicationStatus 枚举
        STATUS_MAP.put("DRAFT", "草稿");
        STATUS_MAP.put("ACTIVE", "已启用");
        STATUS_MAP.put("INACTIVE", "已停用");
        STATUS_MAP.put("DISCARDED", "已废弃");
    }

    // ======================== 便捷查询方法 ========================

    /**
     * 根据诊断目录编码获取中文名
     *
     * @param code 诊断目录编码（如 general_dx）
     * @return 中文名，未找到返回 null
     */
    public static String getSourceName(String code) {
        return SOURCE_MAP.get(code);
    }

    /**
     * 根据诊断类别编码获取中文名
     *
     * @param code 诊断类别编码（如 "1"）
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
     * 校验诊断目录编码是否合法（本地测试用）
     *
     * @param code 诊断目录编码
     * @return 是否存在于 condition_definition_source 约定值中
     */
    public static boolean isValidSource(String code) {
        return SOURCE_MAP.containsKey(code);
    }

    /**
     * 校验诊断类别编码是否合法（本地测试用）
     *
     * @param code 诊断类别编码
     * @return 是否存在于 diag_type 约定值中
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
