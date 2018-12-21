<template>
  <div id="po-details">
    <po-controller></po-controller>
    <div id="po-map-container">
    </div>
    <div id="station-info" v-if="JSON.stringify(showingStation) !== '{}'">
      <div class="station-info-content">
        <div class="content-close">
          <div class="content-close-icon" @click="showingStation =  {}"><i class="el-icon-close"></i></div>
        </div>
        <span class="info-item-title">站台:</span><span>{{showingStation.stationName}}</span>
        <span class="info-item-title">路线:</span>
        <span
            v-for="(item, key) in showingStation.list"
            :style="item.groupId.toString() === $route.query.id ? 'font-weight:bold' : ''">{{item.groupName}}{{(key + 1)  === showingStation.list.length ? '' : ','}}</span>
        <div v-if="showingStation.ioStatus || showingStation.ioStatus.length > 0" v-for="ioItem in showingStation.ioStatus">
          <span class="info-item-title">上下车状况:</span>
          <span class="info-item-text">上车人数: {{ioItem.upCount}}人    </span>
          <span class="info-item-text">下车人数: {{ioItem.downCount}}人    </span>
          <span class="info-item-text">剩余总人数: {{ioItem.total}}人    </span>
          <span class="info-item-text">时间: {{ioItem.time}}</span>

        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import eventBus from '../../../utils/eventBus'
  import Controller from "./comp/Controller";
  import {axiosPost} from '../../../utils/fetchData'
  import {getRealtimeDataUrl, getStationsUrl} from "../../../config/globalUrl";
  import {throttle} from "../../../utils/utils";

  export default {
    name: "PositionDetails",
    components: {
      'po-controller': Controller
    },
    data() {
      return {
        mapConfig: {
          resizeEnable: false,
          level: 10,
          zoom: 13,
          center: [116.397428, 39.90923],
        },
        //返回数据总数组
        trackDataList: [],
        //返回数据的经纬度集
        trackData: [],
        pathCount: 0,
        stationList: [],
        showingStation: {},
        parkingList: []
      }
    },
    updated() {
    },
    computed: {
      busData: function () {
        return this.$store.state.busData
      }
    },
    watch: {
      $route: function (val) {
        if (this.amap) {
          // this.destroyMap();
          // this.initMap();
          eventBus.$emit('locateBus');
          this.clearLocateInterval();
        } else {
          //this.initMap();
          eventBus.$emit('locateBus');
          this.clearLocateInterval();
        }

        //this.initPosition();
        this.$closeFSLoading();
      }
    },
    mounted() {
      // this.$closeFSLoading();
      // setInterval(() => {
      //   this.amap = new AMap.Map('po-map-container')
      //   this.amap.on('complete', () => {
      //     setTimeout(()=> {
      //       this.amap.destroy();
      //     },3000)
      //   })
      // }, 5000);


      if (this.amap) {
        // this.destroyMap();
        // this.initMap();
        eventBus.$emit('locateBus', 1);
        this.clearLocateInterval();
        this.$closeFSLoading();

      } else {
        //this.initMap();
        eventBus.$emit('locateBus', 1);
        this.clearLocateInterval();
        this.$closeFSLoading();
      }
      //this.initPosition();
      /*event register*/
      eventBus.$on('locateBus', (time) => {
        if (this.amap) {
          this.destroyMap();
        }
        this.initMap();
        let lng = this.busData.lng;
        let lat = this.busData.lat;
        this.clearLocateInterval();

        //重设中心点
        this.amap.setCenter(new AMap.LngLat(lng, lat));


        //初始化或更新标识点
        if (!this.marker) {
          this.marker = new AMap.Marker({
            icon: "img/marker_icon.png",
            zIndex: 101,
            position: [lng, lat],
            offset: new AMap.Pixel(-14, -30)
          });
          this.marker.setMap(this.amap);
        } else {
          this.marker.setPosition([lng, lat])
        }

        //设置定时器监听数据变化以及刷新坐标
        if (this.$store.state.autoRefresh) {
          let interval = setInterval(() => {
            this.fetchData();
            let lng = this.busData.lng;
            let lat = this.busData.lat;
            this.marker.setPosition([lng, lat]);
            this.amap.setCenter(new AMap.LngLat(lng, lat));
          }, time * 1000);
          window.g.LOCATE_INTERVAL.push(interval);
        }


      });

      eventBus.$on('trackBus', (data) => {
        this.destroyMap();
        this.initMap();
        this.clearLocateInterval();
        this.trackDataList = data[0];
        this.trackData = data[1];
        this.parkingList = data[2];
        this.fetchStationData();
        let listLength = data[0].length;
        this.marker = new AMap.Marker({
          map: this.amap,
          icon: "img/marker_icon.png",
          offset: new AMap.Pixel(-14, -30),
          angle: 0,
          zIndex: 101,
          position: [this.trackDataList[listLength - 1].lng, this.trackDataList[listLength - 1].lat]
        });
        this.polyline = new AMap.Polyline({
          map: this.amap,
          path: this.trackData,
          showDir: true,
          strokeColor: "#28F",  //线颜色
          // strokeOpacity: 1,     //线透明度
          strokeWeight: 6,      //线宽
          // strokeStyle: "solid"  //线样式
        });
        this.passedPolyline = new AMap.Polyline({
          map: this.amap,
          // path: lineArr,
          strokeColor: "#AF5",  //线颜色
          // strokeOpacity: 1,     //线透明度
          strokeWeight: 6,      //线宽
          // strokeStyle: "solid"  //线样式
        });

        this.marker.on('moving', (e) => {
          if (e.passedPath.length > this.pathCount) {
            this.amap.setCenter(new AMap.LngLat(e.passedPath[e.passedPath.length - 1].lng, e.passedPath[e.passedPath.length - 1].lat));
            this.$store.commit('setBusData', this.trackDataList[e.passedPath.length - 1]);
            console.log(this.marker.getPosition())
            this.pathCount++
          }
          this.passedPolyline.setPath(e.passedPath);
        });
        this.amap.setFitView();
      });

      eventBus.$on('startTrack', () => {
        if (this.trackData.length > 0) {
          let listLength = this.trackDataList.length;
          this.pathCount = 0;
          this.marker.stopMove();
          this.amap.setCenter(new AMap.LngLat(this.trackDataList[listLength - 1].lng, this.trackDataList[listLength - 1].lat));
          this.marker.moveAlong(this.trackData, 300);
        } else {
          this.$alertInfo('没有数据可播放')
        }
      });

      eventBus.$on('clearLocateInterval', () => {
        this.clearLocateInterval()
      });

      eventBus.$on('destoryMap', () => {
        if (this.amap) {
          this.destroyMap()
        }
      });

    },
    methods: {
      initMap: function () {
        let amap = this.amap = new AMap.Map('po-map-container', this.mapConfig);
        amap.plugin(["AMap.ToolBar"], function () {
          amap.addControl(new AMap.ToolBar({
            direction: false,
            locate: false
          }));
        });
      },

      destroyMap: function () {
        this.amap.destroy();
        delete this.amap;
        delete this.marker;
      },
      fetchData: function () {
        if (!this.isPending) {
          this.isPending = true;
          axiosPost({
            url: getRealtimeDataUrl,
            data: {
              imei: this.busData.imei.toString()
            }
          }).then(res => {
            if (res.data.result === 200) {
              this.$store.commit('setBusData', res.data.data);
              this.isPending = false;
            } else {
              this.isPending = false;
            }
          }).catch(err => {
            this.isPending = false;
          })
        }

      },
      fetchStationData: function () {
        if (!this.isPending) {
          this.isPending = true;
          axiosPost({
            url: getStationsUrl
          }).then(res => {
            if (res.data.result === 200) {
              Object.assign(this.stationList, this.stationsFilter(res.data.data));
              this.initStationMarker(this.stationList);
              this.isPending = false;
            } else {
              this.$alertWarning("初始化站台信息失败");
              this.isPending = false;
            }
          }).catch(err => {
            this.$alertWarning("初始化站台信息失败");
            console.log(err)
            this.isPending = false;
          })
        }
      },
      stationsFilter: function (srcData) {
        let tempArray = [];
        for (let i = 0; i < srcData.length; i++) {
          for (let j = 0; j < srcData[i].list.length; j++) {
            if (srcData[i].list[j].groupId.toString() === this.$route.query.id) {
              tempArray.push(srcData[i]);
              j = srcData[i].length;
            }
          }
        }
        return tempArray;
      },
      //初始化列表中每个站点的标记
      initStationMarker: function (groupList) {
        for (let i = 0; i < groupList.length; i++) {
          this['stationMarker' + groupList[i].stationId] = new AMap.Marker({
            icon: "img/station_icon.png",
            position: [groupList[i].lng, groupList[i].lat],
            offset: new AMap.Pixel(-13, -30)
          });
          this['stationMarker' + groupList[i].stationId].setMap(this.amap)
          AMap.event.addListener(this['stationMarker' + groupList[i].stationId], 'click', () => {
            this.initStationCentent(groupList[i].stationId)
          });
        }
        this.initStationIO()

      },
      //生成指定站台信息
      initStationCentent: function (id) {
        for (let i = 0; i < this.stationList.length; i++) {
          if (this.stationList[i].stationId === id) {
            this.showingStation = this.stationList[i];
            return;
          }
        }
      },
      //初始化上下车人数
      initStationIO: function () {
        for (let i = 0; i < this.parkingList.length; i++) {
          let distanceList = [];
          for (let j = 0; j < this.stationList.length; j++) {
            let busPosition = this.parkingList[i];
            let stationPosition = this.stationList[j];
            distanceList.push(this.distanceCalc([busPosition.lng, busPosition.lat], [stationPosition.lng, stationPosition.lat]))
          }
          let index = distanceList.indexOf(Math.min.apply(Math, distanceList));
          this.stationList[index].ioStatus = [];
          this.stationList[index].ioStatus.push(JSON.parse(JSON.stringify(this.parkingList[i])));
          distanceList = [];
        }
      },
      //距离计算器(amap)
      distanceCalc: function (src, des) {
        let srcPos = new AMap.LngLat(src[0], src[1]);
        let desPos = new AMap.LngLat(des[0], des[1]);
        return srcPos.distance(desPos);
      },
      clearLocateInterval: function () {
        for (let i = 0; i < window.g.LOCATE_INTERVAL.length; i++) {
          clearInterval(window.g.LOCATE_INTERVAL[i])
        }
      }
    }
  }
