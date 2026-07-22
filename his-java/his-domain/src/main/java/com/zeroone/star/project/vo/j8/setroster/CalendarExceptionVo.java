package com.zeroone.star.project.vo.j8.setroster;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>描述: 节日例外-响应视图对象</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Data
@Schema(description = "节日例外响应对象")
public class CalendarExceptionVo {

    /**
     * 主键ID
     * 作用: 前端进行“编辑”或“删除”操作的唯一凭证
     */
    @Schema(description = "主键ID", example = "1752938475839201234")
    private String id;

    /**
     * 起始日期
     * 对应数据库: exception_date
     * 注意: 使用 @JsonFormat 确保传给前端时格式统一为 yyyy-MM-dd
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "起始日期", example = "2026-07-14")
    private LocalDate exceptionDate;

    /**
     * 结束日期
     * 对应数据库: range_end
     * 说明: 若与起始日期相同，则为单日例外；否则为区间
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "结束日期", example = "2026-07-16")
    private LocalDate rangeEnd;

    /**
     * 例外类型编码
     * 对应数据库: exception_type
     * 值示例: CLOSED, REDUCED, OVERRIDE, EXTRA
     */
    @Schema(description = "例外类型编码(CLOSED/REDUCED/OVERRIDE/EXTRA)", example = "CLOSED")
    private String exceptionType;

    /**
     * 【扩展】例外类型名称
     * 作用: 后端直接翻译好字典值，前端无需二次转换即可显示中文
     * 示例: "停诊", "减号", "加号"
     */
    @Schema(description = "例外类型名称(如: 停诊)", example = "停诊")
    private String exceptionTypeName;

    /**
     * 说明/备注
     * 对应数据库: reason
     */
    @Schema(description = "说明/备注", example = "国庆节调休")
    private String reason;

    /**
     * 是否参与自动生成
     * 对应数据库: auto_apply_on_generate
     * 值: 1-是, 0-否
     */
    @Schema(description = "是否参与自动生成(1:是, 0:否)", example = "1")
    private Integer autoApplyOnGenerate;
}
