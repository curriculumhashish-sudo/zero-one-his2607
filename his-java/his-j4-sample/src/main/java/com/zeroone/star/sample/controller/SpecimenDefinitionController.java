package com.zeroone.star.sample.controller;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.j4.sample.SpecimenDefinitionApi;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import org.springframework.web.bind.annotation.RestController;

/**
 * 样本定义 Controller
 * 注：本次工单仅产出接口契约与领域模型，不包含 Service/Mapper 实现
 */
@RestController
public class SpecimenDefinitionController implements SpecimenDefinitionApi {

    @Override
    public SpecimenDefinitionOperateVO updateStatus(SpecimenDefinitionStatusDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }

    @Override
    public SpecimenDefinitionOperateVO delete(SpecimenDefinitionDeleteDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }
}