package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * <p>
 * 描述：新增诊断传输数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "新增诊断传输数据对象")
public class DiagnosisSaveDTO {

    /**
     * 病情编码，格式如 CDF00000001，由前端生成
     */
    @NotBlank(message = "病情编码不能为空")
    @Size(max = 50, message = "病情编码长度不能超过50个字符")
    @Schema(description = "病情编码，格式如 CDF00000001", example = "CDF00000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String conditionCode;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @NotBlank(message = "来源枚举不能为空")
    @Size(max = 50, message = "来源枚举长度不能超过50个字符")
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)",
            example = "general_dx", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceEnum;

    /**
     * 诊断名称
     */
    @NotBlank(message = "诊断名称不能为空")
    @Size(max = 255, message = "诊断名称长度不能超过255个字符")
    @Schema(description = "诊断名称", example = "高血压", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 诊断类型编码
     */
    @Size(max = 50, message = "诊断类型编码长度不能超过50个字符")
    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    /**
     * 疾病编码（ICD 映射），可选
     */
    @Size(max = 100, message = "疾病编码长度不能超过100个字符")
    @Schema(description = "疾病编码（ICD 映射）", example = "1.002")
    private String diseaseCode;

    /**
     * 医保标记：0 否 / 1 是
     */
    @Schema(description = "医保标记：0 否 / 1 是", example = "1")
    private Boolean ybFlag;

    /**
     * 医保编号
     */
    @Size(max = 100, message = "医保编号长度不能超过100个字符")
    @Schema(description = "医保编号", example = "YB00123456")
    private String ybNo;

    /**
     * 医保匹配标记：0 未匹配 / 1 已匹配
     */
    @Schema(description = "医保匹配标记：0 未匹配 / 1 已匹配", example = "1")
    private Boolean ybMatchFlag;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @NotBlank(message = "状态枚举不能为空")
    @Size(max = 20, message = "状态枚举长度不能超过20个字符")
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    /**
     * 描述说明
     */
    @Size(max = 500, message = "描述说明长度不能超过500个字符")
    @Schema(description = "描述说明", example = "慢性血压升高性疾病")
    private String description;
}