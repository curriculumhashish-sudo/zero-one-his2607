package com.zeroone.star.project.query.j4.sample;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：样本定义分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "样本定义分页查询对象")
public class SpecimenDefinitionQuery extends PageQuery {

    /**
     * 关键词（样本名称/业务编码/拼音检索码/五笔检索码 模糊匹配）
     */
    @Schema(description = "关键词，同时模糊匹配样本名称、业务编码、拼音检索码、五笔检索码", example = "血")
    private String keyword;

    /**
     * 标本种类编码（对应 SpecimenType 枚举）
     */
    @Schema(description = "标本种类编码（如：BLOOD-血液、URINE-尿液、STOOL-粪便、SPUTUM-痰液）", example = "BLOOD")
    private String typeEnum;

    /**
     * 发布状态编码（对应 PublicationStatus 枚举）
     */
    @Schema(description = "发布状态编码（如：ACTIVE-已启用、INACTIVE-已停用、DRAFT-草稿）", example = "ACTIVE")
    private String statusEnum;
}
