package com.zeroone.star.j3.supplier.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.supplier.entity.Supplier;
import com.zeroone.star.j3.supplier.mapper.SupplierMapper;
import com.zeroone.star.j3.supplier.service.ISupplierService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.j3.supplier.utils.MsSupplierMapper;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import jakarta.annotation.Resource;

import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigInteger;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Objects;

@Service
public class SupplierServiceImpl extends ServiceImpl<SupplierMapper, Supplier> implements ISupplierService {

    private static final String ENABLED = "ENABLED";
    private static final String DISABLED = "DISABLED";

    @Resource
    private UserHolder userHolder;
    @Resource
    private MsSupplierMapper msSupplierMapper;

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
     * 新增供应商
     * 校验必填字段、信用代码格式与唯一性，在事务中通过悲观锁生成ID，保证并发安全
     * @param dto 供应商完整数据
     * @return 操作结果提示
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addSupplier(SupplierFullDTO dto) {
        // 校验必填字段
        if (!StringUtils.hasText(dto.getName())) {
            throw new IllegalArgumentException("供应商名称不能为空");
        }
        if (!StringUtils.hasText(dto.getTypeEnum())) {
            throw new IllegalArgumentException("供应商类型不能为空");
        }
        if (!StringUtils.hasText(dto.getPyStr())) {
            throw new IllegalArgumentException("拼音码不能为空");
        }
        if (!StringUtils.hasText(dto.getWbStr())) {
            throw new IllegalArgumentException("五笔码不能为空");
        }
        if (!StringUtils.hasText(dto.getOrgId())) {
            throw new IllegalArgumentException("机构标识不能为空");
        }
        if (!StringUtils.hasText(dto.getCreditCode())) {
            throw new IllegalArgumentException("统一信用代码不能为空");
        }
        // 校验信用代码格式：18位数字或大写字母
        if (!dto.getCreditCode().matches("^[0-9A-Z]{18}$")) {
            throw new IllegalArgumentException("统一信用代码格式不正确，应为18位数字或大写字母");
        }
        // 校验信用代码唯一性
        long count = this.lambdaQuery().eq(Supplier::getCreditCode, dto.getCreditCode()).count();
        if (count > 0) {
            throw new IllegalArgumentException("统一信用代码已存在，请检查是否重复录入");
        }

        // DTO → Entity 转换
        Supplier supplier = msSupplierMapper.SupplierFullDTO2Supplier(dto);
        // ID后端自增（基于最大ID +1），在事务中使用悲观锁保证并发安全
        // 如果BusNo为空则设置默认值
        if (!StringUtils.hasText(supplier.getBusNo())) {
            supplier.setBusNo(generateBusNo());
        }
        if (!StringUtils.hasText(supplier.getStatusEnum())) {
            supplier.setStatusEnum(ENABLED);
        }
        supplier.setId(generateId());
        // 强制从登录上下文获取租户ID，失败时直接抛出异常，杜绝信任前端传入值
        supplier.setTenantId(getCurrentTenantId());
        supplier.setVersion(0);
        // 软删除字段初始化
        supplier.setIsDeleted(0);
        supplier.setDeletedBy(null);
        supplier.setDeletedAt(null);
        supplier.setUpdatedBy(null);
        // 设置创建人（从登录用户获取）
        try {
            supplier.setCreatedBy(userHolder.getCurrentUser().getUsername());
        } catch (Exception e) {
            e.printStackTrace();
        }
        //supplier.setCreatedAt(LocalDateTime.now());
        //supplier.setUpdatedAt(LocalDateTime.now());

        this.save(supplier);
        return "新增供应商成功，供应商编号：" + supplier.getBusNo();
    }

    /**
     * 修改供应商
     * 校验供应商存在性与必填业务字段，统一信用代码不允许直接修改
     * @param id  供应商ID
     * @param dto 供应商完整数据
     * @return 操作结果提示
     */
    @Override
    public String updateSupplier(String id, SupplierFullDTO dto) {
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("供应商ID不能为空");
        }
        String tenantId = getCurrentTenantId();
        Long exists = this.lambdaQuery()
                .eq(Supplier::getId, id.trim())
                .eq(Supplier::getTenantId, tenantId)
                .count();
        if (exists == null || exists == 0) {
            throw new IllegalArgumentException("供应商不存在，请检查ID是否正确");
        }
        // 校验必填业务字段
        if (!StringUtils.hasText(dto.getTypeEnum())) {
            throw new IllegalArgumentException("供应商类型不能为空");
        }
        if (!StringUtils.hasText(dto.getPyStr())) {
            throw new IllegalArgumentException("拼音码不能为空");
        }
        if (!StringUtils.hasText(dto.getWbStr())) {
            throw new IllegalArgumentException("五笔码不能为空");
        }
        if (!StringUtils.hasText(dto.getOrgId())) {
            throw new IllegalArgumentException("机构标识不能为空");
        }
        // 如果需要校验信用代码不可修改，单独查询 credit_code 字段
        if (StringUtils.hasText(dto.getCreditCode())) {
            String originalCreditCode = baseMapper.selectCreditCodeById(id.trim());
            if (originalCreditCode != null && !dto.getCreditCode().equals(originalCreditCode)) {
                throw new IllegalArgumentException("统一信用代码不允许直接修改，如需修改请联系管理员");
            }
        }

