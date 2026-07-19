package com.zeroone.star.basedata.constant;

import com.zeroone.star.project.dto.j4.treatment.TreatmentOptionDTO;

import java.util.ArrayList;
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

    private static final Map<String, String> STATUS_MAP = new HashMap<>();
    private static final Map<String, String> TYPE_MAP = new HashMap<>();

    static {
        STATUS_MAP.put("ACTIVE", "启用");
        STATUS_MAP.put("INACTIVE", "停用");

        TYPE_MAP.put("OUTPATIENT_SERVICE", "门诊服务");
        TYPE_MAP.put("INPATIENT_SERVICE", "住院服务");
        TYPE_MAP.put("OTHER", "其他");
    }

    public static List<TreatmentOptionDTO> listCategoryOptions() {
        List<TreatmentOptionDTO> options = new ArrayList<>();
        for (String name : CATEGORY_LIST) {
            options.add(new TreatmentOptionDTO(name, name));
        }
        return options;
    }

    public static String getStatusText(String code) {
        return STATUS_MAP.getOrDefault(code, code);
    }

    public static String getTypeName(String code) {
        return TYPE_MAP.getOrDefault(code, code);
    }
}
