package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.AdmLocation;
import com.zeroone.star.pharmacy.entity.AdmPractitioner;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.AdmLocationMapper;
import com.zeroone.star.pharmacy.mapper.AdmPractitionerMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStockOutService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveRequestDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.*;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

/**
 * 药库出库单服务实现
 */
@Service
@Slf4j
public class StockOutServiceImpl implements IStockOutService {

    /** 出库单类型编码 */
    private static final String TYPE_WAREHOUSE_STOCKOUT = "WAREHOUSE_STOCKOUT";

    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_APPROVAL = "APPROVAL";

    private static final String DEFAULT_TENANT_ID = "1";

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private AdmLocationMapper admLocationMapper;

    @Resource
    private AdmPractitionerMapper admPractitionerMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 1. 新增空白草稿 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StockOutDetailVO addStockOut(StockOutAddDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("新增出库单入参不能为空");
        }
        if (!StringUtils.hasText(dto.getSourceLocationId())) {
            throw new IllegalArgumentException("源药库不能为空");
        }
        if (!StringUtils.hasText(dto.getPurposeLocationId())) {
            throw new IllegalArgumentException("目的药房不能为空");
        }

        String busNo = generateBusNo();
        String userId = currentUserId();
        String practitionerId = StringUtils.hasText(dto.getPractitionerId())
                ? dto.getPractitionerId() : userId;
        LocalDateTime now = LocalDateTime.now();

        // 插入锚点行
        WkfSupplyRequest anchor = buildAnchorRow(busNo, dto, practitionerId, now, userId);
        wkfSupplyRequestMapper.insert(anchor);

