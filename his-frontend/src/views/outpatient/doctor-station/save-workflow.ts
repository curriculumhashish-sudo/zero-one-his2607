/**
 * 工作站中一个可以独立保存的诊疗模块。
 *
 * dirty 为 false 时不发请求，避免每次点击保存都无意义地递增三个版本号。
 */
export interface StationSaveTask {
  name: string
  dirty: boolean
  save: () => Promise<boolean>
}

export interface StationSaveResult {
  requested: string[]
  succeeded: string[]
  failed: string[]
}

/**
 * 并行保存所有已修改模块，并汇总每个模块的结果。
 *
 * 这个函数不依赖门诊组件或接口，因此测试时可以直接验证“一次点击会调用
 * 所有待保存模块”，页面只负责构造病历、诊断和医嘱三个任务。
 */
export async function saveDirtyStationSections(
  tasks: StationSaveTask[]
): Promise<StationSaveResult> {
  const pendingTasks = tasks.filter((task) => task.dirty)
  const results = await Promise.all(
    pendingTasks.map(async (task) => ({
      name: task.name,
      success: await task.save()
    }))
  )

  return {
    requested: pendingTasks.map((task) => task.name),
    succeeded: results.filter((item) => item.success).map((item) => item.name),
    failed: results.filter((item) => !item.success).map((item) => item.name)
  }
}
