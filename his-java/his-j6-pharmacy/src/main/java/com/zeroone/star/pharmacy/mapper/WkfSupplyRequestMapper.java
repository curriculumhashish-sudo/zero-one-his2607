package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;
<<<<<<< HEAD
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;
=======
>>>>>>> 776c6b3dd9aed85a0b73156f0f483a61b598ba98

import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>工作流 supply request Mapper 接口</p>
 *
 * @author AB
 * @since 2026-07-27
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    /**
<<<<<<< HEAD
     * 查询指定业务单号下未删除的进货单行。
     * 仅查询写操作校验所需字段，避免 select *。
     *
     * @param busNo 业务单号
     * @return 单据行
     */
    @Select("""
            SELECT id, bus_no, status_enum, version, is_deleted
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    List<WkfSupplyRequest> selectActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * 统计指定业务单号下未删除的进货单行数。
     *
     * @param busNo 业务单号
     * @return 行数
     */
    @Select("""
            SELECT COUNT(1)
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    long countActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * 软删除指定业务单号下未删除的进货单行。
     *
     * @param busNo 业务单号
     * @param deletedAt 删除时间
     * @param deletedBy 删除人
     * @param updatedAt 更新时间
     * @param updatedBy 更新人
     * @return 影响行数
     */
    @Update("""
            UPDATE wkf_supply_request
            SET is_deleted = 1,
                deleted_at = #{deletedAt},
                deleted_by = #{deletedBy},
                updated_at = #{updatedAt},
                updated_by = #{updatedBy}
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    int softDeleteActiveRowsByBusNo(@Param("busNo") String busNo,
                                    @Param("deletedAt") LocalDateTime deletedAt,
                                    @Param("deletedBy") String deletedBy,
                                    @Param("updatedAt") LocalDateTime updatedAt,
                                    @Param("updatedBy") String updatedBy);
=======
     * 查询进货单审批所需行数据（按 busNo + typeEnum，排除已删行）
     */
    List<WkfSupplyRequest> selectInboundRowsForApproval(@Param("busNo") String busNo,
                                                         @Param("typeEnum") String typeEnum);

    /**
     * 提交审批：status_enum → PENDING_APPROVAL，写入审批人和申请人
     */
    int updateInboundToPending(@Param("busNo") String busNo,
                               @Param("typeEnum") String typeEnum,
                               @Param("currentStatus") String currentStatus,
                               @Param("approverId") String approverId,
                               @Param("applicantId") String applicantId,
                               @Param("applyTime") LocalDateTime applyTime,
                               @Param("updatedAt") LocalDateTime updatedAt,
                               @Param("updatedBy") String updatedBy);

    /**
     * 撤回审批：status_enum → WITHDRAW，清除审批信息
     */
    int updateInboundToWithdraw(@Param("busNo") String busNo,
                                @Param("typeEnum") String typeEnum,
                                @Param("currentStatus") String currentStatus,
                                @Param("updatedAt") LocalDateTime updatedAt,
                                @Param("updatedBy") String updatedBy);

    /**
     * 审批通过：status_enum → AGREE，写入审批人及时间
     */
    int updateInboundToAgree(@Param("busNo") String busNo,
                             @Param("typeEnum") String typeEnum,
                             @Param("currentStatus") String currentStatus,
                             @Param("approverId") String approverId,
                             @Param("approvalTime") LocalDateTime approvalTime,
                             @Param("updatedAt") LocalDateTime updatedAt,
                             @Param("updatedBy") String updatedBy);

    /**
     * 驳回审批：status_enum → REJECT，写入审批人、时间及驳回原因
     */
    int updateInboundToReject(@Param("busNo") String busNo,
                              @Param("typeEnum") String typeEnum,
                              @Param("currentStatus") String currentStatus,
                              @Param("approverId") String approverId,
                              @Param("approvalTime") LocalDateTime approvalTime,
                              @Param("reason") String reason,
                              @Param("updatedAt") LocalDateTime updatedAt,
                              @Param("updatedBy") String updatedBy);
>>>>>>> 776c6b3dd9aed85a0b73156f0f483a61b598ba98
}
