package com.zeroone.star.setroster.service.impl;


import cn.hutool.core.util.IdUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j8.setroster.AddDailyScheduleDTO;
import com.zeroone.star.project.dto.j8.setroster.DailyScheduleDTO;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleOptionQuery;
import com.zeroone.star.project.query.j8.setroster.DailyScheduleQuery;
import com.zeroone.star.project.vo.j8.setroster.DailySchedulePageVO;
import com.zeroone.star.project.vo.j8.setroster.DailyScheduleVO;
import com.zeroone.star.setroster.entity.BkgScheduleTemplate;
import com.zeroone.star.setroster.mapper.BkgScheduleTemplateMapper;
import com.zeroone.star.setroster.service.IBkgScheduleTemplateService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * T2 排班日模板（07 §5.3） 服务实现类
 * </p>
 *
 * @author 墨
 * @since 2026-07-21
 */
@Service
public class BkgScheduleTemplateServiceImpl extends ServiceImpl<BkgScheduleTemplateMapper, BkgScheduleTemplate> implements IBkgScheduleTemplateService {

    @Resource
    private BkgScheduleTemplateConvert convert;
    @Resource
    private UserHolder userHolder;

    /**
     * 获取排班日模板选项列表
     * @return 排班日模板选项列表
     */
    @Override
    public DailySchedulePageVO<DailyScheduleVO> listDailyScheduleOptions(DailyScheduleOptionQuery query) {
        long pageIndex = query.getPageIndex() != null ? query.getPageIndex() : 1;
        long pageSize = query.getPageSize() != null ? query.getPageSize() : 200;

        Page<BkgScheduleTemplate> page = new Page<>(pageIndex, pageSize);
        LambdaQueryWrapper<BkgScheduleTemplate> lqw = new LambdaQueryWrapper<>();
        lqw.eq(BkgScheduleTemplate::getEnabled, true);

        if (StringUtils.hasText(query.getOrganizationId())) {
            lqw.and(w -> w.eq(BkgScheduleTemplate::getOrganizationId, query.getOrganizationId())
                    .or().isNull(BkgScheduleTemplate::getOrganizationId));
        }
        String kw = query.getKeyword();
        if (StringUtils.hasText(kw)) {
            lqw.and(wrapper -> wrapper
                    .like(BkgScheduleTemplate::getTemplateCode, kw)
                    .or()
                    .like(BkgScheduleTemplate::getTemplateName, kw));
        }
        lqw.last("ORDER BY `template_name` ASC");

        Page<BkgScheduleTemplate> result = baseMapper.selectPage(page, lqw);

        DailySchedulePageVO<DailyScheduleVO> vo = new DailySchedulePageVO<>();
        vo.setPageIndex(result.getCurrent());
        vo.setPageSize(result.getSize());
        vo.setTotalCount(result.getTotal());
        vo.setContent(result.getRecords().stream()
                .map(convert::toVo)
                .toList());
        return vo;
    }

    /**
     * 分页获取排班日模板列表(条件+分页)
     *
     * @param query 查询参数
     * @return 排班日模板列表
     */
    @Override
    public DailySchedulePageVO<DailyScheduleVO> pageDailySchedule(DailyScheduleQuery query) {
        //1 构造分页对象
        Page<BkgScheduleTemplate> page = new Page<>(query.getPageIndex(), query.getPageSize());

        //2 构建查询条件
        LambdaQueryWrapper<BkgScheduleTemplate> lqw = new LambdaQueryWrapper<>();
        if (StringUtils.hasText(query.getOrganizationId())) {
            lqw.and(w -> w.eq(BkgScheduleTemplate::getOrganizationId, query.getOrganizationId())
                    .or().isNull(BkgScheduleTemplate::getOrganizationId));
        }
        String kw = query.getKeyword();
        if (StringUtils.hasText(kw)) {     //关键词非空时才拼接；OR 连接 编码/名称 两个模糊匹配
            lqw.and(wrapper -> wrapper
                    .like(BkgScheduleTemplate::getTemplateCode, kw)
                    .or()
                    .like(BkgScheduleTemplate::getTemplateName, kw));
        }
        lqw.last("ORDER BY IFNULL(`updated_at`,`created_at`) DESC, `id` ASC");

        //3 执行查询
        Page<BkgScheduleTemplate> result = baseMapper.selectPage(page, lqw);

        //4 手动转换为 DailySchedulePageVO<DailyScheduleVO>
        DailySchedulePageVO<DailyScheduleVO> vo = new DailySchedulePageVO<>();
        vo.setPageIndex(result.getCurrent());
        vo.setPageSize(result.getSize());
        vo.setTotalCount(result.getTotal());
        vo.setContent(result.getRecords().stream()
                    .map(convert::toVo)
                    .toList());

        return vo;

    }

    /**
     * 新增排班日模板
     * @param dto 新增参数
     * @return 新增结果
     */
    @Override
    public String addDailySchedule(AddDailyScheduleDTO dto) {
        BkgScheduleTemplate entity = convert.toEntity(dto);

        entity.setTenantId("1");                         // TODO：租户取值来源待确认
        entity.setTemplateCode("T2-" + IdUtil.fastUUID()); // 业务编码：前缀 T2 + 唯一 UUID
        if (entity.getEnabled() == null) {
            entity.setEnabled(true);  // 启用状态缺省为 true（与新建弹窗默认 ON 一致）
        }
        if (entity.getPriority() == null) {
            entity.setPriority(0);   // 优先级缺省 0（与 DB DEFAULT 保持一致），保证 NOT NULL 不被依赖默认值
        }
        entity.setEffectiveStart(LocalDate.now());
        //entity.setEffectiveEnd(LocalDate.now().plusYears(1)); //TODO 暂时默认一年后失效

        boolean success = save(entity);
        return success ? entity.getId() : null;

    }

    /**
     * 修改排班日模板
     * @param dto 修改参数
     * @return 修改结果
     */
    @Override
    public String modifyDailySchedule(DailyScheduleDTO dto) {
        BkgScheduleTemplate entity = getById(dto.getId());
        if (entity == null){
            return null;
        }
        convert.updateEntity(dto, entity);
        updateById(entity);
        return entity.getId();
    }

    /**
     * 删除排班日模板
     * @param id 删除参数
     * @return 删除结果
     */
    @Override
    public String deleteDailySchedule(String id) {
        boolean updated = lambdaUpdate()
                .eq(BkgScheduleTemplate::getId, id)
                .set(BkgScheduleTemplate::getDeletedAt, LocalDateTime.now())
                .set(BkgScheduleTemplate::getDeletedBy, currentUserId())
                .set(BkgScheduleTemplate::getIsDeleted, true)
                .update();
        return updated ? id : null;
    }

    /**
     * 当前登录用户名；无登录态（本地自测/无网关头）时回退为 "system"，保证软删可登记删除人。
     */
    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getUsername();
            }
        } catch (Exception ignored) {
        }
        return "system";
    }

}
