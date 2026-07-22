package com.zeroone.star.j7.dispense.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j7.dispense.mapper.InpatientDispenseRecordMapper;
import com.zeroone.star.j7.dispense.service.IInpatientDispenseRecordService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.Collections;
import java.util.List;

@Service
public class InpatientDispenseRecordServiceImpl implements IInpatientDispenseRecordService {

    private static final int EXPORT_LIMIT = 10_000;

    @Resource
    private InpatientDispenseRecordMapper mapper;

    @Override
    public PageDTO<InpatientDispenseRecordVO> queryPage(InpatientDispenseRecordQuery query) {
        long pageIndex = Math.max(query.getPageIndex(), 1);
        long pageSize = Math.max(query.getPageSize(), 1);
        Page<InpatientDispenseRecordDTO> page = new Page<>(pageIndex, pageSize);
        mapper.selectInpatientDispensePage(page, query);
        PageDTO<InpatientDispenseRecordVO> result = PageDTO.create(page, InpatientDispenseRecordVO::new);
        if (result.getRows() == null) {
            result.setRows(Collections.emptyList());
        }
        return result;
    }

    @Override
    public List<InpatientDispenseRecordDTO> listForExport(InpatientDispenseRecordQuery query) {
        List<InpatientDispenseRecordDTO> records = mapper.selectInpatientDispenseList(query, EXPORT_LIMIT + 1);
        if (records.size() > EXPORT_LIMIT) {
            throw new IllegalStateException("导出记录不能超过10000条，请缩小筛选范围");
        }
        if (records.isEmpty()) {
            throw new IllegalStateException("没有可导出的住院发退药记录");
        }
        return records;
    }
}
