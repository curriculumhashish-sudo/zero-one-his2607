package com.zeroone.star.project.j4.basedata;

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
@RequestMapping("/basedata/treatment")
@Tag(name = "treatment", description = "诊疗管理接口")
public interface TreatmentApis {

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