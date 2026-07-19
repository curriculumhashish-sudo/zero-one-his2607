package com.zeroone.star.j3.patient.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j3.patient.PatientApis;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：患者管理接口实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 波小夏
 * @version 1.0.0
 */
@RestController
@RequestMapping("patients")
@Tag(name = "patient", description = "患者管理")
public class PatientController implements PatientApis {

    @Operation(summary = "获取选择患者列表")
    @GetMapping("options")
    @Override
    public JsonVO<PageDTO<PatientOptionVO>> queryPatients(PatientQuery query) {
        // TODO: 待实现
        return JsonVO.success(null);
    }

    @Operation(summary = "获取患者详情")
    @GetMapping("{id}")
    @Override
    public JsonVO<PatientVO> getPatient(@PathVariable String id) {
        // TODO: 待实现
        return JsonVO.success(null);
    }

    @Operation(summary = "删除患者")
    @DeleteMapping("{id}")
    @Override
    public JsonVO<String> deletePatient(@PathVariable String id) {
        // TODO: 待实现
        return JsonVO.success("删除成功");
    }
}
