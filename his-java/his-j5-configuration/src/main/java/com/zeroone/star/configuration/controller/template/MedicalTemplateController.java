package com.zeroone.star.configuration.controller.template;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.emr.template.MedicalTemplateApis;
import com.zeroone.star.project.j5.emr.template.query.MedicalTemplateQuery;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateDetailVO;
import com.zeroone.star.project.j5.emr.template.vo.MedicalTemplateVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.RestController;

import java.util.Arrays;

/**
 * 病历模板控制器
 */
@Tag(name = "配置管理-病历模板", description = "病历模板配置管理接口")
@RestController
public class MedicalTemplateController implements MedicalTemplateApis {

    @Override
    public JsonVO<PageDTO<MedicalTemplateVO>> getList(MedicalTemplateQuery query) {
        // 模拟返回数据，实际业务逻辑将在Service层实现
        MedicalTemplateVO template1 = new MedicalTemplateVO();
        template1.setId(1L);
        template1.setTemplateName("门诊病历模板");
        template1.setMedicalType("1");
        template1.setTemplateContent("患者基本信息：姓名、年龄、性别...");
        template1.setStatus(1);
        template1.setCreateTime("2023-01-01 10:00:00");
        template1.setUpdateTime("2023-01-02 15:30:00");
        template1.setCreateBy("admin");
        template1.setUpdateBy("admin");

        MedicalTemplateVO template2 = new MedicalTemplateVO();
        template2.setId(2L);
        template2.setTemplateName("住院病历模板");
        template2.setMedicalType("2");
        template2.setTemplateContent("住院患者详细信息：入院记录、诊断等...");
        template2.setStatus(1);
        template2.setCreateTime("2023-01-03 09:00:00");
        template2.setUpdateTime("2023-01-04 14:20:00");
        template2.setCreateBy("admin");
        template2.setUpdateBy("admin");

        PageDTO<MedicalTemplateVO> pageDTO = new PageDTO<>(
                query.getPageIndex(),
                query.getPageSize(),
                2L,
                1L,
                Arrays.asList(template1, template2)
        );

        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<MedicalTemplateDetailVO> getDetail(Long id) {
        // 模拟返回数据，实际业务逻辑将在Service层实现
        MedicalTemplateDetailVO detail = new MedicalTemplateDetailVO();
        detail.setId(id);
        detail.setTemplateName("门诊病历模板");
        detail.setMedicalType("1");
        detail.setTemplateContent("患者基本信息：姓名、年龄、性别...");
        detail.setDescription("用于门诊患者的病历记录");
        detail.setStatus(1);
        detail.setCreateTime("2023-01-01 10:00:00");
        detail.setUpdateTime("2023-01-02 15:30:00");
        detail.setCreateBy("admin");
        detail.setUpdateBy("admin");

        return JsonVO.success(detail);
    }
}
