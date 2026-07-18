package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.dto.j4.treatment.CreateTreatmentDTO;
import com.zeroone.star.project.dto.j4.treatment.ToggleStatusDTO;
import com.zeroone.star.project.dto.j4.treatment.UpdateTreatmentDTO;
import com.zeroone.star.project.j4.basedata.TreatmentApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentVO;
import org.springframework.web.bind.annotation.RestController;

import jakarta.annotation.Resource;

@RestController
public class TreatmentController implements TreatmentApis {

    @Resource
    private ITreatmentService treatmentService;

    @Override
    public JsonVO<TreatmentVO> createTreatment(CreateTreatmentDTO dto) {
        TreatmentVO vo = treatmentService.createTreatment(dto);
        if (vo != null) {
            return JsonVO.success(vo);
        }
        return JsonVO.fail("新增诊疗失败");
    }

    @Override
    public JsonVO<TreatmentVO> updateTreatment(UpdateTreatmentDTO dto) {
        TreatmentVO vo = treatmentService.updateTreatment(dto);
        if (vo != null) {
            return JsonVO.success(vo);
        }
        return JsonVO.fail("修改诊疗失败，未找到诊疗项目");
    }

    @Override
    public JsonVO<String> deleteTreatment(String id) {
        boolean success = treatmentService.deleteTreatment(id);
        if (success) {
            return JsonVO.success("删除成功");
        }
        return JsonVO.fail("删除失败，未找到诊疗项目");
    }

    @Override
    public JsonVO<TreatmentVO> toggleStatus(ToggleStatusDTO dto) {
        TreatmentVO vo = treatmentService.toggleStatus(dto);
        if (vo != null) {
            return JsonVO.success(vo);
        }
        return JsonVO.fail("状态切换失败，未找到诊疗项目");
    }
}