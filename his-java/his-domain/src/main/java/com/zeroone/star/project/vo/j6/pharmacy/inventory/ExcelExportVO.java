package com.zeroone.star.project.vo.j6.pharmacy.inventory;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "库存明细Excel导出文件VO")
public class ExcelExportVO {

    @Schema(description = "导出文件名称", example = "库存明细.xlsx")
    private String fileName;

    @Schema(description = "文件MIME类型", example = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
    private String contentType;

    @Schema(description = "Excel文件二进制Base64编码字符串")
    private String body;

    @Schema(description = "本次导出数据总行数", example = "0")
    private Long exportedRowCount;

    @Schema(description = "数据是否截断，true=数据量过大被截断", example = "false")
    private Boolean truncated;
}