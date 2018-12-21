<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>

export default {
  name: 'app',
  components: {
  },
  mounted() {
    // let marker, lineArr = [[116.478935,39.997761],[116.478939,39.997825],[116.478912,39.998549],[116.478912,39.998549],[116.478998,39.998555],[116.478998,39.998555],[116.479282,39.99856],[116.479658,39.998528],[116.480151,39.998453],[116.480784,39.998302],[116.480784,39.998302],[116.481149,39.998184],[116.481573,39.997997],[116.481863,39.997846],[116.482072,39.997718],[116.482362,39.997718],[116.483633,39.998935],[116.48367,39.998968],[116.484648,39.999861]];
    // let map = new AMap.Map("app", {
    //   resizeEnable: true,
    //   center: [116.397428, 39.90923],
    //   zoom: 17
    // });
    //
    // marker = new AMap.Marker({
    //   map: map,
    //   position: [116.478935,39.997761],
    //   icon: "https://webapi.amap.com/images/car.png",
    //   offset: new AMap.Pixel(-26, -13),
    //   autoRotation: true,
    //   angle:-90,
    // });
    //
    // // 绘制轨迹
    // let polyline = new AMap.Polyline({
    //   map: map,
    //   path: lineArr,
    //   showDir:true,
    //   strokeColor: "#28F",  //线颜色
    //   // strokeOpacity: 1,     //线透明度
    //   strokeWeight: 6,      //线宽
    //   // strokeStyle: "solid"  //线样式
    // });
    //
    // // let passedPolyline = new AMap.Polyline({
    // //   map: map,
    // //   // path: lineArr,
    // //   strokeColor: "#AF5",  //线颜色
    // //   // strokeOpacity: 1,     //线透明度
    // //   strokeWeight: 6,      //线宽
    // //   // strokeStyle: "solid"  //线样式
    // // });
    // //
    // // marker.on('moving', function (e) {
    // //   passedPolyline.setPath(e.passedPath);
    // // });
    // map.setFitView();
    //   marker.moveAlong(lineArr, 200, function (k) {
    //     return k;
    //   });
    //
    //   marker.on('moving', function (e) {
    //     console.log(e)
    //   })
  }
}
</script>

<style>
  html{
    height: 100%;
  }
  body {
    margin: 0;
    padding: 0;
    height: 100%;
    background: url("./assets/background.jpg");
  }

  #app {
    margin: 0;
    padding: 0;
    height: 100%;
  }

  .el-icon-loading{
    font-size: 33px;
  }
</style>
