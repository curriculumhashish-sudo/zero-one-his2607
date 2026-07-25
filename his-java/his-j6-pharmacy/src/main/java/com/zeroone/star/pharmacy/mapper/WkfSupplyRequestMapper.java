package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 工作流 supply request Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {
    /**
     * 查询药库盘点单审批所需行数据。
     *
     * @param busNo    盘点单号
     * @param typeEnum 单据类型
     * @return 盘点单明细行
     */
    List<WkfSupplyRequest> selectStocktakeRowsForApproval(@Param("busNo") String busNo,
                                                          @Param("typeEnum") String typeEnum);

    int updateStocktakeToPending(@Param("busNo") String busNo,
                                 @Param("typeEnum") String typeEnum,
                                 @Param("currentStatus") String currentStatus,
                                 @Param("applicantId") String applicantId,
                                 @Param("applyTime") LocalDateTime applyTime,
                                 @Param("updatedAt") LocalDateTime updatedAt,
                                 @Param("updatedBy") String updatedBy);

    int updateStocktakeToWithdraw(@Param("busNo") String busNo,
                                  @Param("typeEnum") String typeEnum,
                                  @Param("currentStatus") String currentStatus,
                                  @Param("updatedAt") LocalDateTime updatedAt,
                                  @Param("updatedBy") String updatedBy);

    int updateStocktakeToAgree(@Param("busNo") String busNo,
                               @Param("typeEnum") String typeEnum,
                               @Param("currentStatus") String currentStatus,
                               @Param("approverId") String approverId,
                               @Param("approvalTime") LocalDateTime approvalTime,
                               @Param("updatedAt") LocalDateTime updatedAt,
                               @Param("updatedBy") String updatedBy);

    int updateStocktakeToReject(@Param("busNo") String busNo,
                                @Param("typeEnum") String typeEnum,
                                @Param("currentStatus") String currentStatus,
                                @Param("approverId") String approverId,
                                @Param("approvalTime") LocalDateTime approvalTime,
                                @Param("reason") String reason,
                                @Param("updatedAt") LocalDateTime updatedAt,
                                @Param("updatedBy") String updatedBy);
}
