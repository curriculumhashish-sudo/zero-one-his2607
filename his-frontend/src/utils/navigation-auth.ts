export interface NavigationAuthQuery {
  token?: string
  tokenHead?: string
}

function defaultSearch() {
  return typeof window === 'undefined' ? '' : window.location.search
}

function defaultStorage() {
  return typeof localStorage === 'undefined' ? undefined : localStorage
}

export function readNavigationAuth(
  search = defaultSearch(),
  storage = defaultStorage()
): NavigationAuthQuery {
  const params = new URLSearchParams(search)
  const token = params.get('token')?.trim() || storage?.getItem('token')?.trim() || ''
  const tokenHead = params.get('tokenHead')?.trim() || storage?.getItem('tokenHead')?.trim() || ''

  return {
    ...(token ? { token } : {}),
    ...(tokenHead ? { tokenHead } : {})
  }
}

export function persistNavigationAuth(search = defaultSearch(), storage = defaultStorage()) {
  const auth = readNavigationAuth(search, storage)
  if (auth.token) storage?.setItem('token', auth.token)
  if (auth.tokenHead) storage?.setItem('tokenHead', auth.tokenHead)
  return auth
}

export function resolveExternalNavigationUrl(
  path: string,
  configuredBase: string | undefined,
  currentOrigin = typeof window === 'undefined' ? '' : window.location.origin
) {
  const base = configuredBase?.trim()
  if (!base || !currentOrigin) return ''
  try {
    const target = new URL(path, base)
    return target.origin === new URL(currentOrigin).origin ? '' : target.toString()
  } catch {
    return ''
  }
}
