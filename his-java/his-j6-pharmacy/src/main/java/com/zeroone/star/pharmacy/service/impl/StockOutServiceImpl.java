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
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.*;
import jakarta.annotation.Resource;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
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
@Slf4j
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

    @Resource
    private UserHolder userHolder;

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
        log.info("开始执行逻辑删除，busNo: {}", busNo);
        // 1.使用 LambdaQueryWrapper 根据 busNo 查询（取第一条即可）
        LambdaQueryWrapper<WkfSupplyRequest> wrapper1 = Wrappers.lambdaQuery();
        wrapper1.eq(WkfSupplyRequest::getBusNo, busNo)
                .last("LIMIT 1");  // 只查一条，提高效率

        WkfSupplyRequest entity = wkfSupplyRequestMapper.selectOne(wrapper1);
        if (entity == null) {
            log.warn("单据不存在，busNo: {}", busNo);
            return "单据不存在";
        }
        // 2. 判断是否已删除
        if (Objects.equals(entity.getIsDeleted(), 1)) {
            log.warn("单据已被删除，busNo: {}", busNo);
            return "该单据已被删除";
        }
        // 3. 判断是否草稿状态（假设 0 为草稿）
        if (!Objects.equals(entity.getStatusEnum(), STATUS_DRAFT)) {
            log.warn("单据状态非草稿，无法删除，busNo: {}, status: {}", busNo, entity.getStatusEnum());
            return "该单据不是草稿状态，无法删除";
        }
        //在外面声明变量，供后面使用
        UserDTO userDTO = null;
        try {
            userDTO = userHolder.getCurrentUser();
            log.info("获取当前用户成功: {}", userDTO.getUsername());
        } catch (Exception e) {
            log.error("获取当前登录用户失败，busNo: {}", busNo, e);
            // 抛出异常中断流程，或者根据业务返回特定错误码
            throw new RuntimeException("用户未登录，无法执行删除操作");
        }
        log.info("成功！");
        // 4. 执行逻辑删除
        LambdaUpdateWrapper<WkfSupplyRequest> wrapper = Wrappers.lambdaUpdate();
        wrapper.eq(WkfSupplyRequest::getBusNo, busNo)
                .eq(WkfSupplyRequest::getStatusEnum, STATUS_DRAFT)
                .set(WkfSupplyRequest::getIsDeleted, 1)
                .set(WkfSupplyRequest::getDeletedAt, LocalDateTime.now())
                //这里现在可以正确引用外面定义的 userDTO 了
                .set(WkfSupplyRequest::getDeletedBy, userDTO.getUsername());

        int affected = wkfSupplyRequestMapper.update(null, wrapper);
        if (affected == 0) {
            log.warn("逻辑删除影响行数为0，可能状态已变，busNo: {}", busNo);
            return "删除失败，单据状态可能已变化";
        }
        log.info("逻辑删除成功，busNo: {}, deletedBy: {}", busNo, userDTO.getUsername());
        return "success";
    }

    /**
     * 获取单据详情
     * @param busNo 单据编号（bus_no），用于查询该单据下的所有明细行
     * @return 单据详情VO
     */
    @Override
    public StockOutDetailVO getDetailByBusNo(String busNo) {
        log.info("查询单据详情，busNo: {}", busNo);
        // 1. 查询一条主记录（取任意一行，头信息相同）
        LambdaQueryWrapper<WkfSupplyRequest> headerWrapper = new LambdaQueryWrapper<>();
        headerWrapper.eq(WkfSupplyRequest::getBusNo, busNo)
                .last("LIMIT 1");
        WkfSupplyRequest header = wkfSupplyRequestMapper.selectOne(headerWrapper);
        if (header == null) {
            log.warn("单据主记录不存在，busNo: {}", busNo);
            return null;
        }
        log.debug("主记录查询成功，id: {}", header.getId());

        // 2. 查询所有明细行（已关联药品信息）
        List<StockOutLineVO> lineList = wkfSupplyRequestMapper.selectDetailLinesByBusNo(busNo);
        log.info("明细行数量: {}，busNo: {}", lineList.size(), busNo);

        // 3. 组装 VO
        StockOutDetailVO vo = new StockOutDetailVO();
        BeanUtils.copyProperties(header, vo);
        vo.setLines(lineList);
        log.info("单据详情组装完成，busNo: {}", busNo);
        return vo;
    }

    /**
     * 获取单据列表（条件+分页）
     * @param query 查询条件
     * @return 分页结果
     */
    @Override
    public PageDTO<StockOutListVO> getPharmacyList(StockOutQuery query) {
        log.info("查询单据列表，pageIndex: {}, pageSize: {}, query: {}",
                query.getPageIndex(), query.getPageSize(), query);
        // 1. 构建分页对象（注意：这里的 page 用于自定义分页）
        Page<WkfSupplyRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // 2. 直接执行自定义去重分页查询（条件全部在 XML 中处理）
        IPage<WkfSupplyRequest> resultPage = wkfSupplyRequestMapper.selectDistinctBusNoPage(page, query);
        log.info("分页查询结果，总记录数: {}, 当前页记录数: {}",
                resultPage.getTotal(), resultPage.getRecords().size());

        // 3. 使用工具类直接转换为 PageDTO<StockOutListVO>
        //    第二个参数 StockOutListVO::new 是构造器引用，内部会用 BeanUtil.copyProperties 复制属性
        log.debug("PageDTO转换完成");
        return PageDTO.create((Page<WkfSupplyRequest>) resultPage, StockOutListVO::new);
    }
}