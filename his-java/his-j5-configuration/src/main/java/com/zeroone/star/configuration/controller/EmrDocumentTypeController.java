package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.configuration_management.medical_record_template.EmrDocumentTypeApis;
import com.zeroone.star.project.query.j5.configuration_management.medical_record_template.EmrDocumentTypeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeDetailVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeNameVO;
import com.zeroone.star.project.vo.j5.configuration_management.medical_record_template.EmrDocumentTypeVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@Tag(name = "配置管理-文书类型", description = "文书类型接口")
public class EmrDocumentTypeController implements EmrDocumentTypeApis {

    @Override
    public JsonVO<List<EmrDocumentTypeNameVO>> getEmrDocumentTypeName() {
        return JsonVO.success(null);
    }

    @Override
    public JsonVO<PageDTO<EmrDocumentTypeVO>> queryEmrDocumentType(EmrDocumentTypeQuery query) {
        return JsonVO.success(null);
    }

    @Override
    public JsonVO<EmrDocumentTypeDetailVO> getEmrDocumentTypeDetail(String id) {
        return JsonVO.success(null);
    }
}