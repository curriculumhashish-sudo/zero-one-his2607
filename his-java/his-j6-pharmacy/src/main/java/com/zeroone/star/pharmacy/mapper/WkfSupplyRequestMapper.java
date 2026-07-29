package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailLineVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import org.apache.ibatis.annotations.Param;

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
