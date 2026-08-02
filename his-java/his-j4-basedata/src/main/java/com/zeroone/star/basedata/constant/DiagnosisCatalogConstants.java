package com.zeroone.star.basedata.constant;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 诊断目录临时数据。
 *
 * <p>公共编码管理接口完成前用于开发阶段模拟，联调时只需替换数据来源。</p>
 */
public final class DiagnosisCatalogConstants {

    private static final Map<String, String> CATALOG_OPTIONS;

    static {
        Map<String, String> options = new LinkedHashMap<>();
        options.put("general_dx", "疾病与诊断");
        options.put("chronic_spec", "特慢病诊断目录");
        options.put("disease_pkg", "按病种目录");
        options.put("paid_dx", "付费诊断");
        options.put("day_surgery_dx", "日间手术治疗病种");
        options.put("tcm_dx", "中医诊断");
        options.put("tcm_syndrome", "中医证候目录");
        options.put("tumor_morph", "肿瘤形态学目录");
        CATALOG_OPTIONS = Collections.unmodifiableMap(options);
    }

    private DiagnosisCatalogConstants() {
    }

    /**
     * 获取按页面顺序排列的诊断目录编码和名称。
     *
     * @return 不可修改的目录数据
     */
    public static Map<String, String> catalogOptions() {
        return CATALOG_OPTIONS;
    }
}
