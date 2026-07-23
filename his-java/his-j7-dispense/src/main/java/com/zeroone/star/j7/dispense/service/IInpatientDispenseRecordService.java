package com.zeroone.star.j7.dispense.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;

import java.util.List;

public interface IInpatientDispenseRecordService {

    PageDTO<InpatientDispenseRecordVO> queryPage(InpatientDispenseRecordQuery query);

    List<InpatientDispenseRecordDTO> listForExport(InpatientDispenseRecordQuery query);
}
