<template>
  <MyDialog
    ref="dialogRef"
    :data="form"
    :title="form.id ? '编辑药品' : '新增药品'"
    width="900px"
    draggable
    :close-on-click-modal="false"
  >
    <template #header>
      <strong>{{ form.id ? '编辑药品' : '新增药品' }}</strong>
    </template>

    <template #default>
      <el-form
        ref="formRef"
        v-loading="loading"
        :model="form"
        :rules="rules"
        label-width="110px"
        status-icon
      >
        <el-tabs v-model="activeTab">
          <!-- ==================== 基本信息 ==================== -->
          <el-tab-pane label="基本信息" name="basic">
            <el-row :gutter="18">
              <el-col :span="12">
                <el-form-item label="通用名称" prop="name">
                  <el-input v-model="form.name" maxlength="60" show-word-limit placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="商品名称" prop="merchandiseName">
                  <el-input v-model="form.merchandiseName" maxlength="60" show-word-limit placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="药品分类" prop="categoryCode">
                  <el-select v-model="form.categoryCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.categoryOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="供应商" prop="supplyId">
                  <el-select v-model="form.supplyId" filterable clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.supplierOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="生产厂家" prop="manufacturerText">
                  <el-input v-model="form.manufacturerText" maxlength="80" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="规格" prop="totalVolume">
                  <el-input v-model="form.totalVolume" maxlength="40" placeholder="如 0.25g*50粒" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="剂型" prop="doseFormCode">
                  <el-select v-model="form.doseFormCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.doseFormOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="批准文号" prop="approvalNumber">
                  <el-input v-model="form.approvalNumber" maxlength="40" placeholder="国药准字..." />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="国家药品编码" prop="nationalDrugCode">
                  <el-input v-model="form.nationalDrugCode" maxlength="20" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="69码" prop="drug69Code">
                  <el-input v-model="form.drug69Code" maxlength="20" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="药品性质" prop="characteristic">
                  <el-select v-model="form.characteristic" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.propertyOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="生命周期状态" prop="statusEnum">
                  <el-select v-model="form.statusEnum" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.statusOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="适用领域" prop="domainEnum">
                  <el-select v-model="form.domainEnum" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.domainOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="启用状态" prop="activeFlag">
                  <el-switch v-model="form.activeFlag" inline-prompt active-text="启用" inactive-text="停用" />
                </el-form-item>
              </el-col>
              <el-col :span="24">
                <el-form-item label="药品说明" prop="medicationDefinition">
                  <el-input v-model="form.medicationDefinition" type="textarea" :rows="2" maxlength="200" show-word-limit placeholder="请输入" />
                </el-form-item>
              </el-col>
            </el-row>
          </el-tab-pane>

          <!-- ==================== 医保信息 ==================== -->
          <el-tab-pane label="医保信息" name="insurance">
            <el-row :gutter="18">
              <el-col :span="12">
                <el-form-item label="医保编码" prop="ybNo">
                  <el-input v-model="form.ybNo" maxlength="20" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="是否已对码" prop="ybMatchFlag">
                  <el-switch v-model="form.ybMatchFlag" inline-prompt active-text="是" inactive-text="否" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="医保等级" prop="chrgitmLv">
                  <el-select v-model="form.chrgitmLv" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.ybLevelOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="结算类型" prop="settlementType">
                  <el-select v-model="form.settlementType" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.settlementTypeOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="医保类别" prop="ybCategory">
                  <el-select v-model="form.ybCategory" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.ybCategoryOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="财务类型" prop="financialTypeCode">
                  <el-select v-model="form.financialTypeCode" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.financialTypeOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
            </el-row>
          </el-tab-pane>

          <!-- ==================== 用法用量 ==================== -->
          <el-tab-pane label="用法用量" name="usage">
            <el-row :gutter="18">
              <el-col :span="12">
                <el-form-item label="默认用法" prop="methodCode">
                  <el-select v-model="form.methodCode" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.methodOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="用药频次" prop="rateCode">
                  <el-select v-model="form.rateCode" clearable placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.rateOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="单次剂量" prop="dose">
                  <el-input-number v-model="form.dose" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="单次最大量" prop="maxUnit">
                  <el-input v-model="form.maxUnit" maxlength="20" placeholder="如 2片" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="用量限定" prop="usageLimit">
                  <el-input v-model="form.usageLimit" maxlength="40" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="限制范围" prop="restrictedScope">
                  <el-input v-model="form.restrictedScope" maxlength="40" placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="24">
                <el-form-item label="用法说明" prop="usageInstruction">
                  <el-input v-model="form.usageInstruction" type="textarea" :rows="2" maxlength="200" show-word-limit placeholder="请输入" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="皮试药" prop="skinTestFlag">
                  <el-switch v-model="form.skinTestFlag" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="注射药" prop="injectFlag">
                  <el-switch v-model="form.injectFlag" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="处方药" prop="rxFlag">
                  <el-switch v-model="form.rxFlag" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="基本药物" prop="basicFlag">
                  <el-switch v-model="form.basicFlag" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="抗生素" prop="antibioticFlag">
                  <el-switch v-model="form.antibioticFlag" />
                </el-form-item>
              </el-col>
              <el-col :span="8">
                <el-form-item label="儿童用药" prop="childrenFlag">
                  <el-switch v-model="form.childrenFlag" />
                </el-form-item>
              </el-col>
            </el-row>
          </el-tab-pane>

          <!-- ==================== 价格与单位 ==================== -->
          <el-tab-pane label="价格与单位" name="price">
            <el-row :gutter="18">
              <el-col :span="12">
                <el-form-item label="购入价" prop="purchasePrice">
                  <el-input-number v-model="form.purchasePrice" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="零售价" prop="retailPrice">
                  <el-input-number v-model="form.retailPrice" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="最高零售价" prop="maxRetailPrice">
                  <el-input-number v-model="form.maxRetailPrice" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="采购单位" prop="purchaseUnitCode">
                  <el-select v-model="form.purchaseUnitCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.purchaseUnitOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="采购换算比" prop="purchaseUnitRatio">
                  <el-input-number v-model="form.purchaseUnitRatio" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="库存单位" prop="stockUnitCode">
                  <el-select v-model="form.stockUnitCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.stockUnitOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="库存换算比" prop="stockUnitRatio">
                  <el-input-number v-model="form.stockUnitRatio" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="包装单位" prop="packageUnitCode">
                  <el-select v-model="form.packageUnitCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.packageUnitOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="包装换算比" prop="packageUnitRatio">
                  <el-input-number v-model="form.packageUnitRatio" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="拆零单位" prop="splitUnitCode">
                  <el-select v-model="form.splitUnitCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.splitUnitOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="剂量单位" prop="doseUnitCode">
                  <el-select v-model="form.doseUnitCode" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.doseUnitOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="剂量换算比" prop="doseUnitRatio">
                  <el-input-number v-model="form.doseUnitRatio" :min="0" :precision="4" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="门诊拆分属性" prop="partAttributeEnum">
                  <el-select v-model="form.partAttributeEnum" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.partAttributeOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="住院拆分属性" prop="thoPartAttributeEnum">
                  <el-select v-model="form.thoPartAttributeEnum" placeholder="请选择" style="width: 100%">
                    <el-option
                      v-for="opt in options.thoPartAttributeOptions"
                      :key="opt.value"
                      :label="opt.label"
                      :value="opt.value"
                    />
                  </el-select>
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="警戒上限" prop="itemMaxQuantity">
                  <el-input-number v-model="form.itemMaxQuantity" :min="0" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="警戒下限" prop="itemMinQuantity">
                  <el-input-number v-model="form.itemMinQuantity" :min="0" controls-position="right" style="width: 100%" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="拼音码" prop="pyStr">
                  <el-input v-model="form.pyStr" maxlength="20" placeholder="自动生成或手填" />
                </el-form-item>
              </el-col>
              <el-col :span="12">
                <el-form-item label="五笔码" prop="wbStr">
                  <el-input v-model="form.wbStr" maxlength="20" placeholder="自动生成或手填" />
                </el-form-item>
              </el-col>
            </el-row>
          </el-tab-pane>
        </el-tabs>
      </el-form>
    </template>

    <template #footer>
      <el-button :disabled="saving" @click="close">取消</el-button>
      <el-button type="primary" :loading="saving" :disabled="loading" @click="submit">
        保存
      </el-button>
    </template>
  </MyDialog>
