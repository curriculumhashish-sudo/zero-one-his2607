package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.query.j4.treatment.TreatmentSelectQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;

import java.util.List;

import com.zeroone.star.project.dto.j4.treatment.TreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentResponseDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊疗管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface TreatmentApis {

    /**
     * 分页查询诊疗项目列表
     * @param query 查询条件
     * @return 分页结果数据
     */
    JsonVO<PageDTO<TreatmentListVO>> queryTreatmentList(TreatmentQuery query);

    /**
     * 查询可选诊疗列表
     * @param query 查询条件
     * @return 分页数据，每条含id/busNo/name/categoryCode
     */
    JsonVO<PageDTO<TreatmentSelectVO>> listTreatmentOptions(TreatmentSelectQuery query);

    /**
     * 获取诊疗详情
     * @param id 诊疗数据主键ID
     * @return 诊疗详情数据
     */
    JsonVO<TreatmentDetailVO> getTreatment(String id);

    /**
     * 获取诊疗分类选项
     * @return 分类名称列表
     */
    JsonVO<List<String>> listCategoryOptions();


    @Operation(summary = "新增诊疗项目")
    @PostMapping("/add")
    TreatmentResponseDTO addTreatment(@Valid @RequestBody TreatmentDTO treatmentDTO);

    @Operation(summary = "修改诊疗项目")
    @PutMapping("/update")
    TreatmentResponseDTO updateTreatment(@Valid @RequestBody TreatmentDTO treatmentDTO);

    @Operation(summary = "删除诊疗项目")
    @DeleteMapping("/delete/{id}")
    Boolean deleteTreatment(@Parameter(description = "诊疗项目ID") @PathVariable("id") String id);

    @Operation(summary = "启/停用诊疗项目")
    @PutMapping("/status")
    TreatmentResponseDTO updateStatus(@Valid @RequestBody TreatmentStatusDTO statusDTO);
}