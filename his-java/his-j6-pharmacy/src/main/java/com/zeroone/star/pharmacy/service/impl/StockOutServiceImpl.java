package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
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

import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.UUID;

/**
 * 药库出库单服务实现
 */
@Service
@RequiredArgsConstructor
@Slf4j
public class StockOutServiceImpl implements IStockOutService {

    /** 出库单类型编码，与 StockOutApprovalServiceImpl 保持一致 */
    private static final String TYPE_WAREHOUSE_STOCKOUT = "WAREHOUSE_STOCKOUT";

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 1. 保存单据（新增 / 编辑） ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto) {
        validateLines(dto);

        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        if (!StringUtils.hasText(dto.getBusNo())) {
            // === 新增 ===
            String busNo = generateBusNo();
            writeLines(busNo, dto, STATUS_DRAFT, 0, now, userId, null);

            StockOutSaveResultVO result = new StockOutSaveResultVO();
            result.setBusNo(busNo);
            result.setStatus(STATUS_DRAFT);
            result.setMessage("新增保存成功");
            result.setVersion(1);
            return JsonVO.success(result);
        } else {
            // === 编辑 ===
            String busNo = dto.getBusNo();
            List<WkfSupplyRequest> oldRows = listActive(busNo);
            if (oldRows.isEmpty()) {
                return JsonVO.fail("单据不存在");
            }

            int nextVersion = oldRows.get(0).getVersion() + 1;
            softDeleteRows(busNo, now, userId);
            writeLines(busNo, dto, oldRows.get(0).getStatusEnum(), nextVersion, now, userId, null);

            StockOutSaveResultVO result = new StockOutSaveResultVO();
            result.setBusNo(busNo);
            result.setStatus(oldRows.get(0).getStatusEnum());
            result.setMessage("保存成功");
            result.setVersion(nextVersion);
            return JsonVO.success(result);
        }
    }

    // ==================== 2. 保存并提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(StockOutSaveDTO dto) {
        validateLines(dto);

        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        if (!StringUtils.hasText(dto.getBusNo())) {
            // 新增 + 直接提交
            String busNo = generateBusNo();
            writeLines(busNo, dto, STATUS_PENDING_APPROVAL, 0, now, userId, userId);

            StockOutApprovalResultVO result = new StockOutApprovalResultVO();
            result.setSuccess(true);
            result.setStatusEnum(STATUS_PENDING_APPROVAL);
            return JsonVO.success(result);
        } else {
            // 编辑已有单据 + 提交
            String busNo = dto.getBusNo();
            List<WkfSupplyRequest> oldRows = listActive(busNo);
            if (oldRows.isEmpty()) {
                return JsonVO.fail("单据不存在");
            }
            if (!STATUS_DRAFT.equals(oldRows.get(0).getStatusEnum())) {
                return JsonVO.fail("当前状态不允许提交审批，当前状态: " + oldRows.get(0).getStatusEnum());
            }

            int nextVersion = oldRows.get(0).getVersion() + 1;
            softDeleteRows(busNo, now, userId);
            writeLines(busNo, dto, STATUS_PENDING_APPROVAL, nextVersion, now, userId, userId);

            StockOutApprovalResultVO result = new StockOutApprovalResultVO();
            result.setSuccess(true);
            result.setStatusEnum(STATUS_PENDING_APPROVAL);
            return JsonVO.success(result);
        }
    }

    // ==================== 3. 单独提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto) {
        List<WkfSupplyRequest> rows = listActive(dto.getBusNo());
        if (rows.isEmpty()) {
            return JsonVO.fail("单据不存在");
        }
        if (!STATUS_DRAFT.equals(rows.get(0).getStatusEnum())) {
            return JsonVO.fail("当前状态不允许提交审批");
        }

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        for (WkfSupplyRequest row : rows) {
            WkfSupplyRequest update = new WkfSupplyRequest();
            update.setId(row.getId());
            update.setStatusEnum(STATUS_PENDING_APPROVAL);
            update.setApplicantId(userId);
            update.setApplyTime(now);
            update.setApproverId(userId);
            update.setUpdatedAt(now);
            update.setUpdatedBy(userId);
            wkfSupplyRequestMapper.updateById(update);
        }

        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);
        result.setStatusEnum(STATUS_PENDING_APPROVAL);
        return JsonVO.success(result);
    }

    /**
     * 删除草稿单据（逻辑删除）
     * @param busNo 单据编号（bus_no）
     * @return 是否成功
     */
    @Override
    public String deleteDraftByBusNo(String busNo) {
        // 1. 查活跃行（排除已软删）
        List<WkfSupplyRequest> rows = listActive(busNo);
        if (rows.isEmpty()) {
            return "单据不存在或已删除";
        }
        String currentStatus = rows.get(0).getStatusEnum();
        // 2. 只允许 DRAFT / WITHDRAW / REJECT 状态删除
        if (!STATUS_DRAFT.equals(currentStatus)
                && !"WITHDRAW".equals(currentStatus)
                && !"REJECT".equals(currentStatus)) {
            return "当前状态不允许删除，当前状态: " + currentStatus;
        }
        // 3. 批量软删除
        String userId = currentUserId();
        softDeleteRows(busNo, LocalDateTime.now(), userId);
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

    // ==================== 出库单 CRUD 辅助方法 ====================

    private void validateLines(StockOutSaveDTO dto) {
        if (dto == null || dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("出库单明细不能为空");
        }
        for (int i = 0; i < dto.getLines().size(); i++) {
            StockOutSaveDTO.StockOutLineDTO line = dto.getLines().get(i);
            if (line == null) throw new IllegalArgumentException("第" + (i + 1) + "行明细不能为空");
            if (line.getItemQuantity() == null || line.getItemQuantity().compareTo(java.math.BigDecimal.ZERO) <= 0)
                throw new IllegalArgumentException("第" + (i + 1) + "行数量必须大于0");
            if (!StringUtils.hasText(line.getItemTable()))
                throw new IllegalArgumentException("第" + (i + 1) + "行物品表名不能为空");
            if (line.getItemId() == null)
                throw new IllegalArgumentException("第" + (i + 1) + "行物品ID不能为空");
            if (!StringUtils.hasText(line.getUnitCode()))
                throw new IllegalArgumentException("第" + (i + 1) + "行单位编码不能为空");
        }
    }

    private void writeLines(String busNo, StockOutSaveDTO dto, String status, int version,
                             LocalDateTime now, String userId, String approverId) {
        for (StockOutSaveDTO.StockOutLineDTO line : dto.getLines()) {
            WkfSupplyRequest row = new WkfSupplyRequest();
            row.setId(java.util.UUID.randomUUID().toString().replace("-", ""));
            row.setBusNo(busNo);
            row.setTypeEnum(TYPE_WAREHOUSE_STOCKOUT);
            row.setStatusEnum(status);
            row.setCategoryEnum("STOCK_OUT");
            row.setSourceTypeEnum("WAREHOUSE");
            row.setSourceLocationId(String.valueOf(dto.getSourceLocationId()));
            row.setPurposeTypeEnum("PHARMACY");
            row.setPurposeLocationId(String.valueOf(dto.getPurposeLocationId()));
            row.setPractitionerId(String.valueOf(dto.getPractitionerId()));
            row.setItemTable(line.getItemTable());
            row.setItemId(String.valueOf(line.getItemId()));
            row.setItemQuantity(line.getItemQuantity());
            row.setUnitCode(line.getUnitCode());
            row.setLotNumber(line.getLotNumber());
            row.setPrice(line.getPrice() != null ? line.getPrice() : java.math.BigDecimal.ZERO);
            row.setTotalPrice(line.getPrice() != null
                    ? line.getPrice().multiply(line.getItemQuantity()) : java.math.BigDecimal.ZERO);
            row.setOccurrenceTime(now);
            row.setOriginalBusNo(dto.getRelatedReqNo());
            row.setRemake(dto.getRemark());
            row.setApplicantId(userId);
            row.setApproverId(approverId);
            row.setCreatedAt(now);
            row.setCreatedBy(userId);
            row.setUpdatedAt(now);
            row.setUpdatedBy(userId);
            row.setIsDeleted(false);
            row.setVersion(version);
            wkfSupplyRequestMapper.insert(row);
        }
    }

    private List<WkfSupplyRequest> listActive(String busNo) {
        QueryWrapper<WkfSupplyRequest> wrapper = new QueryWrapper<>();
        wrapper.eq("bus_no", busNo)
               .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
               .eq("is_deleted", 0);
        return wkfSupplyRequestMapper.selectList(wrapper);
    }

    private void softDeleteRows(String busNo, LocalDateTime now, String userId) {
        UpdateWrapper<WkfSupplyRequest> wrapper = new UpdateWrapper<>();
        wrapper.eq("bus_no", busNo)
               .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
               .eq("is_deleted", 0)
               .set("is_deleted", 1)
               .set("deleted_at", now)
               .set("deleted_by", userId)
               .set("updated_at", now)
               .set("updated_by", userId);
        wkfSupplyRequestMapper.update(null, wrapper);
    }

    private String generateBusNo() {
        String datePart = LocalDate.now().format(DateTimeFormatter.ofPattern("yyyyMMdd"));
        int rand = java.util.concurrent.ThreadLocalRandom.current().nextInt(1000, 10000);
        return "CSO" + datePart + String.format("%04d", rand);
    }

    private String currentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            if (user != null && StringUtils.hasText(user.getId())) return user.getId();
        } catch (Exception ignored) {}
        return "system";
    }
}