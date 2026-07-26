package com.zeroone.star.j3.register.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.register.entity.RegisterType;
import com.zeroone.star.j3.register.mapper.RegisterTypeMapper;
import com.zeroone.star.j3.register.service.IRegisterTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.register.RegisterDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.j3.register.RegisterVO;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：挂号类型服务实现类（基于 adm_charge_item_definition JOIN adm_healthcare_service）
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Service
public class RegisterTypeServiceImpl extends ServiceImpl<RegisterTypeMapper, RegisterType> implements IRegisterTypeService {

    /** 挂号费类型编码 */
    private static final String REG_TYPE_CODE = "1012";

    /** DB状态转API状态：ACTIVE→1, 其他→0 */
    private Integer toApiStatus(String dbStatus) {
        return "ACTIVE".equals(dbStatus) ? 1 : 0;
    }

    /** API状态转DB状态：1→ACTIVE, 其他→INACTIVE */
    private String toDbStatus(Integer apiStatus) {
        return Integer.valueOf(1).equals(apiStatus) ? "ACTIVE" : "INACTIVE";
    }

    /** Entity转VO（businessCode 优先用 JOIN 来的 serviceBusNo） */
    private RegisterVO toVO(RegisterType entity) {
        if (entity == null) return null;
        RegisterVO vo = new RegisterVO();
        vo.setId(entity.getId() != null ? Long.parseLong(entity.getId()) : null);
        vo.setName(entity.getName());
        vo.setMdtrtCertNo(entity.getMdtrtCertNo());
        // JOIN来的业务编码优先，否则用 type_code
        vo.setBusinessCode(entity.getServiceBusNo() != null ? entity.getServiceBusNo() : entity.getBusinessCode());
        vo.setAmount(entity.getAmount());
        vo.setStatus(toApiStatus(entity.getStatus()));
        return vo;
    }

    /** DTO转Entity（新增用） */
    private RegisterType dtoToEntity(RegisterDTO dto) {
        RegisterType entity = new RegisterType();
        BeanUtil.copyProperties(dto, entity, "status");

        // status: Integer → String
        entity.setStatus(toDbStatus(dto.getStatus()));

        // 挂号费固定类型编码（type_code）
        entity.setBusinessCode(REG_TYPE_CODE);

        // 默认值
        entity.setTenantId("1");
        entity.setInstanceTable("adm_healthcare_service");
        entity.setPriceTypeCode("SALE");
        entity.setIsDeleted(0);
        return entity;
    }

    @Override
    public PageDTO<RegisterVO> queryRegisterList(RegisterQuery query) {
        Page<RegisterType> page = new Page<>(query.getPageIndex(), query.getPageSize());
        String keyword = query.getKeyword();

        // 使用自定义 JOIN 查询
        IPage<RegisterType> resultPage = baseMapper.selectRegisterTypePage(page, keyword);

        List<RegisterVO> voList = resultPage.getRecords().stream()
                .map(this::toVO)
                .collect(Collectors.toList());

        PageDTO<RegisterVO> result = new PageDTO<>();
        result.setPageIndex(resultPage.getCurrent());
        result.setPageSize(resultPage.getSize());
        result.setTotal(resultPage.getTotal());
        result.setPages(resultPage.getPages());
        result.setRows(voList);
        return result;
    }

    @Override
    public boolean addRegister(RegisterDTO dto) {
        RegisterType entity = dtoToEntity(dto);
        entity.setId(String.valueOf(IdWorker.getId()));

        // 根据名称自动关联 adm_healthcare_service，获取业务编码 bus_no
        String serviceId = baseMapper.selectHealthcareServiceIdByName(dto.getName());
        if (serviceId != null && !serviceId.isEmpty()) {
            entity.setInstanceId(serviceId);
        }
        return save(entity);
    }

    @Override
    public boolean modifyRegister(Long id, RegisterDTO dto) {
        if (id == null) return false;
        RegisterType entity = new RegisterType();
        entity.setId(String.valueOf(id));
        entity.setName(dto.getName());
        entity.setMdtrtCertNo(dto.getMdtrtCertNo());
        entity.setAmount(dto.getAmount());
        entity.setStatus(toDbStatus(dto.getStatus()));
        // 不更新 businessCode/type_code，保持原有 healthcare_service 关联
        return updateById(entity);
    }

    @Override
    public boolean removeRegister(Long id) {
        if (id == null) return false;
        RegisterType entity = new RegisterType();
        entity.setId(String.valueOf(id));
        entity.setIsDeleted(1);
        return updateById(entity);
    }
}
