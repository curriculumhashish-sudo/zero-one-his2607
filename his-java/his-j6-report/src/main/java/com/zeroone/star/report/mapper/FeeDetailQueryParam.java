package com.zeroone.star.report.mapper;

import lombok.Data;
import java.util.List;

/**
 * 费用明细查询参数
 * @author J6
 */
@Data
public class FeeDetailQueryParam {
    private String tradeDirection;
    private String registrationDepartmentId;
    private List<String> subCategoryCodes;
    private String patientKeyword;
    private String productNameKeyword;
    private String productCodeKeyword;
    private String cashierId;
    private String settlementDateFrom;
    private String settlementDateTo;
}
