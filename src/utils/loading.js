import Bus from './eventBus'

let  loading;

export const openFSLoading = () => {
  loading = Bus.$loading({
    lock: true,
    text: '加载中',
    spinner: 'el-icon-loading',
    background: 'rgba(0, 0, 0, 0.1)'
  })
};

export const closeFSLoading = () => {
  if (loading) {
    loading.close();
  }
};