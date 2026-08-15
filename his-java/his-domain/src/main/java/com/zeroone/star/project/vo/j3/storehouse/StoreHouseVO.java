package com.zeroone.star.project.vo.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * 库房药房显示对象
 * @author mie
 * @version 1.0.0
 */
@Schema(description = "库房药房显示对象")
@Data
public class StoreHouseVO implements Serializable {

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "名称")
    private String name;

    @Schema(description = "状态枚举值")
    private String statusEnum;

    @Schema(description = "状态文本")
    private String statusEnumText;

    @Schema(description = "运营状态枚举值")
    private String operationalEnum;

    @Schema(description = "运营状态文本")
    private String operationalEnumText;

    @Schema(description = "模式枚举值")
    private String modeEnum;

    @Schema(description = "模式文本")
    private String modeEnumText;

    @Schema(description = "类型扩展信息")
    private Map<String, Object> typeExtensions;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;

    @Schema(description = "形态枚举值")
    private String formEnum;

    @Schema(description = "形态文本")
    private String formEnumText;

    @Schema(description = "机构ID")
    private String organizationId;

    @Schema(description = "机构名称")
    private String organizationName;

    @Schema(description = "显示顺序")
    private Integer displayOrder;

    @Schema(description = "上级科室ID")
    private String parentId;

    @Schema(description = "上级科室名称")
    private String parentName;

    @Schema(description = "子节点")
    private List<StoreHouseVO> children = new ArrayList<>();
}