        // DTO → Entity
        Supplier supplier = msSupplierMapper.SupplierFullDTO2Supplier(dto);
        BeanUtils.copyProperties(dto, supplier);
        supplier.setId(id.trim());
        // 设置更新人/更新时间（从登录用户获取，类似删除处理方式）
        try {
            supplier.setUpdatedBy(userHolder.getCurrentUser().getUsername());
        } catch (Exception e) {
            e.printStackTrace();
        }
        supplier.setTenantId(tenantId);
        this.updateById(supplier);
        return "修改供应商成功";
    }

    /**
     * 删除供应商（软删除）
     * @param id 供应商ID
     * @return 操作结果提示
     */
    @Override
    public String deleteSupplier(String id) {
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("供应商ID不能为空");
        }
        Long count = this.lambdaQuery()
                .eq(Supplier::getId, id.trim())
                .eq(Supplier::getIsDeleted, 0)
                .count();
        if (count == null || count == 0) {
            throw new IllegalArgumentException("供应商不存在，请检查ID是否正确");
        }

        String deletedBy = null;
        try {
            deletedBy = userHolder.getCurrentUser().getUsername();
        } catch (Exception e) {
            e.printStackTrace();
        }

        this.lambdaUpdate()
                .eq(Supplier::getId, id.trim())
                .set(Supplier::getIsDeleted, 1)
                .set(Supplier::getDeletedAt, LocalDateTime.now())
                .set(deletedBy != null, Supplier::getDeletedBy, deletedBy)
                .update();
        return "删除供应商成功";
    }

    /**
     * 批量启/停用供应商
     * @param batchDTO 批量操作参数（ID列表 + 目标状态）
     * @return 操作结果提示
     */
    @Override
    public String batchUpdateSuppliers(SupplierBatchDTO batchDTO) {
        List<String> ids = batchDTO.getIds();
        String status = batchDTO.getStatusEnum();
        if (ids == null || ids.isEmpty()) {
            throw new IllegalArgumentException("请选择需要操作的供应商");
        }
        // 规范化状态参数
        String normalizedStatus = normalizeStatus(status, ENABLED);
        // 查询待操作的供应商
        List<Supplier> suppliers = this.lambdaQuery()
                .in(Supplier::getId, ids)
                .ne(Supplier::getStatusEnum, normalizedStatus)
                .list();
        int size = suppliers.size();
        if (size == 0) {
            return normalizedStatus.equals(ENABLED)?"所选供应商中已全部启用，无需重复操作":"所选供应商中已全部停用，无需重复操作";
        }
        // 批量更改状态
        this.lambdaUpdate()
                .in(Supplier::getId, ids)
                .set(Supplier::getStatusEnum, normalizedStatus)
                .update();

        String statusText = ENABLED.equals(normalizedStatus) ? "启用" : "停用";
        return "成功批量" + statusText + " " + size + " 个供应商";
    }

    /**
     * 从当前登录上下文获取租户ID
     * 获取失败时直接抛出异常，杜绝信任前端传入的租户ID
     */
    private String getCurrentTenantId() {
        try {
            return userHolder.getCurrentTenantId();
        } catch (Exception e) {
            throw new IllegalStateException("无法获取当前租户信息，请重新登录", e);
        }
    }

    /**
     * 生成主键ID：基于当前最大ID自增+1，使用数据库悲观锁保证并发安全
     * 必须在 @Transactional 事务中调用，FOR UPDATE 锁在事务提交时释放
     * @return 新的主键ID
     */
    private String generateId() {
        // 使用 SELECT COALESCE(MAX(id), '4100000000000000001') ... FOR UPDATE
        // 悲观锁锁定行，防止并发请求读到相同最大值
        // COALESCE 处理空表场景，返回初始值前一位，+1 后得到 '4100000000000000002'
        String maxId = baseMapper.selectMaxIdForUpdate();
        return new BigInteger(maxId).add(BigInteger.ONE).toString();
    }

    /**
     * 生成业务编号：SUPP + 自增序号
     * 初始3位数字（SUPP002、SUPP003...），超出999后自动扩大位数
     * @return 业务编号
     */
    private String generateBusNo() {
        String prefix = "SUPP";

        // 查询最大业务编号
        Supplier maxSupplier = this.lambdaQuery()
                .likeRight(Supplier::getBusNo, prefix)
                .orderByDesc(Supplier::getBusNo)
                .last("LIMIT 1")
                .one();

        int sequence = 1;
        if (maxSupplier != null && maxSupplier.getBusNo() != null) {
            sequence = Integer.parseInt(maxSupplier.getBusNo().substring(prefix.length())) + 1;
        }
        // 最小3位，超出时按实际位数（如SUPP1000）
        String format = "%0" + Math.max(3, String.valueOf(sequence).length()) + "d";
        return prefix + String.format(format, sequence);
    }

    /**
     * 规范化状态参数
     * 校验状态枚举合法性，非法参数直接抛出异常；无入参时使用传入默认值
     * @param statusEnum  原始状态字符串
     * @param defaultValue 为空时使用的默认值
     * @return 清洗后的合法状态值 ENABLED / DISABLED
     * @throws IllegalArgumentException 传入非法状态值抛出异常
     */
    private String normalizeStatus(String statusEnum, String defaultValue) {
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
