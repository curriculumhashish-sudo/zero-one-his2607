package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;

import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;

import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：诊断服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class DiagnosisServiceImpl extends ServiceImpl<DiagnosisMapper, ClnConditionDefinitionDO> 
    implements IDiagnosisService {

    @Override
    public Page<ClnConditionDefinitionDO> pageQuery(DiagnosisDefinitionQuery query) {
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper();
        
        // 添加查询条件
        if (query.getName() != null && !query.getName().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getName, query.getName());
        }
        if (query.getPyStr() != null && !query.getPyStr().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getPyStr, query.getPyStr());
        }
        if (query.getWbStr() != null && !query.getWbStr().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getWbStr, query.getWbStr());
        }
        if (query.getSourceEnum() != null && !query.getSourceEnum().trim().isEmpty()) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, query.getSourceEnum());
        }
        if (query.getTypeCode() != null && !query.getTypeCode().trim().isEmpty()) {
            wrapper.eq(ClnConditionDefinitionDO::getTypeCode, query.getTypeCode());
        }
        if (query.getStatusEnum() != null && !query.getStatusEnum().trim().isEmpty()) {
            wrapper.eq(ClnConditionDefinitionDO::getStatusEnum, query.getStatusEnum());
        }
        
        // 分页查询
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        return this.page(page, wrapper);
    }

    @Override
    public ClnConditionDefinitionDO getById(String id) {
        return this.getById(id);
    }

    @Override
    public List<DiagnosisDefinitionDetailVO.ReferenceDetail> getReferences(String id) {
        // 这里需要根据实际业务逻辑实现引用关系查询
        // 暂时返回空列表，实际实现需要根据数据库中的外键关系来确定
        return new ArrayList<>();
    }

    @Override
    @Transactional
    public String saveDefinition(DiagnosisDefinitionDTO dto) {
        ClnConditionDefinitionDO entity = new ClnConditionDefinitionDO();
        
        // 设置基本信息
        entity.setSourceEnum(dto.getSourceEnum());
        entity.setName(dto.getName());
        entity.setTypeCode(dto.getTypeCode());
        entity.setDiseaseCode(dto.getDiseaseCode());
        entity.setYbFlag(dto.getYbFlag());
        entity.setYbNo(dto.getYbNo());
        entity.setYbMatchFlag(dto.getYbMatchFlag());
        entity.setDescription(dto.getDescription());
        
        // 自动生成编码
        entity.setConditionCode(generateConditionCode());
        
        // 自动生成拼音码和五笔码
        entity.setPyStr(generatePinYinCode(dto.getName()));
        entity.setWbStr(generateWuBiCode(dto.getName()));
        
        // 设置状态为激活
        entity.setStatusEnum("ACTIVE");
        
        // 设置创建时间
        entity.setCreatedAt(LocalDateTime.now());
        entity.setUpdatedAt(LocalDateTime.now());
        
        // 保存到数据库
        this.save(entity);
        
        return entity.getId();
    }

    @Override
    @Transactional
    public void modifyDefinition(DiagnosisDefinitionDTO dto) {
        if (dto.getId() == null || dto.getId().trim().isEmpty()) {
            throw new IllegalArgumentException("诊断定义ID不能为空");
        }
        
        ClnConditionDefinitionDO entity = this.getById(dto.getId());
        if (entity == null) {
            throw new RuntimeException("诊断定义不存在");
        }
        
        // 更新可编辑字段
        entity.setName(dto.getName());
        entity.setTypeCode(dto.getTypeCode());
        entity.setDiseaseCode(dto.getDiseaseCode());
        entity.setYbFlag(dto.getYbFlag());
        entity.setYbNo(dto.getYbNo());
        entity.setYbMatchFlag(dto.getYbMatchFlag());
        entity.setDescription(dto.getDescription());
        
        // 重新生成拼音码和五笔码
        entity.setPyStr(generatePinYinCode(dto.getName()));
        entity.setWbStr(generateWuBiCode(dto.getName()));
        
        // 设置更新时间
        entity.setUpdatedAt(LocalDateTime.now());
        
        // 更新到数据库
        this.updateById(entity);
    }

    /**
     * 批量启用/停用诊断
     * @param ids 诊断ID列表
     * @param status 状态（ACTIVE/INACTIVE）
     */
    @Override
    @Transactional
    public void batchModifyStatus(List<String> ids, String status) {
        if (ids == null || ids.isEmpty()) {
            throw new IllegalArgumentException("诊断ID列表不能为空");
        }
        
        if (!"ACTIVE".equals(status) && !"INACTIVE".equals(status)) {
            throw new IllegalArgumentException("状态参数错误，应为ACTIVE或INACTIVE");
        }
        
        // 检查每个诊断是否存在
        for (String id : ids) {
            ClnConditionDefinitionDO entity = this.getById(id);
            if (entity == null) {
                throw new RuntimeException("诊断定义不存在，ID: " + id);
            }
            
            // 如果是停用操作，需要检查引用关系
            if ("INACTIVE".equals(status)) {
                // 这里可以添加停用前的检查逻辑
                // 检查诊断是否被其他地方引用
                // if (isBeingReferenced(id)) {
                //     throw new RuntimeException("诊断无法停用: " + id + ", 原因: 正在被引用");
                // }
            }
        }
        
        // 批量更新状态
        LambdaUpdateWrapper<ClnConditionDefinitionDO> updateWrapper = new LambdaUpdateWrapper();
        updateWrapper.in(ClnConditionDefinitionDO::getId, ids);
        updateWrapper.set(ClnConditionDefinitionDO::getStatusEnum, status);
        updateWrapper.set(ClnConditionDefinitionDO::getUpdatedAt, LocalDateTime.now());
        
        this.update(updateWrapper);
    }

    /**
     * 批量删除诊断（软删除，实际上也是停用）
     * @param ids 诊断ID列表
     */
    @Override
    @Transactional
    public void batchDelete(List<String> ids) {
        if (ids == null || ids.isEmpty()) {
            throw new IllegalArgumentException("诊断ID列表不能为空");
        }
        
        // 将删除转换为停用操作
        batchModifyStatus(ids, "INACTIVE");
    }

    @Override
    public Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query) {
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper();
        
        // 添加查询条件
        if (query.getName() != null && !query.getName().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getName, query.getName());
        }
        if (query.getPyStr() != null && !query.getPyStr().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getPyStr, query.getPyStr());
        }
        if (query.getWbStr() != null && !query.getWbStr().trim().isEmpty()) {
            wrapper.like(ClnConditionDefinitionDO::getWbStr, query.getWbStr());
        }
        if (query.getSourceEnum() != null && !query.getSourceEnum().trim().isEmpty()) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, query.getSourceEnum());
        }
        if (query.getTypeCode() != null && !query.getTypeCode().trim().isEmpty()) {
            wrapper.eq(ClnConditionDefinitionDO::getTypeCode, query.getTypeCode());
        }
        // 默认只查询激活状态的诊断
        wrapper.eq(ClnConditionDefinitionDO::getStatusEnum, "ACTIVE");
        
        // 分页查询
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        return this.page(page, wrapper);
    }

    /**
     * 生成诊断编码
     * @return 诊断编码
     */
    private String generateConditionCode() {
        // 生成唯一编码，格式：CDF + 时间戳 + 随机数
        long timestamp = System.currentTimeMillis();
        int randomNum = (int)(Math.random() * 1000);
        return String.format("CDF%013d%03d", timestamp, randomNum);
    }

    /**
     * 生成拼音码
     * @param name 诊断名称
     * @return 拼音码
     */
    private String generatePinYinCode(String name) {
        // 这里应该调用实际的拼音转换工具
        // 暂时返回名称的首字母大写形式作为示例
        if (name == null || name.isEmpty()) {
            return "";
        }
        
        StringBuilder pinyin = new StringBuilder();
        for (char c : name.toCharArray()) {
            if (Character.isLetter(c)) {
                pinyin.append(Character.toUpperCase(c));
            }
        }
        return pinyin.toString();
    }

    /**
     * 生成五笔码
     * @param name 诊断名称
     * @return 五笔码
     */
    private String generateWuBiCode(String name) {
        // 这里应该调用实际的五笔转换工具
        // 暂时返回名称长度作为示例
        if (name == null || name.isEmpty()) {
            return "";
        }
        
        return String.valueOf(name.length());
    }
}