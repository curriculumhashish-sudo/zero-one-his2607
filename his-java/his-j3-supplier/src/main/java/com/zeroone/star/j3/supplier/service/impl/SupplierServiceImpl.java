package com.zeroone.star.j3.supplier.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.j3.supplier.mapper.SupplierMapper;
import com.zeroone.star.j3.supplier.service.ISupplierService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.List;

/**
 * <p>
 * 供应商表 服务实现类
 * </p>
 *
 * @author 业之
 * @since 2026-07-20
 */
@Service
public class SupplierServiceImpl extends ServiceImpl<SupplierMapper, Supplier> implements ISupplierService {

    private static final String ENABLED = "ENABLED";
    private static final String DISABLED = "DISABLED";

    /**
     * 查询供应商名称下拉列表
     * 只展示启用供应商，避免停用数据被选入新增业务单据
     * @param statusEnum 状态枚举：ENABLED启用 / DISABLED停用
     * @param keyword 搜索关键词（名称、拼音码、五笔码）
     * @return 供应商名称简易VO列表
     */
    @Override
    public List<SupplierNameVO> querySupplierNames(String statusEnum, String keyword) {
        String normalizedStatus = normalizeStatus(statusEnum, ENABLED);
        return baseMapper.selectSupplierNames(normalizedStatus, trimToNull(keyword));
    }

    /**
     * 分页查询供应商管理列表
     * 管理页面支持查询启用/停用全部数据，Mapper层统一过滤已软删除数据
     * @param query 查询条件对象
     * @return 分页供应商列表VO
     */
    @Override
    public PageDTO<SupplierListVO> querySupplierList(SupplierQuery query) {
        // 管理列表允许查询启用和停用数据，但 Mapper 层统一过滤软删除记录。
        SupplierQuery normalizedQuery = normalizeQuery(query);
        Page<SupplierListVO> page = new Page<>(normalizedQuery.getPageIndex(), normalizedQuery.getPageSize());
        Page<SupplierListVO> result = baseMapper.selectSupplierPage(page, normalizedQuery);
        return PageDTO.create(result);
    }

    /**
     * 根据供应商ID查询供应商详情
     * 仅返回有效未删除供应商，不存在/已删除返回null，交由控制器统一处理
     * @param id 供应商主键ID
     * @return 供应商详情VO，无数据返回null
     */
    @Override
    public SupplierDetailVO querySupplierDetail(String id) {
        // 详情只返回未删除供应商，已删除或不存在的数据统一返回 null 给 Controller 处理。
        if (!StringUtils.hasText(id)) {
            return null;
        }
        return baseMapper.selectSupplierDetail(id.trim());
    }

    /**
     * 规范化状态参数
     * 校验状态枚举合法性，非法参数直接抛出异常；无入参时使用传入默认值
     * @param statusEnum 原始状态字符串
     * @param defaultValue 为空时使用的默认值
     * @return 清洗后的合法状态值 ENABLED / DISABLED
     * @throws IllegalArgumentException 传入非法状态值抛出异常
     */
    private String normalizeStatus(String statusEnum, String defaultValue) {
        // 状态枚举提前收口，避免非法值直接进入动态 SQL。
        if (!StringUtils.hasText(statusEnum)) {
            return defaultValue;
        }
        String value = statusEnum.trim();
        if (!ENABLED.equals(value) && !DISABLED.equals(value)) {
            throw new IllegalArgumentException("statusEnum only supports ENABLED or DISABLED");
        }
        return value;
    }

    /**
     * 清洗分页查询条件
     * 分页参数兜底、字符串去空格、各字段规范化处理，防止空字符串干扰SQL查询
     * @param query 原始查询条件
     * @return 清洗完成的查询对象
     */
    private SupplierQuery normalizeQuery(SupplierQuery query) {
        // Knife4j 手动测试可能不传分页参数，这里给查询接口兜底。
        SupplierQuery target = query == null ? new SupplierQuery() : query;
        if (target.getPageIndex() < 1) {
            target.setPageIndex(1);
        }
        if (target.getPageSize() < 1) {
            target.setPageSize(10);
        }
        target.setKeyword(trimToNull(target.getKeyword()));
        target.setBusNo(trimToNull(target.getBusNo()));
        target.setName(trimToNull(target.getName()));
        target.setCreditCode(trimToNull(target.getCreditCode()));
        target.setTypeEnum(trimToNull(target.getTypeEnum()));
        target.setStatusEnum(normalizeStatus(target.getStatusEnum(), null));
        target.setOrgId(trimToNull(target.getOrgId()));
        target.setPyStr(trimToNull(target.getPyStr()));
        target.setWbStr(trimToNull(target.getWbStr()));
        target.setPhone(trimToNull(target.getPhone()));
        return target;
    }

    /**
     * 字符串去空格工具方法
     * 空白字符串统一转为null，方便Mybatis动态sql判断条件
     * @param value 原始字符串
     * @return 去除首尾空格后的字符串；空白文本返回null
     */
    private String trimToNull(String value) {
        if (!StringUtils.hasText(value)) {
            return null;
        }
        return value.trim();
    }
}