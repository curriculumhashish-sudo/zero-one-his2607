package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * 【前端对接说明 - 诊疗列表】
 *
 * === 用途 ===
 * 本VO用于「诊疗项目管理」列表页的分页查询结果展示，对应 GET /treatment/query-treatment-list 接口。
 *
 * === 列表页表格列 ===
 * ┌──────────┬──────────────┬──────────────────────────────────────────────────────┐
 * │ 前端列名  │ 字段名        │ 说明                                                 │
 * ├──────────┼──────────────┼──────────────────────────────────────────────────────┤
 * │ 诊疗名称  │ name          │ 点击可跳转到详情页/编辑页，传id参数                   │
 * │ 业务类型  │ typeEnum      │ 数字枚举翻译：1=检验/2=检查/3=护理/4=手术/5=其他       │
 * │ 状态      │ statusEnum    │ 数字枚举翻译+颜色标签：                               │
 * │          │              │   1=草稿(灰色) / 2=已启用(绿色) / 3=已停用(橙色) / 4=已废弃(红色) │
 * │ 价格      │ price         │ 零售价，单位元，保留2位小数，格式化为¥xx.xx展示        │
 * │ (操作列)  │ -             │ 编辑、停用/启用、删除按钮，使用id调用对应接口          │
 * └──────────┴──────────────┴──────────────────────────────────────────────────────┘
 *
 * === 注意事项 ===
 * 1. 字典翻译复用 TreatmentDetailVO 中的映射规则，不要单独维护重复字典表。
 * 2. price 字段是零售价（SALE类型价格），未设置价格时返回null，前端展示为"--"或"待定价"。
 * 3. 点击行名称跳转详情页：/treatment/detail?id={id}，调用 get-treatment 接口获取完整数据。
 * 4. 列表页支持按关键词（名称/编码）搜索和按目录分类筛选，筛选条件通过TreatmentQuery传入。
 *
 * @author j4组-treatment模块
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗项目列表显示对象（管理列表分页查询结果）")
public class TreatmentListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "诊疗项目主键ID（点击行名称跳转详情/编辑时使用）", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "诊疗项目名称（列表主列，可点击跳转详情）", example = "静脉注射", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "发布状态（数字枚举+颜色标签）：1=草稿(灰)/2=已启用(绿)/3=已停用(橙)/4=已废弃(红)", example = "2", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer statusEnum;

    @Schema(description = "业务类型（数字枚举翻译）：1=检验/2=检查/3=护理/4=手术/5=其他", example = "5", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer typeEnum;

    @Schema(description = "零售价（单位：元，保留2位小数，未设置时为null展示--）", example = "6.00")
    private BigDecimal price;
}
