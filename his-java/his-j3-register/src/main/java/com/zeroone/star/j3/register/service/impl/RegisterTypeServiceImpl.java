package com.zeroone.star.j3.register.service.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.register.entity.HealthcareService;
import com.zeroone.star.j3.register.entity.RegisterType;
import com.zeroone.star.j3.register.mapper.HealthcareServiceMapper;
import com.zeroone.star.j3.register.mapper.RegisterTypeMapper;
import com.zeroone.star.j3.register.service.IRegisterTypeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.register.RegisterDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.j3.register.RegisterNameVO;
import com.zeroone.star.project.vo.j3.register.RegisterVO;
import jakarta.annotation.Resource;
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

    @Resource
    private HealthcareServiceMapper healthcareServiceMapper;

    /** 挂号费类型编码 */
    private static final String REG_TYPE_CODE = "1012";

    @Resource
    private HealthcareServiceMapper healthcareServiceMapper;

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

        // 1. 按名称查找已有 healthcare_service
        String serviceId = baseMapper.selectHealthcareServiceIdByName(dto.getName());

        // 2. 找不到则同步创建一条，确保下拉查询一致
        if (serviceId == null || serviceId.isEmpty()) {
            HealthcareService hs = new HealthcareService();
            String hsId = String.valueOf(IdWorker.getId());
            hs.setId(hsId);
            hs.setTenantId("1");
            hs.setName(dto.getName());
            hs.setTypeCode("1");
            hs.setCategoryCode("REGISTRATION");
            hs.setStatusEnum("ACTIVE");
            hs.setActiveFlag(1);
            hs.setIsDeleted(0);
            hs.setVersion(0);
            hs.setBusNo("HS" + hsId.substring(hsId.length() - 6));
            healthcareServiceMapper.insert(hs);
            serviceId = hsId;
        }

        entity.setInstanceId(serviceId);
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

    /**
     * 挂号类型在医疗服务表中的类别标记
     */
    private static final String CATEGORY_REGISTRATION = "REGISTRATION";

    @Override
    public List<RegisterNameVO> queryRegisterNames() {
        // 只查启用的挂号类型，仅取 id + name 两列（@TableLogic 自动过滤软删）
        return healthcareServiceMapper.selectList(new LambdaQueryWrapper<HealthcareService>()
                .select(HealthcareService::getId, HealthcareService::getName)
                .eq(HealthcareService::getCategoryCode, CATEGORY_REGISTRATION)
                .eq(HealthcareService::getActiveFlag, 1)
                .orderByAsc(HealthcareService::getCreatedAt))
                .stream()
                .map(hs -> new RegisterNameVO(Long.valueOf(hs.getId()), hs.getName()))
                .toList();
    }
}
