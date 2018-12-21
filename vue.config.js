module.exports = {
  devServer: {
    port: 6101,
    proxy: 'http://10.10.11.110:10000/mock/5bf36bb60a62aa1c2dde4c8a'
  },

  baseUrl: '/bus_monitor/'
}