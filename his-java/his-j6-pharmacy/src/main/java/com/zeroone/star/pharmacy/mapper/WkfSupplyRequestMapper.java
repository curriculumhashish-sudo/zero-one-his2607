package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailLineVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutLineVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

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
                                 @Param("approverId") String approverId,
                                 @Param("applyTime") LocalDateTime applyTime,
                                 @Param("updatedAt") LocalDateTime updatedAt,
                                 @Param("updatedBy") String updatedBy);

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

    /**
     * 按 bus_no 去重分页查询单据列表（每个单据只返回一条记录）
     * @param page 分页参数
     * @param query 查询条件（keyword, status, time range）
     * @return 分页结果
     */
    IPage<StockOutListVO> selectDistinctBusNoPage(Page<StockOutListVO> page,
                                                    @Param("query") StockOutQuery query);

    /**
     * 根据单据编号查询所有明细行（已关联药品信息）
     */
    List<StockOutLineVO> selectDetailLinesByBusNo(@Param("busNo") String busNo);

    // ==================== 业务查询与操作 ====================

    /**
     * 分页查询入库单列表（按 bus_no 聚合）
     */
    IPage<InboundReceiptListVO> selectInboundReceiptListPage(Page<InboundReceiptListVO> page,
                                                             @Param("query") InboundReceiptQuery query);

    /**
     * 根据 busNo 查询入库单明细行（含物料名称/规格/厂商）
     */
    List<InboundReceiptDetailLineVO> selectInboundReceiptDetailLines(@Param("busNo") String busNo);

    /**
     * 逻辑删除指定入库单（按 bus_no 软删）
     */
    int deleteInboundByBusNo(@Param("busNo") String busNo,
                             @Param("deletedAt") LocalDateTime deletedAt,
                             @Param("deletedBy") String deletedBy);
}