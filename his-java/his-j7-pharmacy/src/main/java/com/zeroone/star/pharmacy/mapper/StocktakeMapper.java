package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeItemVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：盘点单 Mapper 接口，基于 {@code wkf_supply_request} 表
 * </p>
 * <p>
 * 功能说明：提供盘点单的查询、明细查询及状态更新能力：
 * 1. 分页查询盘点单列表：按 {@code bus_no} 聚合，关联库房、申请人、审批人
 * 2. 查询盘点单明细：根据 {@code bus_no} 查询所有明细行，关联药品定义
 * 3. 查询盘点单聚合信息：根据 {@code bus_no} 查询汇总信息（总盈亏金额、明细条数等）
 * 4. 校验盘点单是否存在：用于提交审批前的业务校验
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Mapper
public interface StocktakeMapper extends BaseMapper<WkfSupplyRequest> {

    /**
     * 分页查询盘点单列表（按 bus_no 聚合）
     * @param page         分页对象
     * @param busNo        业务单号（模糊查询）
     * @param statusEnum   状态枚举
     * @param warehouseId  库房标识（purpose_location_id）
     * @param warehouseName 库房/盘点药房名称（模糊查询）
     * @param applicantId  申请人标识
     * @param startTime    创建时间起
     * @param endTime      创建时间止
     * @return 分页结果
     */
    IPage<StocktakeListVO> selectStocktakePage(IPage<StocktakeListVO> page,
                                               @Param("busNo") String busNo,
                                               @Param("statusEnum") String statusEnum,
                                               @Param("warehouseId") String warehouseId,
                                               @Param("warehouseName") String warehouseName,
                                               @Param("applicantId") String applicantId,
                                               @Param("startTime") String startTime,
                                               @Param("endTime") String endTime);

    /**
     * 根据盘点单号查询汇总信息（一行聚合）
     * @param busNo 盘点单号
     * @return 盘点单详情（不含明细行）
     */
    StocktakeDetailVO selectStocktakeDetail(@Param("busNo") String busNo);

    /**
     * 根据盘点单号查询所有明细行
     * @param busNo 盘点单号
     * @return 明细列表
     */
    List<StocktakeItemVO> selectStocktakeItems(@Param("busNo") String busNo);

    /**
     * 根据盘点单号查询该单据的任意一行（用于校验单据存在性、类型、状态等）
     * @param busNo 盘点单号
     * @return 任意一行记录（同一单据号共享审批状态）
     */
    WkfSupplyRequest selectOneByBusNo(@Param("busNo") String busNo);

    /**
     * 将库房标识归一为 adm_organization 的 ID：
     * 兼容前端传库房 ID 或库房名称（如"门诊西药房"）两种情况
     * @param warehouse 库房 ID 或库房名称
     * @return 机构 ID；不存在时返回 null
     */
    String resolveLocationId(@Param("warehouse") String warehouse);

    /**
     * 查询药品定义的库存单位编码（unit_code 缺失时的回填兜底来源）
     * @param itemId 药品定义标识
     * @return 库存单位编码；不存在时返回 null
     */
    String findStockUnitCodeByItemId(@Param("itemId") String itemId);

    /**
     * 统计某盘点单的明细行数
     * @param busNo 盘点单号
     * @return 明细行数
     */
    Long countItemsByBusNo(@Param("busNo") String busNo);

    /**
     * 提交审批：批量更新同一 bus_no 下所有行的状态、申请人、申请时间
     * @param busNo        盘点单号
     * @param statusEnum   目标状态
     * @param applicantId  申请人标识
     * @param applyTime    申请时间
     * @param updatedBy    更新人
     * @param updatedAt    更新时间
     * @return 受影响行数
     */
    int updateStatusByBusNo(@Param("busNo") String busNo,
                            @Param("statusEnum") String statusEnum,
                            @Param("applicantId") String applicantId,
                            @Param("applyTime") String applyTime,
                            @Param("updatedBy") String updatedBy,
                            @Param("updatedAt") String updatedAt,
                            @Param("remark") String remark);

    /**
     * 根据盘点单号软删除所有明细行
     * @param busNo      盘点单号
     * @param deletedBy  删除人
     * @param deletedAt  删除时间
     * @return 受影响行数
     */
    int softDeleteByBusNo(@Param("busNo") String busNo,
                          @Param("deletedBy") String deletedBy,
                          @Param("deletedAt") String deletedAt);
}
