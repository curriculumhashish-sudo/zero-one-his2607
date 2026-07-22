package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.InpatientDispenseRecordMapper;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class InpatientDispenseRecordServiceImplTest {

    @Mock
    private InpatientDispenseRecordMapper mapper;

    @InjectMocks
    private InpatientDispenseRecordServiceImpl service;

    @Test
    void queryPageReturnsDatabaseProjection() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        query.setPageIndex(2);
        query.setPageSize(10);

        InpatientDispenseRecordDTO record = new InpatientDispenseRecordDTO();
        record.setId("DISP-001");
        record.setPatientName("张三");
        record.setInpatientNo("IP-001");

        when(mapper.selectInpatientDispensePage(any(Page.class), eq(query)))
                .thenAnswer(invocation -> {
                    Page<InpatientDispenseRecordDTO> page = invocation.getArgument(0);
                    page.setTotal(11);
                    page.setRecords(List.of(record));
                    return page;
                });

        var result = service.queryPage(query);

        assertEquals(2, result.getPageIndex());
        assertEquals(10, result.getPageSize());
        assertEquals(11, result.getTotal());
        InpatientDispenseRecordVO row = result.getRows().get(0);
        assertEquals("DISP-001", row.getId());
        assertEquals("张三", row.getPatientName());
        assertEquals("IP-001", row.getInpatientNo());
    }

    @Test
    void listForExportRejectsDataOverLimit() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        when(mapper.selectInpatientDispenseList(eq(query), eq(10_001)))
                .thenReturn(Collections.nCopies(10_001, new InpatientDispenseRecordDTO()));

        IllegalStateException error = assertThrows(
                IllegalStateException.class,
                () -> service.listForExport(query)
        );

        assertEquals("导出记录不能超过10000条，请缩小筛选范围", error.getMessage());
    }

    @Test
    void listForExportRejectsEmptyResult() {
        InpatientDispenseRecordQuery query = new InpatientDispenseRecordQuery();
        when(mapper.selectInpatientDispenseList(eq(query), eq(10_001)))
                .thenReturn(Collections.emptyList());

        IllegalStateException error = assertThrows(
                IllegalStateException.class,
                () -> service.listForExport(query)
        );

        assertEquals("没有可导出的住院发退药记录", error.getMessage());
    }
}
