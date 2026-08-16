package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "报表导出响应结构")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class ExportResultVo {

    @Schema(description = "导出文件名称", example = "门诊费用明细_2026xxxxxx.xlsx")
    private String filename;

    @Schema(description = "文件的MIME类型", example = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
    private String contentType;

    @Schema(description = "文件内容的Base64编码字符串")
    private String body;

    @Schema(description = "导出的数据总行数", example = "100")
    private Integer rowCount;

    @Schema(description = "数据是否被截断", example = "false")
    private Boolean truncated;
}
