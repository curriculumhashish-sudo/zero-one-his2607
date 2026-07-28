package com.zeroone.star.sysmgr.enums;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * <p>
 * 描述：配置定义枚举，对应参考项目 SettingDefinitionEnum，硬编码所有可配置项元数据
 * </p>
 *
 * @author L
 * @version 1.0.0
 */
@Getter
@AllArgsConstructor
public enum SettingDefinitionEnum {

    STUDENT_SWITCH("studentSwich", "学生开关"),

    YB_SWITCH("ybSwich", "医保开关"),

    DEVICE_AUTO_INVENTORY_DEDUCT_SWITCH("deviceAutoInventoryDeductSwich", "耗材是否自动扣库"),

    OUTPATIENT_PRESCRIPTION_AMOUNT_LIMIT_YUAN("outpatientPrescriptionAmountLimitYuan", "门诊处方金额上限(元)");

    /**
     * 配置键名（对应 whale_settings.name）
     */
    private final String name;

    /**
     * 显示标题
     */
    private final String title;
}
