package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Wrappers;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStockOutService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.*;
import jakarta.annotation.Resource;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

/**
 * 药库出库单服务实现
 */
@Service
@RequiredArgsConstructor
public class StockOutServiceImpl implements IStockOutService {

    // 状态常量
    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_APPROVED = "APPROVED";
    private static final String STATUS_REJECTED = "REJECTED";

    // TODO: 后续注入 Mapper
    // private final StockOutMapper stockOutMapper;

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Override
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(StockOutSaveDTO dto) {
        // 第一步：保存出库单
        StockOutSaveResultVO saveResult = saveStockOut(dto).getData();

        // 第二步：提交审批
        SubmitStockOutApprovalDTO approvalDTO = new SubmitStockOutApprovalDTO();
        approvalDTO.setBusNo(saveResult.getBusNo());
        // approvalDTO.setApproverId(...); // 如果需要审批人ID

        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);
        result.setStatusEnum("APPROVAL");

        return JsonVO.success(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto) {
        StockOutSaveResultVO result = new StockOutSaveResultVO();
        //result.setBusNo(generateBusNo());  // 只设置 busNo
        // 其他字段不设置，默认为 null

        return JsonVO.success(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto) {
        StockOutApprovalResultVO result = new StockOutApprovalResultVO();

        // TODO: 业务验证
        // 1. 检查单据是否存在
        // 2. 检查当前状态是否允许提交审批
        // 3. 检查明细行是否完整

        // TODO: 更新数据库状态
        // stockOutMapper.updateStatus(dto.getBusNo(), STATUS_PENDING_APPROVAL);

        result.setSuccess(true);
        result.setStatusEnum("APPROVAL");

        return JsonVO.success(result);
    }

    /**
     * 删除草稿单据（逻辑删除）
     * @param busNo 单据编号（bus_no）
     * @return 是否成功
     */
    @Override
    public String deleteDraftByBusNo(String busNo) {
        // 1.使用 LambdaQueryWrapper 根据 busNo 查询（取第一条即可）
        LambdaQueryWrapper<WkfSupplyRequest> wrapper1 = Wrappers.lambdaQuery();
        wrapper1.eq(WkfSupplyRequest::getBusNo, busNo)
                .last("LIMIT 1");  // 只查一条，提高效率

        WkfSupplyRequest entity = wkfSupplyRequestMapper.selectOne(wrapper1);
        if (entity == null) {
            return "单据不存在";
        }
        // 2. 判断是否已删除
        if (Objects.equals(entity.getIsDeleted(), 1)) {
            return "该单据已被删除";
        }
        // 3. 判断是否草稿状态（假设 0 为草稿）
        if (!Objects.equals(entity.getStatusEnum(), STATUS_DRAFT)) {
            return "该单据不是草稿状态，无法删除";
        }
        // 4. 执行逻辑删除
        LambdaUpdateWrapper<WkfSupplyRequest> wrapper = Wrappers.lambdaUpdate();
        wrapper.eq(WkfSupplyRequest::getBusNo, busNo)
                .set(WkfSupplyRequest::getIsDeleted, 1)
                .set(WkfSupplyRequest::getDeletedAt, LocalDateTime.now());
                //.set(WkfSupplyRequest::getDeletedBy, getCurrentUsername());
        wkfSupplyRequestMapper.update(null, wrapper);
        return "success";
    }

    /**
     * 获取单据详情
     * @param busNo 单据编号（bus_no），用于查询该单据下的所有明细行
     * @return 单据详情VO
     */
    @Override
    public StockOutDetailVO getDetailByBusNo(String busNo) {
        // 1. 查询一条主记录（取任意一行，头信息相同）
        LambdaQueryWrapper<WkfSupplyRequest> headerWrapper = new LambdaQueryWrapper<>();
        headerWrapper.eq(WkfSupplyRequest::getBusNo, busNo)
                .last("LIMIT 1");
        WkfSupplyRequest header = wkfSupplyRequestMapper.selectOne(headerWrapper);
        if (header == null) {
            return null;
        }

        // 2. 查询所有明细行（已关联药品信息）
        List<StockOutLineVO> lineList = wkfSupplyRequestMapper.selectDetailLinesByBusNo(busNo);

        // 3. 组装 VO
        StockOutDetailVO vo = new StockOutDetailVO();
        BeanUtils.copyProperties(header, vo);
        vo.setLines(lineList);
        return vo;
    }

    /**
     * 获取单据列表（条件+分页）
     * @param query 查询条件
     * @return 分页结果
     */
    @Override
    public PageDTO<StockOutListVO> getPharmacyList(StockOutQuery query) {
        // 1. 构建分页对象（注意：这里的 page 用于自定义分页）
        Page<WkfSupplyRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // 2. 直接执行自定义去重分页查询（条件全部在 XML 中处理）
        IPage<WkfSupplyRequest> resultPage = wkfSupplyRequestMapper.selectDistinctBusNoPage(page, query);

        // 3. 使用工具类直接转换为 PageDTO<StockOutListVO>
        //    第二个参数 StockOutListVO::new 是构造器引用，内部会用 BeanUtil.copyProperties 复制属性
        return PageDTO.create((Page<WkfSupplyRequest>) resultPage, StockOutListVO::new);
    }
}