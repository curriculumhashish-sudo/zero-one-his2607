package com.zeroone.star.project.dto.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：样本定义 新增 请求参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@Schema(description = "样本定义新增请求")
public class SpecimenDefinitionAddDTO {

    /**
     * 业务编码（样本定义聚合根）
     */
    @NotBlank(message = "业务编码不能为空")
    @Schema(description = "业务编码", example = "SPD0001")
    private String busNo;

    /**
     * 样本名称（面向临床与维护页的显示名称）
     */
    @NotBlank(message = "样本名称不能为空")
    @Schema(description = "样本名称", example = "血")
    private String name;

    /**
     * 拼音检索码
     */
    @Schema(description = "拼音检索码", example = "X")
    private String pyStr;

    /**
     * 五笔检索码
     */
    @Schema(description = "五笔检索码", example = "T")
    private String wbStr;

    /**
     * 标本种类编码（对应 SpecimenType 枚举）
     */
    @NotBlank(message = "标本种类不能为空")
    @Schema(description = "标本种类编码（如：BLOOD-血液、URINE-尿液、STOOL-粪便、SPUTUM-痰液）", example = "BLOOD")
    private String typeEnum;

    /**
     * 发布状态编码（对应 PublicationStatus 枚举）
     */
    @NotBlank(message = "发布状态不能为空")
    @Schema(description = "发布状态编码（如：ACTIVE-已启用、INACTIVE-已停用、DRAFT-草稿）", example = "ACTIVE")
    private String statusEnum;
}
