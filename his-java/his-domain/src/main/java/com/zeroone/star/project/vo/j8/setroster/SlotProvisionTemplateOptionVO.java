package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述：放号时段模板名称下拉选项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@Schema(description = "放号时段模板名称下拉选项")
public class SlotProvisionTemplateOptionVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "选项值（模板主键ID）", example = "tpl-001")
    private String value;

    @Schema(description = "选项显示文本（模板名称）", example = "门诊上午时段")
    private String label;

    @Schema(description = "排序号", example = "1")
    private Integer sort;

    @Schema(description = "业务编号（模板编码）", example = "SPT000001")
    private String busNo;

    @Schema(description = "子选项列表（树形结构，放号时段模板通常无子节点）")
    private List<SlotProvisionTemplateOptionVO> children;
}
