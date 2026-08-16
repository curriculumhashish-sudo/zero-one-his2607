package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 【前端对接说明 - 诊疗子项】
 *
 * === 用途 ===
 * 本对象用于「组合项目/套餐」的子项列表展示和编辑回显。
 * 出现在：TreatmentDetailVO.children 数组中；新增/编辑页面的「子项」动态列表区域。
 *
 * === 前端页面展示 ===
 * ┌──────────────┬──────────────────┬──────────────────────────────────────────────┐
 * │ 前端标签      │ 字段名            │ 控件说明                                     │
 * ├──────────────┼──────────────────┼──────────────────────────────────────────────┤
 * │ 名称          │ childrenName     │ 只读文本展示（子项名称快照，点击右侧删除按钮）│
 * │ （隐藏）      │ childrenId       │ 隐藏字段，提交时带上，是子项诊疗项目的主键ID │
 * │ 执行次数      │ childrenNum      │ 数字输入框，默认1，表示本套餐包含几次该子项  │
 * └──────────────┴──────────────────┴──────────────────────────────────────────────┘
 *
 * === 交互说明 ===
 * 1. 点击「+ 添加子项」按钮 → 弹出诊疗项目选择弹窗 → 调用 listTreatmentOptions 接口选择项目 →
 *    将选中项的id作为childrenId、name作为childrenName、默认childrenNum=1 加入children数组。
 * 2. childrenName 是创建组合项目时的子项名称快照，即使后续子项名称被修改，这里也不变。
 *    如果需要最新名称，可通过 childrenId 再次调用 get-treatment 接口获取。
 * 3. 点击子项行右侧红色「垃圾桶」图标 → 从数组中移除该子项。
 * 4. 执行次数 childrenNum：表示在一次套餐执行中，该子项目要执行多少次（如"输液套餐"包含2次"静脉输液"）。
 * 5. id 和 activityId 是系统字段，新增时不传（后端自动生成），编辑回显时有值，提交时原样传回即可。
 * 6. 单项目（非组合）时 children 返回空数组 []，前端不展示子项区域或展示"无子项"。
 *
 * @author j4组-treatment模块
 * @version 1.0.0
 */
@Data
@Schema(description = "诊疗项目子项显示对象（组合项目/套餐子项列表）")
public class WkfActivityDefinitionChildVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "子项记录主键ID（系统生成，新增不传，编辑回显有值，提交时原样传回）", example = "1890000000000000002")
    private String id;

    @Schema(description = "父项ID（外键，指向父诊疗项目id，系统字段，新增不传，提交时原样传回）", example = "1890000000000000001")
    private String activityId;

    @Schema(description = "【必填·隐藏字段】子项诊疗项目ID（被引用的诊疗项目主键，从选择弹窗回填）", example = "323001764504670208", requiredMode = Schema.RequiredMode.REQUIRED)
    private String childrenId;

    @Schema(description = "【数字输入框】执行次数，默认1，表示一次套餐中包含几次该子项", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer childrenNum;

    @Schema(description = "【只读文本】子项名称快照（创建时记录，可能与当前实际名称不一致）", example = "静脉注射")
    private String childrenName;
}
