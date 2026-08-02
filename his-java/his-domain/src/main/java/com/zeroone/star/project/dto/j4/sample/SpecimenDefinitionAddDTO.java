package com.zeroone.star.project.dto.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * <p>
 * 描述：样本定义 新增 请求参数
 * 说明：业务编码(busNo)由前端自动生成后传入；拼音/五笔检索码由前端拼音库生成后传入；
 * 发布状态(statusEnum)不在入参中，由服务端调用 PublicationStatus 枚举默认置为启用(ACTIVE)
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "样本定义新增请求参数")
public class SpecimenDefinitionAddDTO {

    /**
     * 业务编码（样本定义聚合根，前端自动生成后传入）
     */
    @NotBlank(message = "业务编码不能为空")
    @Size(max = 255, message = "业务编码长度不能超过255个字符")
    @Pattern(regexp = "^[A-Za-z0-9_-]+$", message = "业务编码只能包含字母、数字、下划线和中划线")
    @Schema(description = "业务编码，全局唯一，前端自动生成，仅允许字母/数字/下划线/中划线",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "SPD0001", maxLength = 255)
    private String busNo;

    /**
     * 样本名称（面向临床与维护页的显示名称）
     */
    @NotBlank(message = "样本名称不能为空")
    @Size(max = 50, message = "样本名称长度不能超过50个字符")
    @Schema(description = "样本名称，面向临床与维护页展示",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "血", maxLength = 50)
    private String name;

    /**
     * 拼音检索码（前端拼音库生成后传入）
     */
    @NotBlank(message = "拼音检索码不能为空")
    @Size(max = 255, message = "拼音检索码长度不能超过255个字符")
    @Pattern(regexp = "^[A-Za-z]*$", message = "拼音检索码只能包含字母")
    @Schema(description = "拼音检索码，前端拼音库生成后传入",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "X", maxLength = 255)
    private String pyStr;

    /**
     * 五笔检索码（前端拼音库生成后传入）
     */
    @NotBlank(message = "五笔检索码不能为空")
    @Size(max = 255, message = "五笔检索码长度不能超过255个字符")
    @Pattern(regexp = "^[A-Za-z]*$", message = "五笔检索码只能包含字母")
    @Schema(description = "五笔检索码，前端拼音库生成后传入",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "T", maxLength = 255)
    private String wbStr;

    /**
     * 标本种类编码（对应 SpecimenType 枚举）
     */
    @NotBlank(message = "标本种类不能为空")
    @Size(max = 32, message = "标本种类编码长度不能超过32个字符")
    @Pattern(regexp = "^[A-Z][A-Z0-9_]*$", message = "标本种类编码格式不正确，只能为大写字母、数字或下划线")
    @Schema(description = "标本种类编码，对应 SpecimenType 枚举（BLOOD-血液、URINE-尿液、STOOL-粪便、SPUTUM-痰液）",
            requiredMode = Schema.RequiredMode.REQUIRED,
            example = "BLOOD",
            allowableValues = {"BLOOD", "URINE", "STOOL", "SPUTUM"})
    private String typeEnum;
}
