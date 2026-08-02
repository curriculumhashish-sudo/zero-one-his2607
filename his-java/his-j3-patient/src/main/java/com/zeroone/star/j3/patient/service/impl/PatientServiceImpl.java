package com.zeroone.star.j3.patient.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.patient.entity.PatientDO;
import com.zeroone.star.j3.patient.exception.PatientBusinessException;
import com.zeroone.star.j3.patient.mapper.PatientMapper;
import com.zeroone.star.j3.patient.service.PatientService;
import com.zeroone.star.j3.patient.util.PatientNameCodeUtils;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;
import com.zeroone.star.project.vo.j3.patient.PatientOptionVO;
import com.zeroone.star.project.vo.j3.patient.PatientVO;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.Period;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.Set;

/**
 * 患者管理业务实现。
 *
 * @author 照夜清
 */
@Service
@RequiredArgsConstructor
public class PatientServiceImpl implements PatientService {

    private static final String STATUS_ACTIVE = "ACTIVE";
    private static final Set<String> MODIFIABLE_STATUSES =
            Set.of("DRAFT", STATUS_ACTIVE);
    private static final Set<String> GENDER_CODES =
            Set.of("MALE", "FEMALE", "UNKNOWN");
    private static final String PATIENT_NUMBER_PREFIX = "PA";

    private final PatientMapper patientMapper;
    private final UserHolder userHolder;

