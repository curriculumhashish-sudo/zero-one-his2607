declare global {
  interface Window {
    // 天爱验证码全局方法声明
    initTAC: (path: string, config: any, style: any) => Promise<any>
  }
}

export {}
