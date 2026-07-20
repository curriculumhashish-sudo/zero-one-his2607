package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * <p>
 * 描述：住院发退药记录接口定义
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhishixuebao
 * @version 1.0.0
 */
@Tag(name = "住院发退药记录", description = "住院发退药记录分页查询与Excel导出")
@RequestMapping("/dispense/inpatient-records")
public interface InpatientDispenseRecordApis {

    /**
     * 获取住院发退药记录分页。
     *
     * @param query 分页及筛选条件
     * @return 住院发退药记录分页数据
     */
    @Operation(summary = "获取发药记录分页", description = "按病区、患者、床号、状态、日期和药品筛选住院发退药记录")
    @GetMapping("/page")
    JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(@Valid InpatientDispenseRecordQuery query);

    /**
     * 按当前筛选条件导出住院发退药记录。
     *
     * @param query 筛选条件，pageIndex和pageSize不参与导出范围计算
     * @param response HTTP响应，用于输出Excel文件流
     */
    @Operation(
            summary = "导出记录",
            description = "复用分页查询的筛选条件导出全部匹配记录，返回xlsx文件流",
            responses = @ApiResponse(
                    responseCode = "200",
                    description = "导出成功",
                    content = @Content(
                            mediaType = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
                            schema = @Schema(type = "string", format = "binary")
                    )
            )
    )
    @GetMapping(value = "/export", produces = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet")
    void export(@Valid InpatientDispenseRecordQuery query,
                @Parameter(hidden = true) HttpServletResponse response);
}
