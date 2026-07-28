package com.zeroone.star.j7.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j7.pharmacy.entity.approval.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 工作流 supply request Mapper 接口
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    /**
     * 根据聚合单号查询所有盘点单明细列表
     */
    List<WkfSupplyRequest> getPharmacyStockTakeList(@Param("busNo") String busNo);

    /**
     * 提交审批：更新状态为待审批，写入申请人和申请时间
     */
    void updateStatusForSubmit(@Param("statusEnum") String statusEnum,
                               @Param("applicantId") String applicantId,
                               @Param("applyTime") LocalDateTime applyTime,
                               @Param("busNo") String busNo);

    /**
     * 审批通过/驳回：更新审批状态，写入审批人和审批时间
     */
    void updateStatusForApprove(@Param("statusEnum") String statusEnum,
                                @Param("approverId") String approverId,
                                @Param("approvalTime") LocalDateTime approvalTime,
                                @Param("busNo") String busNo);

    /**
     * 撤回审批：回退状态为草稿，清空申请人和申请时间
     */
    void updateStatusForWithdraw(@Param("statusEnum") String statusEnum,
                                  @Param("busNo") String busNo);
}
