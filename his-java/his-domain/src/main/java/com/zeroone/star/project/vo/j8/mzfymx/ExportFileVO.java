package com.zeroone.star.project.vo.j8.mzfymx;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：文件导出响应对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "文件导出响应对象")
public class ExportFileVO {

    @Schema(description = "文件名", example = "门诊费用明细_20260718165534.xlsx")
    private String fileName;

    @Schema(description = "内容类型", example = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
    private String contentType;

    @Schema(description = "文件内容（Base64编码）")
    private String body;

}