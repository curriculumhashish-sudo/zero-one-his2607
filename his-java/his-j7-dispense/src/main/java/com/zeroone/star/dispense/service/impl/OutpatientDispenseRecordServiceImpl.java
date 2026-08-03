package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.OutpatientDispenseRecordMapper;
import com.zeroone.star.dispense.service.IOutpatientDispenseRecordService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseFlatDTO;
import com.zeroone.star.project.dto.j7.dispense.OutpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import com.zeroone.star.project.vo.j7.dispense.DrugItemVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：门诊发退药记录服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 再睡一夏
 * @version 1.0.0
 */
@Service
public class OutpatientDispenseRecordServiceImpl implements IOutpatientDispenseRecordService {

    private static final int EXPORT_LIMIT = 10_000;

    @Resource
    private OutpatientDispenseRecordMapper mapper;

    @Override
    public PageDTO<DispenseGroupVO> queryPage(DispenseQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        long pageIndex = Math.max(query.getPageIndex(), 1);
        long pageSize = Math.max(query.getPageSize(), 1);
        Page<DispenseFlatDTO> page = new Page<>(pageIndex, pageSize);
        mapper.selectOutpatientDispensePage(page, query);

        List<DispenseFlatDTO> flatRows = page.getRecords();
        if (flatRows == null || flatRows.isEmpty()) {
            PageDTO<DispenseGroupVO> empty = new PageDTO<>();
            empty.setPageIndex(pageIndex);
            empty.setPageSize(pageSize);
            empty.setTotal(0L);
            empty.setPages(0L);
            empty.setRows(Collections.emptyList());
            return empty;
        }

        // 按处方号分组，保持顺序
        Map<String, List<DispenseFlatDTO>> grouped = flatRows.stream()
                .collect(Collectors.groupingBy(
                        f -> f.getPrescriptionNo() == null ? "" : f.getPrescriptionNo(),
                        LinkedHashMap::new,
                        Collectors.toList()
                ));

        List<DispenseGroupVO> groups = new ArrayList<>();
        for (List<DispenseFlatDTO> group : grouped.values()) {
            DispenseFlatDTO head = group.get(0);
            DispenseGroupVO vo = new DispenseGroupVO();
            vo.setRecordNo(head.getRecordNo());
            vo.setStatusEnum(head.getStatusEnum());
            vo.setPrescriptionNo(head.getPrescriptionNo());
            vo.setRecordTypeName(head.getRecordTypeName());
            vo.setPatientName(head.getPatientName());
            vo.setClinicNo(head.getClinicNo());
            vo.setDeptName(head.getDeptName());
            vo.setDispenseTime(head.getDispenseTime() != null
                    ? head.getDispenseTime().toString().replace("T", " ") : null);
            vo.setPharmacyName(head.getPharmacyName());
            vo.setDispenserName(head.getDispenserName());

            List<DrugItemVO> items = group.stream().map(f -> {
                DrugItemVO item = new DrugItemVO();
                item.setDrugName(f.getDrugName());
                item.setSpec(f.getSpec());
                item.setQuantity(f.getQuantity());
                item.setUnit(f.getUnit());
                item.setLotNumber(f.getLotNumber());
                item.setDosageInstruction(f.getDosageInstruction());
                item.setDose(f.getDose());
                item.setDoseUnitCode(f.getDoseUnitCode());
                item.setFrequencyCode(f.getFrequencyCode());
                item.setManufacturerText(f.getManufacturerText());
                return item;
            }).collect(Collectors.toList());
            vo.setDrugItems(items);
            groups.add(vo);
        }

        PageDTO<DispenseGroupVO> result = new PageDTO<>();
        result.setPageIndex(pageIndex);
        result.setPageSize(pageSize);
        result.setTotal(page.getTotal());
        result.setPages(page.getPages());
        result.setRows(groups);
        return result;
    }

    @Override
    public List<OutpatientDispenseRecordDTO> listForExport(DispenseQuery query) {
        Objects.requireNonNull(query, "查询条件不能为空");
        List<DispenseFlatDTO> flatRows = mapper.selectOutpatientDispenseFlatList(query, EXPORT_LIMIT + 1);
        if (flatRows.size() > EXPORT_LIMIT) {
            throw new IllegalArgumentException("导出记录不能超过10000条，请缩小筛选范围");
        }
        if (flatRows.isEmpty()) {
            throw new IllegalArgumentException("没有可导出的门诊发退药记录");
        }

        return flatRows.stream().map(f -> {
            OutpatientDispenseRecordDTO dto = new OutpatientDispenseRecordDTO();
            dto.setId(null);
            dto.setRecordNo(f.getRecordNo());
            dto.setOpTypeName(f.getRecordTypeName());
            dto.setStatusEnum(f.getStatusEnum());
            dto.setPatientName(f.getPatientName());
            dto.setClinicNo(f.getClinicNo());
            dto.setOpTime(f.getDispenseTime());
            dto.setPharmacyName(f.getPharmacyName());
            dto.setOperatorAccount(f.getOperatorAccount());
            dto.setDrugName(f.getDrugName());
            dto.setSpec(f.getSpec());
            dto.setQuantity(f.getQuantity());
            dto.setUnit(f.getUnit());
            dto.setLotNumber(f.getLotNumber());
            dto.setDosageInstruction(f.getDosageInstruction());
            dto.setDose(f.getDose());
            dto.setDoseUnitCode(f.getDoseUnitCode());
            dto.setFrequencyCode(f.getFrequencyCode());
            dto.setManufacturerText(f.getManufacturerText());
            return dto;
        }).collect(Collectors.toList());
    }
}
