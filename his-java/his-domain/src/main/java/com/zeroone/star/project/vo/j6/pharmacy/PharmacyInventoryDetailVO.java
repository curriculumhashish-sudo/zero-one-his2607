package com.zeroone.star.project.vo.j6.pharmacy;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

/**
 * <p>
 * 描述：药库盘点单据详情VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01星球
 * @version 1.0.0
 */
@Data
@Schema(description = "药库盘点单据详情")
public class PharmacyInventoryDetailVO {
    @Schema(description = "业务单号", example = "CST2026-07-190003")
    private String busNo;

    @Schema(description = "单据状态编码", example = "AGREE")
    private String statusEnum;

    @Schema(description = "目标库房ID")
    private String purposeLocationId;

    @Schema(description = "目标库房名称", example = "中药库")
    private String purposeLocationName;

    @Schema(description = "执行人ID")
    private String practitionerId;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请时间", example = "2026-07-19 20:57:08")
    private String applyTime;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批时间", example = "2026-07-19 20:58:02")
    private String approvalTime;

    @Schema(description = "盘点明细行列表")
    private List<PharmacyInventoryItemVO> lines;
}
