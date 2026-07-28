package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import java.time.LocalDateTime;
import java.util.List;

/**
 * Mapper for workflow supply request records.
 */
public interface WkfSupplyRequestMapper extends BaseMapper<WkfSupplyRequest> {

    /**
     * Query active inbound rows by business number for draft save validation.
     */
    @Select("""
            SELECT id, bus_no, status_enum, version, is_deleted
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    List<WkfSupplyRequest> selectActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * Count active inbound rows by business number.
     */
    @Select("""
            SELECT COUNT(1)
            FROM wkf_supply_request
            WHERE bus_no = #{busNo}
              AND is_deleted = 0
            """)
    long countActiveRowsByBusNo(@Param("busNo") String busNo);

    /**
     * Soft delete active rows before rebuilding receipt details.
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
    int updateActiveRowsDeletedByBusNo(@Param("busNo") String busNo,
                                       @Param("deletedAt") LocalDateTime deletedAt,
                                       @Param("deletedBy") String deletedBy,
                                       @Param("updatedAt") LocalDateTime updatedAt,
                                       @Param("updatedBy") String updatedBy);

    /**
     * Query inbound rows required for approval by business number and type.
     */
    List<WkfSupplyRequest> selectInboundRowsForApproval(@Param("busNo") String busNo,
                                                        @Param("typeEnum") String typeEnum);

    /**
     * Submit inbound receipt for approval.
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
     * Withdraw inbound approval.
     */
    int updateInboundToWithdraw(@Param("busNo") String busNo,
                                @Param("typeEnum") String typeEnum,
                                @Param("currentStatus") String currentStatus,
                                @Param("updatedAt") LocalDateTime updatedAt,
                                @Param("updatedBy") String updatedBy);

    /**
     * Approve inbound receipt.
     */
    int updateInboundToAgree(@Param("busNo") String busNo,
                             @Param("typeEnum") String typeEnum,
                             @Param("currentStatus") String currentStatus,
                             @Param("approverId") String approverId,
                             @Param("approvalTime") LocalDateTime approvalTime,
                             @Param("updatedAt") LocalDateTime updatedAt,
                             @Param("updatedBy") String updatedBy);

    /**
     * Reject inbound receipt.
     */
    int updateInboundToReject(@Param("busNo") String busNo,
                              @Param("typeEnum") String typeEnum,
                              @Param("currentStatus") String currentStatus,
                              @Param("approverId") String approverId,
                              @Param("approvalTime") LocalDateTime approvalTime,
                              @Param("reason") String reason,
                              @Param("updatedAt") LocalDateTime updatedAt,
                              @Param("updatedBy") String updatedBy);
}
