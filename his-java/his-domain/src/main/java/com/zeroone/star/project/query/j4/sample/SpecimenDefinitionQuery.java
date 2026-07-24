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
public class SpecimenDefinitionQuery extends PageQuery {

    /**
     * 样本名称（支持模糊搜索）
     */
    @Schema(description = "样本名称（模糊搜索）", example = "血")
    private String name;

    /**
     * 拼音检索码（支持模糊搜索）
     */
    @Schema(description = "拼音检索码（模糊搜索）", example = "X")
    private String pyStr;

    /**
     * 五笔检索码（支持模糊搜索）
     */
    @Schema(description = "五笔检索码（模糊搜索）", example = "T")
    private String wbStr;

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
