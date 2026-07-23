package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.entity.sample.AdmSpecimenDefinitionDO;
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
import cn.hutool.core.bean.BeanUtil;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import io.swagger.v3.oas.annotations.Parameter;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.List;

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
@Tag(name = "样本管理", description = "样本管理接口")
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

    /**
     * 获取样本名称（根据ID获取单个样本信息）
     */
    @Operation(summary = "获取样本名称", description = "根据样本定义主键ID获取单个样本的详细信息")
    @GetMapping("get-sample-name")
    @Override
    public JsonVO<SpecimenDefinitionDetailVO> getSampleName(
            @Parameter(description = "样本定义主键", example = "328090253175296000")
            @RequestParam String id) {
        // TODO: 待实现 Service/Mapper 层逻辑，当前返回 mock 数据
        AdmSpecimenDefinitionDO sample = buildMockSample();
        SpecimenDefinitionDetailVO vo = new SpecimenDefinitionDetailVO();
        BeanUtil.copyProperties(sample, vo);
        return JsonVO.success(vo);
    }

    /**
     * 获取样本列表（条件+分页）
     */
    @Operation(summary = "获取样本列表", description = "支持按名称/拼音/五笔模糊查询，按标本种类/状态精确筛选，返回分页结果")
    @PostMapping("query-sample-list")
    @Override
    public JsonVO<PageDTO<SpecimenDefinitionDetailVO>> querySampleList(@RequestBody SpecimenDefinitionQuery query) {
        // TODO: 待实现 Service/Mapper 层逻辑，当前返回 mock 数据
        AdmSpecimenDefinitionDO sample = buildMockSample();
        SpecimenDefinitionDetailVO vo = new SpecimenDefinitionDetailVO();
        BeanUtil.copyProperties(sample, vo);
        PageDTO<SpecimenDefinitionDetailVO> pageDTO = new ExtendPageDTO<>(List.of(vo), 1L, 10L, 1L);
        return JsonVO.success(pageDTO);
    }

    /**
     * 新增样本定义
     */
    @Operation(summary = "新增样本定义", description = "根据传入的样本数据新增一条样本定义记录")
    @PostMapping("add-sample")
    @Override
    public JsonVO<String> addSample(@RequestBody SpecimenDefinitionAddDTO dto) {
        // TODO: 待实现 Service/Mapper 层逻辑，当前返回 mock 数据
        // 示例：将 DTO 转换为实体对象
        AdmSpecimenDefinitionDO sample = new AdmSpecimenDefinitionDO();
        BeanUtil.copyProperties(dto, sample);
        return JsonVO.success("新增样本定义成功");
    }

    /**
     * 构建 mock 样本实体数据（基于测试数据 SPD0001-血）
     */
    private AdmSpecimenDefinitionDO buildMockSample() {
        AdmSpecimenDefinitionDO sample = new AdmSpecimenDefinitionDO();
        sample.setId("328090253175296000");
        sample.setTenantId("1");
        sample.setBusNo("SPD0001");
        sample.setName("血");
        sample.setPyStr("X");
        sample.setWbStr("T");
        sample.setTypeEnum("BLOOD");
        sample.setStatusEnum("ACTIVE");
        sample.setCreatedAt(LocalDateTime.of(2026, 6, 24, 16, 33, 31));
        sample.setUpdatedAt(LocalDateTime.of(2026, 6, 24, 16, 33, 31));
        sample.setCreatedBy("1");
        sample.setUpdatedBy("1");
        sample.setVersion(0);
        return sample;
    }
}
