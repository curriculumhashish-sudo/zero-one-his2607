package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：住院退药-患者队列显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药-患者队列显示对象")
public class InpatientRefundPatientVO {

    /**
     * 就诊ID
     */
    @Schema(description = "就诊ID", example = "329874660147597312", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    /**
     * 患者姓名
     */
    @Schema(description = "患者姓名", example = "张三", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientName;

    /**
     * 性别编码
     */
    @Schema(description = "性别编码", example = "1")
    private String genderEnum;

    /**
     * 性别文案
     */
    @Schema(description = "性别文案", example = "男")
    private String genderEnumText;

    /**
     * 年龄
     */
    @Schema(description = "年龄", example = "45岁")
    private String age;

    /**
     * 病区名称
     */
    @Schema(description = "病区名称", example = "内科病区")
    private String wardName;

    /**
     * 床位名称
     */
    @Schema(description = "床位名称", example = "08床")
    private String bedName;

    /**
     * 住院号
     */
    @Schema(description = "住院号", example = "ZY20260719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterNo;

    /**
     * 退药状态码
     */
    @Schema(description = "退药状态码", example = "PENDING_REFUND")
    private String refundEnum;

    /**
     * 退药状态文案
     */
    @Schema(description = "退药状态文案", example = "待退药")
    private String refundEnumText;
}
