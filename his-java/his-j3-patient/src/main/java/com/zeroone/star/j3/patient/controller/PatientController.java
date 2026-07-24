package com.zeroone.star.j3.patient.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.j3.patient.PatientApis;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;
import com.zeroone.star.j3.patient.service.PatientService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：患者管理接口实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 波小夏
 * @author 照夜清
 * @version 1.0.0
 */
@RestController
@RequestMapping("/patients")
@Tag(name = "patient", description = "患者管理")
@RequiredArgsConstructor
public class PatientController implements PatientApis {

    private final PatientService patientService;

    /**
     * 波小夏负责：获取选择患者列表。
     */
    @Operation(
            summary = "获取选择患者列表",
            description = "用于门诊挂号、住院登记等业务选择患者"
    )
    @GetMapping("options")
    @Override
    public JsonVO<PageDTO<PatientOptionVO>> queryPatients(
            PatientQuery query) {
        // TODO: 业务实现阶段查询患者选择列表
        return JsonVO.success(null);
    }

    /**
     * 照夜清负责：获取患者管理列表。
     */
    @Operation(
            summary = "获取患者管理列表",
            description = "根据姓名、身份证号、手机号、性别和状态等条件分页查询患者"
    )
    @GetMapping("page")
    @Override
    public JsonVO<PageDTO<PatientListVO>> queryPatientPage(
            @Valid PatientQuery query) {
        return JsonVO.success(patientService.queryPatientPage(query));
    }

    /**
     * 波小夏负责：获取患者详情。
     */
    @Operation(
            summary = "获取患者详情",
            description = "根据患者ID获取患者档案详情"
    )
    @GetMapping("{id}")
    @Override
    public JsonVO<PatientVO> getPatient(
            @PathVariable("id") String id) {
        // TODO: 业务实现阶段查询患者详情
        return JsonVO.success(null);
    }

    /**
     * 照夜清负责：新增患者。
     */
    @Operation(
            summary = "新增患者",
            description = "创建患者档案，患者编号由后端生成"
    )
    @PostMapping
    @Override
    public JsonVO<String> createPatient(
            @Valid @RequestBody PatientCreateDTO dto) {
        patientService.createPatient(dto);
        return JsonVO.success("新增成功");
    }

    /**
     * 照夜清负责：修改患者。
     */
    @Operation(
            summary = "修改患者",
            description = "根据患者ID修改患者档案"
    )
    @PutMapping("{id}")
    @Override
    public JsonVO<String> updatePatient(
            @PathVariable("id") String id,
            @Valid @RequestBody PatientUpdateDTO dto) {
        patientService.updatePatient(id, dto);
        return JsonVO.success("修改成功");
    }

    /**
     * 波小夏负责：删除患者。
     */
    @Operation(
            summary = "删除患者",
            description = "根据患者ID软删除患者档案"
    )
    @DeleteMapping("{id}")
    @Override
    public JsonVO<String> deletePatient(
            @PathVariable("id") String id) {
        // TODO: 业务实现阶段检查挂号、住院等业务引用
        return JsonVO.success("删除成功");
    }
}
