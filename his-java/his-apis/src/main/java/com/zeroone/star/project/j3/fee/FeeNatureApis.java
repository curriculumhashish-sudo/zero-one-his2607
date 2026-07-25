package com.zeroone.star.project.j3.fee;

import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

import java.util.List;

/**
 * <p>
 * 描述：费用性质管理接口
 * </p>
 * <p>业务关联：费用性质是收费结算的基础字典，在患者缴费时供收费员选择费用类别（自费/医保等），
 * 影响费用减免计算和医保报销比例。一条费用性质可被多条门诊/住院收费记录引用。</p>
 * @author 白文儿
 * @version 1.0.0
 */
@Tag(name = "费用性质管理", description = "费用性质（自费/医保等）是收费结算的核心字典，患者缴费时用于区分费用类别并计算报销比例")
public interface FeeNatureApis {

    /**
     * 获取费用性质列表（条件+分页）
     * @param query 查询条件
     * @return 分页数据
     */
    @Operation(summary = "获取费用性质列表（条件+分页）",
            description = "查询所有费用性质记录，收费结算模块引用此列表生成患者费用明细。支持按名称或编码模糊搜索，返回分页结果")
    JsonVO<PageDTO<FeeNatureVO>> getPageList(
            @Parameter(description = "查询条件", required = true)
            FeeNatureQuery query
    );

    /**
     * 修改费用性质
     * @param id 费用性质ID
     * @param dto 费用性质数据
     * @return 操作结果
     */
    @Operation(summary = "修改费用性质",
            description = "根据ID修改费用性质信息。修改后新产生的收费记录将使用更新后的名称/编码，已结算的历史记录不受影响")
    JsonVO<String> update(
            @Parameter(description = "费用性质ID", example = "1", required = true)
            Long id,
            @Parameter(description = "费用性质数据", required = true)
            FeeNatureDTO dto
    );

    /**
     * 停/启用费用性质
     * @param id 费用性质ID
     * @param isEnabled 是否启用 1 启用 0 禁用
     * @return 操作结果
     */
    @Operation(summary = "停/启用费用性质",
            description = "切换费用性质的启用状态。停用后收费结算下拉框不再显示该项，新收费不可选择；已有结算记录不受影响，仍可正常查询统计")
    JsonVO<String> updateStatus(
            @Parameter(description = "费用性质ID", example = "1", required = true)
            Long id,
            @Parameter(description = "是否启用 1 启用 0 禁用", example = "1", required = true)
            Integer isEnabled
    );

    /**
     * 获取费用性质名称列表（用于下拉框）
     * @return 名称列表
     */
    @Operation(summary = "获取费用性质名称",
            description = "获取所有已启用的费用性质，供门诊/住院收费结算界面下拉框选择，常用于患者缴费时确定费用类别")
    JsonVO<List<FeeNatureVO>> getNameList();

    /**
     * 添加费用性质
     * @param dto 费用性质数据
     * @return 操作结果
     */
    @Operation(summary = "添加费用性质",
            description = "新增一条费用性质记录（如新增医保类型）。添加后即时生效，收费结算模块可立即选用")
    JsonVO<String> add(
            @Parameter(description = "费用性质数据", required = true)
            FeeNatureDTO dto
    );

    /**
     * 删除费用性质
     * @param id 费用性质ID
     * @return 操作结果
     */
    @Operation(summary = "删除费用性质",
            description = "根据ID删除费用性质。注意：若该费用性质已被收费结算记录引用（存在关联患者缴费数据），则不允许删除，需先停用或处理关联数据")
    JsonVO<String> delete(
            @Parameter(description = "费用性质ID", example = "1", required = true)
            Long id
    );
}