    @Override
    public PageDTO<PatientListVO> queryPatientPage(PatientQuery query) {
        normalizeQuery(query);
        Page<PatientDO> page = patientMapper.selectPatientPage(
                new Page<>(query.getPageIndex(), query.getPageSize()),
                query,
                userHolder.getCurrentTenantId()
        );

        List<PatientListVO> rows = page.getRecords().stream()
                .map(this::toListVO)
                .toList();
        return new PageDTO<>(
                page.getCurrent(),
                page.getSize(),
                page.getTotal(),
                page.getPages(),
                rows
        );
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String createPatient(PatientCreateDTO dto) {
        NormalizedPatient normalized = normalizeAndValidate(dto);
        String tenantId = userHolder.getCurrentTenantId();
        requireActiveOrganization(
                normalized.organizationId(),
                tenantId
        );
        ensureIdCardAvailable(
                normalized.idCard(),
                null,
                tenantId
        );

        LocalDateTime now = LocalDateTime.now();
        PatientDO patient = new PatientDO();
        BeanUtils.copyProperties(dto, patient);
        applyPrototypeFields(patient, dto);
        applyNormalizedValues(patient, normalized);
        patient.setTenantId(tenantId);
        patient.setBusNo(nextPatientNumber(tenantId, now));
        patient.setStatusEnum(STATUS_ACTIVE);
        patient.setActiveFlag(true);
        patient.setTempFlag(false);
        patient.setVersion(0);
        patient.setIsDeleted(false);
        patient.setCreatedAt(now);
        patient.setUpdatedAt(now);
        patient.setPyStr(PatientNameCodeUtils.toPinyinInitials(
                normalized.name()
        ));

        if (patientMapper.insert(patient) != 1) {
            throw new PatientBusinessException("患者建档失败，请稍后重试");
        }
        return patient.getId();
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void updatePatient(String id, PatientUpdateDTO dto) {
        if (!StringUtils.hasText(id)) {
            throw new PatientBusinessException("患者ID不能为空");
        }

        String tenantId = userHolder.getCurrentTenantId();
        PatientDO patient = patientMapper.selectPatientByIdAndTenant(
                id.trim(),
                tenantId
        );
        if (patient == null) {
            throw new PatientBusinessException("患者不存在或已删除");
        }
        if (!MODIFIABLE_STATUSES.contains(patient.getStatusEnum())) {
            throw new PatientBusinessException(
                    "当前患者状态不允许修改基本资料"
            );
        }
        if (dto.getVersion() != null
                && !patient.getVersion().equals(dto.getVersion())) {
            throw new PatientBusinessException(
                    "患者资料已被其他人修改，请刷新后重试"
            );
        }

        NormalizedPatient normalized = normalizeAndValidate(dto);
        requireUnchangedState(patient, dto, normalized.organizationId());
        requireActiveOrganization(
                normalized.organizationId(),
                tenantId
        );
        ensureIdCardAvailable(
                normalized.idCard(),
                patient.getId(),
                tenantId
        );

        BeanUtils.copyProperties(
                dto,
                patient,
                "statusEnum",
                "activeFlag",
                "tempFlag",
                "version",
                "organizationId"
        );
        applyPrototypeFields(patient, dto);
        applyNormalizedValues(patient, normalized);
        patient.setUpdatedAt(LocalDateTime.now());
        patient.setPyStr(PatientNameCodeUtils.toPinyinInitials(
                normalized.name()
        ));

        if (patientMapper.updateById(patient) != 1) {
            throw new PatientBusinessException(
                    "患者资料版本冲突，请刷新后重试"
            );
        }
    }

    @Override
    public PageDTO<PatientOptionVO> queryPatients(PatientQuery query) {
        normalizeQuery(query);
        Page<PatientDO> page = patientMapper.selectPatientPage(
                new Page<>(query.getPageIndex(), query.getPageSize()),
                query,
                userHolder.getCurrentTenantId()
        );//根据条件查询患者
        List<PatientOptionVO> rows = page.getRecords().stream()
                .map(this::toOptionVO)
                .toList();//将患者数据转换成患者选择的数据格式

        return new PageDTO<>(
                page.getCurrent(),
                page.getSize(),
                page.getTotal(),
                page.getPages(),
                rows
        );
    }

    @Override
    public PatientVO getPatientById(String id) {//根据id获取患者详情
        PatientDO patient = patientMapper.selectById(id);
        if (patient == null || !Objects.equals(patient.getTenantId(), userHolder.getCurrentTenantId())) {
            throw new PatientBusinessException("患者不存在或已删除");
        }
        PatientVO result = new PatientVO();
        BeanUtils.copyProperties(patient, result);
        return result;
    }
    @Override
    public void deletePatientById(String id) {//根据id删除患者
        if (!StringUtils.hasText(id)) {
            throw new PatientBusinessException("患者ID不能为空");
        }
        PatientDO patient = patientMapper.selectById(id.trim());
        if (patient == null|| !Objects.equals(patient.getTenantId(), userHolder.getCurrentTenantId())) {
            throw new PatientBusinessException("患者不存在或已删除");
        }
        int activeEncounters = patientMapper.countActiveEncounters(
                id.trim(),
                userHolder.getCurrentTenantId()
        );
        if (activeEncounters > 0) {
            throw new PatientBusinessException(
                    "该患者存在进行中的就诊记录（挂号/住院），暂不能删除"
            );
        }
        int allEncounters = patientMapper.countAllEncounters(
                id.trim(),
                userHolder.getCurrentTenantId()
        );
        int chargeItems = patientMapper.countChargeItems(
                id.trim(),
                userHolder.getCurrentTenantId()
        );
        int invoices = patientMapper.countInvoices(
                id.trim(),
                userHolder.getCurrentTenantId()
        );
        if (allEncounters > 0 || chargeItems > 0 || invoices > 0) {
            throw new PatientBusinessException(
                    "该患者存在历史业务记录（就诊/收费/发票），不允许删除。如需处理请先归档或合并患者档案"
            );
        }
        patientMapper.deleteById(id.trim());
    }

    private PatientOptionVO toOptionVO(PatientDO patientDO) {
        PatientOptionVO result = new PatientOptionVO();
        BeanUtils.copyProperties(patientDO, result);
        return result;
    }

    private void normalizeQuery(PatientQuery query) {
        query.setKeyword(trimToNull(query.getKeyword()));
        query.setName(trimToNull(query.getName()));
        query.setBusNo(trimToNull(query.getBusNo()));
        query.setIdCard(normalizeIdCard(query.getIdCard()));
        query.setPhone(trimToNull(query.getPhone()));
        query.setGenderEnum(toUpperCase(query.getGenderEnum()));
        query.setStatusEnum(toUpperCase(query.getStatusEnum()));
        query.setOrganizationId(trimToNull(query.getOrganizationId()));
    }

    private PatientListVO toListVO(PatientDO patient) {
        PatientListVO result = new PatientListVO();
        BeanUtils.copyProperties(patient, result);
        result.setAge(calculateAge(patient.getBirthDate()));
        result.setGenderEnumText(genderText(patient.getGenderEnum()));
        return result;
    }

    private NormalizedPatient normalizeAndValidate(PatientCreateDTO dto) {
        String name = trimToNull(dto.getName());
        String genderEnum = toUpperCase(dto.getGenderEnum());
        String organizationId = trimToNull(dto.getOrganizationId());
        String idCard = normalizeIdCard(dto.getIdCard());
        String phone = trimToNull(dto.getPhone());
        String linkTelcom = trimToNull(dto.getLinkTelcom());

        if (name == null) {
            throw new PatientBusinessException("患者姓名不能为空");
        }
        if (!GENDER_CODES.contains(genderEnum)) {
            throw new PatientBusinessException("性别编码不合法");
        }
        validateIdCard(idCard);
        if (dto.getBirthDate() == null) {
            throw new PatientBusinessException("出生日期不能为空");
        }
        if (dto.getAge() == null) {
            throw new PatientBusinessException("年龄不能为空");
        }
        if (dto.getAge() < 0 || dto.getAge() > 150) {
            throw new PatientBusinessException("年龄必须在0到150之间");
        }
        if (dto.getBirthDate().isAfter(LocalDate.now())) {
            throw new PatientBusinessException("出生日期不能晚于当前日期");
        }
        if (!dto.getAge().equals(calculateAge(dto.getBirthDate()))) {
            throw new PatientBusinessException("年龄与出生日期不一致");
        }
        if (dto.getDeceasedDate() != null
                && dto.getDeceasedDate().toLocalDate()
                .isBefore(dto.getBirthDate())) {
            throw new PatientBusinessException("死亡日期不能早于出生日期");
        }
        if (organizationId == null) {
            throw new PatientBusinessException("建档机构ID不能为空");
        }
        validateTelephone(phone, "联系电话");
        validateTelephone(linkTelcom, "联系人电话");

        return new NormalizedPatient(
                name,
                genderEnum,
                organizationId,
                idCard,
                phone,
                linkTelcom
        );
    }

    private void requireActiveOrganization(
            String organizationId,
            String tenantId
    ) {
        String organizationTenantId = patientMapper
                .selectTenantIdByActiveOrganizationId(
                        organizationId,
                        tenantId
                );
        if (!StringUtils.hasText(organizationTenantId)) {
            throw new PatientBusinessException(
                    "建档机构不存在、已停用、已删除或不属于当前租户"
            );
        }
    }

    private void ensureIdCardAvailable(
            String idCard,
            String excludedPatientId,
            String tenantId
    ) {
        String occupiedPatientId = patientMapper
                .selectIdByIdCardForUpdate(
                        idCard,
                        excludedPatientId,
                        tenantId
                );
        if (occupiedPatientId != null) {
            throw new PatientBusinessException(
                    "该身份证号已存在患者档案"
            );
        }
    }

    private String nextPatientNumber(
            String tenantId,
            LocalDateTime now
    ) {
        String sequenceKey = StringUtils.hasText(tenantId)
                ? tenantId + ":" + PATIENT_NUMBER_PREFIX
                : PATIENT_NUMBER_PREFIX;
        Long currentValue =
                patientMapper.selectSequenceValueForUpdate(sequenceKey);
        long nextValue;
        if (currentValue == null) {
            nextValue = 1L;
            if (patientMapper.insertSequence(sequenceKey, nextValue, now)
                    != 1) {
                throw new PatientBusinessException(
                        "患者编号流水初始化失败"
                );
            }
        } else {
            nextValue = currentValue + 1L;
            if (patientMapper.updateSequence(
                    sequenceKey,
                    nextValue,
                    now
            ) != 1) {
                throw new PatientBusinessException(
                        "患者编号流水更新失败"
                );
            }
        }
        return PATIENT_NUMBER_PREFIX + "%08d".formatted(nextValue);
    }

    private void requireUnchangedState(
            PatientDO patient,
            PatientUpdateDTO dto,
            String normalizedOrganizationId
    ) {
        if (!patient.getOrganizationId().equals(normalizedOrganizationId)) {
            throw new PatientBusinessException(
                    "普通资料修改不允许更换建档机构"
            );
        }
        if (dto.getStatusEnum() != null
                && !patient.getStatusEnum().equals(
                        toUpperCase(dto.getStatusEnum())
                )) {
            throw new PatientBusinessException(
                    "普通资料修改不允许变更患者状态"
            );
        }
        if (dto.getActiveFlag() != null
                && !patient.getActiveFlag().equals(dto.getActiveFlag())) {
            throw new PatientBusinessException(
                    "普通资料修改不允许变更启用标记"
            );
        }
        if (dto.getTempFlag() != null
                && !patient.getTempFlag().equals(dto.getTempFlag())) {
            throw new PatientBusinessException(
                    "普通资料修改不允许变更临时建档标记"
            );
        }
    }

    private void applyNormalizedValues(
            PatientDO patient,
            NormalizedPatient normalized
    ) {
        patient.setName(normalized.name());
        patient.setGenderEnum(normalized.genderEnum());
        patient.setOrganizationId(normalized.organizationId());
        patient.setIdCard(normalized.idCard());
        patient.setPhone(normalized.phone());
        patient.setLinkTelcom(normalized.linkTelcom());
        patient.setMaritalStatusEnum(trimToNull(
                patient.getMaritalStatusEnum()
        ));
        patient.setPrfsEnum(trimToNull(patient.getPrfsEnum()));
        patient.setAddress(trimToNull(patient.getAddress()));
        patient.setAddressProvince(trimToNull(
                patient.getAddressProvince()
        ));
        patient.setAddressCity(trimToNull(patient.getAddressCity()));
        patient.setAddressDistrict(trimToNull(
                patient.getAddressDistrict()
        ));
        patient.setAddressStreet(trimToNull(patient.getAddressStreet()));
        patient.setNationalityCode(trimToNull(
                patient.getNationalityCode()
        ));
        patient.setWorkCompany(trimToNull(patient.getWorkCompany()));
        patient.setNativePlace(trimToNull(patient.getNativePlace()));
        patient.setCountryCode(trimToNull(patient.getCountryCode()));
        patient.setLinkName(trimToNull(patient.getLinkName()));
    }

    private void applyPrototypeFields(
            PatientDO patient,
            PatientCreateDTO dto
    ) {
        patient.setBloodTypeAbo(dto.getBloodAbo());
        patient.setBloodTypeRh(dto.getBloodRh());
        patient.setDeceasedDate(dto.getDeceasedDate());
    }

    private void validateIdCard(String idCard) {
        if (idCard == null) {
            throw new PatientBusinessException("证件号不能为空");
        }
        if (!idCard.matches("(?:\\d{15}|\\d{17}[\\dX])")) {
            throw new PatientBusinessException(
                    "证件号格式不正确，应为15位或18位身份证号"
            );
        }
    }

    private void validateTelephone(String telephone, String fieldName) {
        if (telephone != null
                && !telephone.matches("[0-9+()\\-\\s]{6,32}")) {
            throw new PatientBusinessException(fieldName + "格式不正确");
        }
    }

    private String normalizeIdCard(String idCard) {
        String normalized = trimToNull(idCard);
        return normalized == null
                ? null
                : normalized.toUpperCase(Locale.ROOT);
    }

    private String toUpperCase(String value) {
        String normalized = trimToNull(value);
        return normalized == null
                ? null
                : normalized.toUpperCase(Locale.ROOT);
    }

    private String trimToNull(String value) {
        if (value == null) {
            return null;
        }
        String normalized = value.trim();
        return normalized.isEmpty() ? null : normalized;
    }

    private Integer calculateAge(LocalDate birthDate) {
        if (birthDate == null) {
            return null;
        }
        return Period.between(birthDate, LocalDate.now()).getYears();
    }

    private String genderText(String genderEnum) {
        if (genderEnum == null) {
            return null;
        }
        return switch (genderEnum) {
            case "MALE" -> "男性";
            case "FEMALE" -> "女性";
            case "UNKNOWN" -> "未知";
            default -> genderEnum;
        };
    }

    private record NormalizedPatient(
            String name,
            String genderEnum,
            String organizationId,
            String idCard,
            String phone,
            String linkTelcom
    ) {
    }
}
