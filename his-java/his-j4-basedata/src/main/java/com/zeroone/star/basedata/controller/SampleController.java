package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.j4.basedata.SampleApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
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
@Tag(name = "basedata-sample", description = "样本管理接口")
public class SampleController implements SampleApis {

    @Operation(summary = "启用/停用样本定义", description = "根据主键与乐观锁版本号，更新样本定义的发布状态")
    @PostMapping("/update-status")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> updateStatus(@Valid @RequestBody SpecimenDefinitionStatusDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }

    @Operation(summary = "删除样本定义", description = "根据主键与乐观锁版本号，软删除样本定义")
    @PostMapping("/delete")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> delete(@Valid @RequestBody SpecimenDefinitionDeleteDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }

    @Operation(summary = "修改样本定义", description = "根据主键与乐观锁版本号，修改样本名称、标本种类")
    @PostMapping("/update")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> update(@Valid @RequestBody SpecimenDefinitionUpdateDTO dto) {
        // TODO: 后续工单实现具体业务逻辑
        throw new UnsupportedOperationException("待实现");
    }
}