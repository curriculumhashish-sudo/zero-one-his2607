package com.zeroone.star.basedata.constant;

import java.util.Arrays;
import java.util.List;

public class TreatmentConstants {

    public static final List<String> CATEGORY_LIST = Arrays.asList(
            "检验", "检查", "治疗", "手术", "麻醉", "护理级别", "护理项目", "膳食", "输血", "输氧",
            "其他", "转科", "术后", "出院", "转院", "死亡", "产后", "病情", "护理常规", "体位",
            "陪护", "隔离等级", "口腔检查", "内镜检查", "内镜治疗", "床位服务", "基础服务", "诊断费",
            "预防接种手续费", "口腔治疗"
    );

    public static List<String> listCategoryOptions() {
        return CATEGORY_LIST;
    }
}
