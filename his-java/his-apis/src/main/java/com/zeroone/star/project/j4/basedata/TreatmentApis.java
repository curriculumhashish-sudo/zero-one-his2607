package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.j4.treatment.CreateTreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.ToggleStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.UpdateTreatmentDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@Tag(name = "诊疗管理", description = "诊疗项目管理相关接口")
@RequestMapping("/basedata/treatment")
public interface TreatmentApis {

    @Operation(summary = "新增诊疗", description = "新增诊疗项目信息")
    @PostMapping("create")
    JsonVO<TreatmentVO> createTreatment(@RequestBody CreateTreatmentDTO dto);

    @Operation(summary = "修改诊疗", description = "修改诊疗项目信息")
    @PutMapping("update")
    JsonVO<TreatmentVO> updateTreatment(@RequestBody UpdateTreatmentDTO dto);

    @Operation(summary = "删除诊疗", description = "删除诊疗项目信息")
    @DeleteMapping("delete/{id}")
    JsonVO<String> deleteTreatment(@Parameter(description = "诊疗项目ID") @PathVariable String id);

    @Operation(summary = "启/停用诊疗", description = "启用或停用诊疗项目")
    @PutMapping("toggle-status")
    JsonVO<TreatmentVO> toggleStatus(@RequestBody ToggleStatusDTO dto);
}