</template>

<script setup lang="ts">
import { nextTick, reactive, ref } from 'vue'
import type { FormInstance, FormRules } from 'element-plus'
import { ElMessage } from 'element-plus'
import MyDialog from '@/components/mydialog/MyDialog.vue'
import {
  createMedicationCatalog,
  getMedicationCatalogDetail,
  loadMedicationCatalogFormOptions,
  updateMedicationCatalog
} from '@/apis/basicdata'
import type { MedicationCatalogFormOptions } from '@/apis/basicdata'
import { createEmptyMedicationCatalogForm, toMedicationCatalogForm } from '../adapter'
import type { MedicationCatalogDialogOpenOptions, MedicationCatalogFormModel } from '../type'

interface DialogExpose {
  openDialog: () => void
  closeDialog: () => void
}

const emit = defineEmits<{ saved: [] }>()
const dialogRef = ref<DialogExpose | null>(null)
const formRef = ref<FormInstance>()
const form = reactive<MedicationCatalogFormModel>(createEmptyMedicationCatalogForm())
const options = reactive<MedicationCatalogFormOptions>({
  categoryOptions: [],
  supplierOptions: [],
  financialTypeOptions: [],
  propertyOptions: [],
  doseUnitOptions: [],
  doseFormOptions: [],
  methodOptions: [],
  rateOptions: [],
  purchaseUnitOptions: [],
  stockUnitOptions: [],
  packageUnitOptions: [],
  splitUnitOptions: [],
  partAttributeOptions: [],
  thoPartAttributeOptions: [],
  statusOptions: [],
  domainOptions: [],
  ybLevelOptions: [],
  settlementTypeOptions: [],
  ybCategoryOptions: [],
  dddUnitOptions: []
})
const loading = ref(false)
const saving = ref(false)
const activeTab = ref('basic')