        return buildEnrichedDetailVO(anchor, new ArrayList<>());
    }

    // ==================== 2. 保存草稿 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StockOutDetailVO saveStockOut(StockOutSaveRequestDTO dto) {
        validateSaveRequest(dto);
        String busNo = requireBusNo(dto);
        List<WkfSupplyRequest> oldRows = listActive(busNo);
        if (oldRows.isEmpty()) {
            throw new IllegalArgumentException("单据不存在: " + busNo);
        }
        int nextVersion = oldRows.get(0).getVersion() + 1;
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        softDeleteRows(busNo, now, userId);
        List<WkfSupplyRequest> newRows = buildRowsFromSaveRequest(dto, busNo,
                STATUS_PENDING_APPROVAL, nextVersion, now, userId, null);
        for (WkfSupplyRequest row : newRows) {
            wkfSupplyRequestMapper.insert(row);
        }
        return buildEnrichedDetailVO(newRows.get(0), convertRowsToLines(newRows));
    }

    // ==================== 3. 保存并提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StockOutApprovalResultVO saveAndSubmitApproval(StockOutSaveRequestDTO dto) {
        validateSaveRequest(dto);

        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        if (!StringUtils.hasText(dto.getBusNo())) {
            // 新增 + 直接提交
            String busNo = generateBusNo();
            String practitionerId = StringUtils.hasText(dto.getPractitionerId())
                    ? dto.getPractitionerId() : userId;
            List<WkfSupplyRequest> newRows = buildRowsFromSaveRequest(dto, busNo,
                    STATUS_APPROVAL, 0, now, userId, userId);
            for (WkfSupplyRequest row : newRows) {
                wkfSupplyRequestMapper.insert(row);
            }
        } else {
            // 编辑已有单据 + 提交
            String busNo = dto.getBusNo();
            List<WkfSupplyRequest> oldRows = listActive(busNo);
            if (oldRows.isEmpty()) {
                throw new IllegalArgumentException("单据不存在: " + busNo);
            }
            if (!STATUS_PENDING_APPROVAL.equals(oldRows.get(0).getStatusEnum())) {
                throw new IllegalArgumentException("当前状态不允许提交审批，当前状态: "
                        + oldRows.get(0).getStatusEnum());
            }
            int nextVersion = oldRows.get(0).getVersion() + 1;
            softDeleteRows(busNo, now, userId);
            List<WkfSupplyRequest> newRows = buildRowsFromSaveRequest(dto, busNo,
                    STATUS_APPROVAL, nextVersion, now, userId, userId);
            for (WkfSupplyRequest row : newRows) {
                wkfSupplyRequestMapper.insert(row);
            }
        }

        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);
        result.setStatusEnum(STATUS_APPROVAL);
        return result;
    }

    // ==================== 4. 单独提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto) {
        List<WkfSupplyRequest> rows = listActive(dto.getBusNo());
        if (rows.isEmpty()) {
            return JsonVO.fail("单据不存在");
        }
        String cur = rows.get(0).getStatusEnum();
        if (!STATUS_PENDING_APPROVAL.equals(cur)
                && !"WITHDRAW".equals(cur) && !"REJECT".equals(cur)) {
            return JsonVO.fail("当前状态不允许提交审批");
        }

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        for (WkfSupplyRequest row : rows) {
            WkfSupplyRequest update = new WkfSupplyRequest();
            update.setId(row.getId());
            update.setStatusEnum(STATUS_APPROVAL);
            update.setApplicantId(userId);
            update.setApplyTime(now);
            update.setApproverId(userId);
            update.setUpdatedAt(now);
            update.setUpdatedBy(userId);
            wkfSupplyRequestMapper.updateById(update);
        }

        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);
        result.setStatusEnum(STATUS_APPROVAL);
        return JsonVO.success(result);
    }

    // ==================== 5. 删除 ====================

    @Override
    public String deleteDraftByBusNo(String busNo) {
        List<WkfSupplyRequest> rows = listActive(busNo);
        if (rows.isEmpty()) {
            return "单据不存在或已删除";
        }
        String currentStatus = rows.get(0).getStatusEnum();
        if (!STATUS_PENDING_APPROVAL.equals(currentStatus)
                && !"WITHDRAW".equals(currentStatus)
                && !"REJECT".equals(currentStatus)) {
            return "当前状态不允许删除，当前状态: " + currentStatus;
        }
        String userId = currentUserId();
        softDeleteRows(busNo, LocalDateTime.now(), userId);
        return "success";
    }

    // ==================== 6. 详情 ====================

    @Override
    public StockOutDetailVO getDetailByBusNo(String busNo) {
        log.info("查询出库单详情，busNo: {}", busNo);

        // 查询所有明细行
        List<StockOutLineVO> lineList = wkfSupplyRequestMapper.selectDetailLinesByBusNo(busNo);
        log.info("明细行数量: {}，busNo: {}", lineList.size(), busNo);

        // 查询头信息
        LambdaQueryWrapper<WkfSupplyRequest> headerWrapper = new LambdaQueryWrapper<>();
        headerWrapper.eq(WkfSupplyRequest::getBusNo, busNo)
                .eq(WkfSupplyRequest::getIsDeleted, false)
                .last("LIMIT 1");
        WkfSupplyRequest header = wkfSupplyRequestMapper.selectOne(headerWrapper);
        if (header == null) {
            log.warn("出库单主记录不存在，busNo: {}", busNo);
            return null;
        }

        return buildEnrichedDetailVO(header, lineList);
    }

    // ==================== 7. 列表 ====================

    @Override
    public PageDTO<StockOutListVO> getPharmacyList(StockOutQuery query) {
        log.info("查询出库单列表，pageIndex: {}, pageSize: {}", query.getPageIndex(), query.getPageSize());
        Page<StockOutListVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        IPage<StockOutListVO> resultPage = wkfSupplyRequestMapper.selectDistinctBusNoPage(page, query);
        log.info("分页查询结果，总记录数: {}, 当前页记录数: {}",
                resultPage.getTotal(), resultPage.getRecords().size());
        return PageDTO.create((Page<StockOutListVO>) resultPage);
    }

    // ==================== 私有辅助方法 ====================

    private void validateSaveRequest(StockOutSaveRequestDTO dto) {
        if (dto == null || dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("出库单明细不能为空");
        }
        for (int i = 0; i < dto.getLines().size(); i++) {
            StockOutSaveRequestDTO.StockOutLineDTO line = dto.getLines().get(i);
            if (line == null) throw new IllegalArgumentException("第" + (i + 1) + "行明细不能为空");
            if (line.getItemQuantity() == null || line.getItemQuantity().compareTo(BigDecimal.ZERO) <= 0)
                throw new IllegalArgumentException("第" + (i + 1) + "行数量必须大于0");
            if (!StringUtils.hasText(line.getItemTable()))
                throw new IllegalArgumentException("第" + (i + 1) + "行物品表名不能为空");
            if (!StringUtils.hasText(line.getItemId()))
                throw new IllegalArgumentException("第" + (i + 1) + "行物品ID不能为空");
            if (!StringUtils.hasText(line.getUnitCode()))
                throw new IllegalArgumentException("第" + (i + 1) + "行单位编码不能为空");
        }
    }

    /** 锚点行 */
    private WkfSupplyRequest buildAnchorRow(String busNo, StockOutAddDTO dto,
                                            String practitionerId, LocalDateTime now, String userId) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId(UUID.randomUUID().toString().replace("-", ""));
        row.setTenantId(DEFAULT_TENANT_ID);
        row.setBusNo(busNo);
        row.setTypeEnum(TYPE_WAREHOUSE_STOCKOUT);
        row.setStatusEnum(STATUS_PENDING_APPROVAL);
        row.setCategoryEnum("STOCK_OUT");
        row.setSourceTypeEnum("WAREHOUSE");
        row.setSourceLocationId(dto.getSourceLocationId());
        row.setPurposeTypeEnum("PHARMACY");
        row.setPurposeLocationId(dto.getPurposeLocationId());
        row.setPractitionerId(practitionerId);
        row.setOriginalBusNo(dto.getRelatedReqNo());
        row.setRemake(dto.getRemark());
        row.setApplicantId(userId);
        row.setCreatedAt(now);
        row.setCreatedBy(userId);
        row.setUpdatedAt(now);
        row.setUpdatedBy(userId);
        row.setIsDeleted(false);
        row.setVersion(0);
        // NOT NULL 占位
        row.setItemTable("");
        row.setItemId("");
        row.setItemQuantity(BigDecimal.ZERO);
        row.setUnitCode("");
        return row;
    }

    /** 从 SaveRequest 构建数据行 */
    private List<WkfSupplyRequest> buildRowsFromSaveRequest(StockOutSaveRequestDTO dto, String busNo,
                                                            String status, int version,
                                                            LocalDateTime now, String userId,
                                                            String approverId) {
        List<WkfSupplyRequest> rows = new ArrayList<>();
        for (StockOutSaveRequestDTO.StockOutLineDTO line : dto.getLines()) {
            WkfSupplyRequest row = new WkfSupplyRequest();
            row.setId(UUID.randomUUID().toString().replace("-", ""));
            row.setTenantId(DEFAULT_TENANT_ID);
            row.setBusNo(busNo);
            row.setTypeEnum(StringUtils.hasText(dto.getTypeEnum()) ? dto.getTypeEnum() : TYPE_WAREHOUSE_STOCKOUT);
            row.setStatusEnum(status);
            row.setCategoryEnum(StringUtils.hasText(dto.getCategoryEnum()) ? dto.getCategoryEnum() : "STOCK_OUT");
            row.setSourceTypeEnum(StringUtils.hasText(dto.getSourceTypeEnum()) ? dto.getSourceTypeEnum() : "WAREHOUSE");
            row.setSourceLocationId(dto.getSourceLocationId());
            row.setPurposeTypeEnum(StringUtils.hasText(dto.getPurposeTypeEnum()) ? dto.getPurposeTypeEnum() : "PHARMACY");
            row.setPurposeLocationId(dto.getPurposeLocationId());
            row.setPractitionerId(dto.getPractitionerId());
            row.setItemTable(line.getItemTable());
            row.setItemId(line.getItemId());
            row.setItemQuantity(line.getItemQuantity());
            row.setUnitCode(line.getUnitCode());
            row.setLotNumber(line.getLotNumber());
            row.setTraceNo(line.getTraceNo());
            row.setPrice(line.getPrice() != null ? line.getPrice() : BigDecimal.ZERO);
            row.setTotalPrice(line.getPrice() != null && line.getItemQuantity() != null
                    ? line.getPrice().multiply(line.getItemQuantity()) : BigDecimal.ZERO);
            row.setOccurrenceTime(line.getOccurrenceTime() != null ? line.getOccurrenceTime() : now);
            row.setStartTime(line.getStartTime());
            row.setEndTime(line.getEndTime());
            row.setOriginalBusNo(dto.getOriginalBusNo());
            row.setRemake(line.getRemake());
            row.setApplicantId(userId);
            row.setApproverId(approverId);
            row.setApplyTime(approverId != null ? now : null);
            row.setCreatedAt(now);
            row.setCreatedBy(userId);
            row.setUpdatedAt(now);
            row.setUpdatedBy(userId);
            row.setIsDeleted(false);
            row.setVersion(version);
            rows.add(row);
        }
        return rows;
    }

    /** 组装完整的文档详情视图 */
    private StockOutDetailVO buildEnrichedDetailVO(WkfSupplyRequest header, List<StockOutLineVO> lines) {
        StockOutDetailVO vo = new StockOutDetailVO();
        vo.setBusNo(header.getBusNo());
        vo.setOriginalBusNo(header.getOriginalBusNo());
        vo.setTypeEnum(header.getTypeEnum());
        vo.setStatusEnum(header.getStatusEnum());
        vo.setCategoryEnum(header.getCategoryEnum());
        vo.setSourceTypeEnum(header.getSourceTypeEnum());
        vo.setSourceLocationId(header.getSourceLocationId());
        vo.setPurposeTypeEnum(header.getPurposeTypeEnum());
        vo.setPurposeLocationId(header.getPurposeLocationId());
        vo.setPractitionerId(header.getPractitionerId());
        vo.setCreatorId(header.getCreatedBy());
        vo.setApplicantId(header.getApplicantId());
        vo.setApproverId(header.getApproverId());
        vo.setApplyTime(header.getApplyTime());
        vo.setApprovalTime(header.getApprovalTime());
        vo.setRemark(header.getRemake());
        vo.setVersion(header.getVersion());

        // 源药库名称
        if (StringUtils.hasText(header.getSourceLocationId())) {
            try {
                AdmLocation loc = admLocationMapper.selectById(header.getSourceLocationId());
                if (loc != null) vo.setSourceLocationName(loc.getName());
            } catch (Exception ignored) {}
        }
        // 目的药房名称
        if (StringUtils.hasText(header.getPurposeLocationId())) {
            try {
                AdmLocation loc = admLocationMapper.selectById(header.getPurposeLocationId());
                if (loc != null) vo.setPurposeLocationName(loc.getName());
            } catch (Exception ignored) {}
        }
        // 制单人名称
        if (StringUtils.hasText(header.getPractitionerId())) {
            try {
                AdmPractitioner p = admPractitionerMapper.selectById(header.getPractitionerId());
                if (p != null) vo.setPractitionerName(p.getName());
            } catch (Exception ignored) {}
        }
        // 创建人名称
        if (StringUtils.hasText(header.getCreatedBy())) {
            try {
                AdmPractitioner p = admPractitionerMapper.selectById(header.getCreatedBy());
                if (p != null) vo.setCreatorName(p.getName());
            } catch (Exception ignored) {}
        }
        // 申请人名称
        if (StringUtils.hasText(header.getApplicantId())) {
            try {
                AdmPractitioner p = admPractitionerMapper.selectById(header.getApplicantId());
                if (p != null) vo.setApplicantName(p.getName());
            } catch (Exception ignored) {}
        }
        // 审批人名称
        if (StringUtils.hasText(header.getApproverId())) {
            try {
                AdmPractitioner p = admPractitionerMapper.selectById(header.getApproverId());
                if (p != null) vo.setApproverName(p.getName());
            } catch (Exception ignored) {}
        }

        // 总金额
        BigDecimal totalAmount = BigDecimal.ZERO;
        if (lines != null) {
            for (StockOutLineVO line : lines) {
                if (line.getTotalPrice() != null) {
                    totalAmount = totalAmount.add(line.getTotalPrice());
                }
            }
        }
        vo.setTotalAmount(totalAmount);
        vo.setLines(lines != null ? lines : List.of());

        return vo;
    }

    /** WkfSupplyRequest 实体行 → StockOutLineVO */
    private List<StockOutLineVO> convertRowsToLines(List<WkfSupplyRequest> rows) {
        List<StockOutLineVO> lines = new ArrayList<>();
        for (WkfSupplyRequest r : rows) {
            StockOutLineVO line = new StockOutLineVO();
            line.setId(r.getId());
            line.setItemTable(r.getItemTable());
            line.setItemId(r.getItemId());
            line.setItemQuantity(r.getItemQuantity());
            line.setUnitCode(r.getUnitCode());
            line.setUnitCodeDictText(r.getUnitCode());
            line.setLotNumber(r.getLotNumber());
            line.setTraceNo(r.getTraceNo());
            line.setPrice(r.getPrice());
            line.setTotalPrice(r.getTotalPrice());
            line.setOccurrenceTime(r.getOccurrenceTime());
            line.setStartTime(r.getStartTime());
            line.setEndTime(r.getEndTime());
            line.setRemake(r.getRemake());
            lines.add(line);
        }
        return lines;
    }

    private String requireBusNo(StockOutSaveRequestDTO dto) {
        if (!StringUtils.hasText(dto.getBusNo())) {
            throw new IllegalArgumentException("保存出库单时业务单号不能为空");
        }
        return dto.getBusNo();
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
