package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.j4.basedata.SampleApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：样本管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/sample")
public class SampleController implements SampleApis {

    @Override
    public JsonVO<SpecimenDefinitionOperateVO> updateStatus(SpecimenDefinitionStatusDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }

    @Override
    public JsonVO<SpecimenDefinitionOperateVO> delete(SpecimenDefinitionDeleteDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }
}