const rules: FormRules<MedicationCatalogFormModel> = {
  name: [{ required: true, message: '请输入通用名称', trigger: 'blur' }],
  merchandiseName: [{ required: true, message: '请输入商品名称', trigger: 'blur' }],
  categoryCode: [{ required: true, message: '请选择药品分类', trigger: 'change' }],
  supplyId: [{ required: true, message: '请选择供应商', trigger: 'change' }],
  totalVolume: [{ required: true, message: '请输入规格', trigger: 'blur' }],
  doseFormCode: [{ required: true, message: '请选择剂型', trigger: 'change' }],
  statusEnum: [{ required: true, message: '请选择生命周期状态', trigger: 'change' }]
}

async function open(openOptions: MedicationCatalogDialogOpenOptions = {}) {
  Object.assign(form, createEmptyMedicationCatalogForm())
  activeTab.value = 'basic'
  dialogRef.value?.openDialog()
  await nextTick()
  formRef.value?.clearValidate()

  loading.value = true
  try {
    const [formOptions, detail] = await Promise.all([
      loadMedicationCatalogFormOptions(),
      openOptions.id ? getMedicationCatalogDetail(openOptions.id) : Promise.resolve(null)
    ])
    Object.assign(options, formOptions)
    if (detail) Object.assign(form, toMedicationCatalogForm(detail))
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '药品表单加载失败')
    close()
  } finally {
    loading.value = false
  }
}

function close() {
  if (!saving.value) dialogRef.value?.closeDialog()
}

async function submit() {
  if (saving.value || loading.value) return
  const valid = await formRef.value?.validate().catch(() => false)
  if (!valid) return
  saving.value = true
  try {
    const payload = { ...form }
    if (form.id) await updateMedicationCatalog(form.id, payload)
    else await createMedicationCatalog(payload)
    ElMessage.success(form.id ? '药品更新成功' : '药品创建成功')
    dialogRef.value?.closeDialog()
    emit('saved')
  } catch (error) {
    ElMessage.error(error instanceof Error ? error.message : '药品保存失败')
  } finally {
    saving.value = false
  }
}

defineExpose({ open })
</script>
