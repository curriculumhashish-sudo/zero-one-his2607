package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.j4.basedata.dto.TreatmentDTO;
import com.zeroone.star.project.j4.basedata.dto.TreatmentResponseDTO;
import com.zeroone.star.project.j4.basedata.dto.TreatmentStatusDTO;
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
public interface TreatmentApis {

    @PostMapping("/add")
    TreatmentResponseDTO addTreatment(@Valid @RequestBody TreatmentDTO treatmentDTO);

    @PutMapping("/update")
    TreatmentResponseDTO updateTreatment(@Valid @RequestBody TreatmentDTO treatmentDTO);

    @DeleteMapping("/delete/{id}")
    Boolean deleteTreatment(@PathVariable("id") String id);

    @PutMapping("/status")
    TreatmentResponseDTO updateStatus(@RequestBody TreatmentStatusDTO statusDTO);
}