</script>

<style scoped>
  #po-details {
    height: 100%;
  }

  #po-map-container {
    width: 100%;
    height: calc(100% - 160px);
    min-width: 600px;
    min-height: 400px;
    margin: 16px 0;
    border: 1px solid #ddd;
    border-radius: 0 0 10px 10px;
  }


</style>

<style>
  .amap-icon img,
  .amap-marker-content img {
    width: 28px;
    height: 34px;
  }

  #station-info {
    position: absolute;
    bottom: 40px;
    width: 280px;
    min-height: 120px;
    height: auto;
    z-index: 10000;
    margin-left: 10px;
  }

  .station-info-content {
    cursor: default;
    width: 100%;
    min-height: 140px;
    margin-top: 20px;
    padding: 10px 20px;
    background: rgba(255, 255, 255, 0.9);
    border-radius: 5px;
    color: #606266;
    white-space: pre-wrap;
  }

  .info-item-title {
    display: block;
    line-height: 10px;
    font-size: 10px;
    color: #888;
    margin: 6px 0;
  }

  .content-close {
    width: 100%;
    display: flex;
    justify-content: flex-end;
  }

  .content-close-icon {
    display: block;
    width: 18px;
    margin-bottom: -10px;
  }

  .content-close-icon:hover {
    color: #389aff;
    cursor: pointer;
  }

  .info-item-text {
    display: inline-block;

  }
</style>