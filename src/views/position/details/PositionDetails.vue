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
        <div v-if="showingStation.ioStatus || showingStation.ioStatus.length > 0">
          <span class="info-item-title">上下车状况:</span>
          <div v-for="ioItem in showingStation.ioStatus" style="margin-bottom: 3px">
            <span class="info-item-text">上车人数: {{ioItem.upCount}}人    </span>
            <span class="info-item-text">下车人数: {{ioItem.downCount}}人    </span>
            <span class="info-item-text">剩余总人数: {{ioItem.total}}人    </span>
            <span class="info-item-text">时间: {{ioItem.time}}</span>
          </div>

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
        //返回的最新的点
        firstPoint: {},
        //返回数据的经纬度集
        trackData: [],
        pathCount: 0,
        stationList: [],
        showingStation: {},
        parkingList: [],
        trackList: [],
        locMarkerGroup: {}
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
        Object.assign(this.$data, this.$options.data());
        eventBus.$emit('locateBus');
        this.$closeFSLoading();
      }
    },
    mounted() {
      if (this.amap) {
        this.destroyMap();
      }
      this.initMap();
      this.amap.on('complete', () => {
        let lng = this.$store.state.busData.lng;
        let lat = this.$store.state.busData.lat;
        this.clearLocateInterval();

        //重设中心点
        this.amap.setCenter(new AMap.LngLat(lng, lat));

        let transferOption =  {
          city: '0752'
        };
        let transfer = new AMap.Transfer(transferOption);

        transfer.search(new AMap.LngLat(114.309677, 23.176152), new AMap.LngLat(114.40556, 23.113625), (status, result) => {
          if (status === 'complete') {
            if (result.plans && result.plans.length) {
              this.drawRoute(result.plans[0], '#5dff8b')
            }
          }
        });

        transfer.search(new AMap.LngLat(114.40556, 23.113625), new AMap.LngLat(114.309677, 23.176152), (status, result) => {
          if (status === 'complete') {
            if (result.plans && result.plans.length) {
              this.drawRoute(result.plans[0], '#389aff')
            }
          }
        });

        //初始化或更新标识点
        if (!this.marker) {
          this.marker = new AMap.Marker({
            icon: "img/marker_icon.png",
            zIndex: 101,
            position: [lng, lat],
            offset: new AMap.Pixel(-11, -29)
          });
          this.marker.setMap(this.amap);
        } else {
          this.marker.setPosition([lng, lat])
        }
        this.clearLocateInterval();
        this.$closeFSLoading();
      });


      /*event register*/
      eventBus.$on('locateBus', (time) => {
        if (this.amap) {
          this.destroyMap();
        }
        this.initMap();
        this.amap.on('complete', () => {
          let lng = this.busData.lng;
          let lat = this.busData.lat;
          this.clearLocateInterval();

          //重设中心点
          this.amap.setCenter(new AMap.LngLat(lng, lat));


          let transferOption =  {
            city: '0752'
          };
          let transfer = new AMap.Transfer(transferOption);

          transfer.search(new AMap.LngLat(114.309677, 23.176152), new AMap.LngLat(114.40556, 23.113625), (status, result) => {
            if (status === 'complete') {
              if (result.plans && result.plans.length) {
                this.drawRoute(result.plans[0], '#5dff8b')
              }
            }
          });

          transfer.search(new AMap.LngLat(114.40556, 23.113625), new AMap.LngLat(114.309677, 23.176152), (status, result) => {
            if (status === 'complete') {
              if (result.plans && result.plans.length) {
                this.drawRoute(result.plans[0], '#389aff')
              }
            }
          });





          //初始化或更新标识点
          if (!this.marker) {
            this.marker = new AMap.Marker({
              icon: "img/marker_icon.png",
              zIndex: 101,
              position: [lng, lat],
              offset: new AMap.Pixel(-11, -29)
            });
            this.marker.setMap(this.amap);
          } else {
            this.marker.setPosition([lng, lat])
          }
          //设置定时器监听数据变化以及刷新坐标
          if (this.$store.state.autoRefresh && time) {
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

      });

      eventBus.$on('trackBus', (data) => {
        this.pathCount = 0;
        if (this.marker.stopMove) {
          this.marker.stopMove();
        }

        this.destroyMap();
        this.initMap();
        this.amap.on('complete', () => {
          this.clearLocateInterval();
          this.firstPoint = Object.assign(data[0], this.firstPoint);
          this.trackData = data[1];
          this.parkingList = data[2];
          this.trackList = data[3];
          this.fetchStationData();



          this.marker = new AMap.Marker({
            map: this.amap,
            icon: "img/marker_icon.png",
            offset: new AMap.Pixel(-11, -29),
            angle: 0,
            zIndex: 101,
            position: [this.firstPoint.aLng, this.firstPoint.aLat]
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
              this.$store.commit('setBusData', this.firstPoint);
              this.pathCount++
            }
            this.passedPolyline.setPath(e.passedPath);
          });
          this.amap.setFitView();
        });

      });

      eventBus.$on('startTrack', () => {
        if (this.trackData.length > 0) {
          this.pathCount = 0;
          this.marker.stopMove();
          this.amap.setCenter(new AMap.LngLat(this.firstPoint.aLng, this.firstPoint.aLat));
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

      eventBus.$on('addDebugPoint', () => {
        this.locMarkerGroup = {};
        for (let i = 0; i < this.trackList.length; i++) {
          let randomKey = Math.round(Math.random() * 100000000);
          this.locMarkerGroup['marker' + randomKey] = new AMap.Marker({
            icon: "img/marker_icon.png",
            position: [this.trackData[i].lng, this.trackData[i].lat],
            offset: new AMap.Pixel(-11, -29)
          });
          this.locMarkerGroup['marker' + randomKey].setMap(this.amap);
          AMap.event.addListener(this.locMarkerGroup['marker' + randomKey], 'click', () => {
            let string = "";
            Object.keys(this.trackList[this.trackList.length - i]).forEach((index) => {
              string += index + ":" + this.trackList[this.trackList.length - i][index] + "\n"
            });
            alert(string)
          });
        }
      })
    },
    methods: {
      initMap: function () {
        let _this = this;
        this.amap = new AMap.Map('po-map-container', this.mapConfig);
        this.amap.plugin(["AMap.ToolBar"], function () {
          _this.amap.addControl(new AMap.ToolBar({
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
              AMap.convertFrom((res.data.data.lng + ',' + res.data.data.lat), "gps", (status, result) => {

                if (status === "complete") {
                  res.data.data.lng = result.locations[0].P;
                  res.data.data.lat = result.locations[0].O;
                }

                this.$store.commit('setBusData', res.data.data);
                this.isPending = false;
              });
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
            url: getStationsUrl,
            data: {
              groupId: this.$route.query.id
            }
          }).then(res => {
            if (res.data.result === 200) {
              Object.assign(this.stationList, res.data.data);
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

      //初始化列表中每个站点的标记
      initStationMarker: function (groupList) {
        for (let i = 0; i < groupList.length; i++) {
          let iconName;
          switch (groupList[i].type) {
            case 0:
              iconName = 'img/station_icon_c.png';
              break;
            case 1:
              iconName = 'img/station_icon_a.png';
              break;
            case 2:
              iconName = 'img/station_icon_b.png';
              break;
          }
          this['stationMarker' + groupList[i].stationId] = new AMap.Marker({
            icon: iconName,
            position: [groupList[i].lng, groupList[i].lat],
            offset: new AMap.Pixel(-11, -29)
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
          if (!this.stationList[index].ioStatus) {
            this.stationList[index].ioStatus = [];
          }
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
      },
      drawRoute: function (route, color) {
        // let startMarker = new AMap.Marker({
        //   position: route.segments[0].transit.origin,
        //   icon: 'https://webapi.amap.com/theme/v1.3/markers/n/start.png',
        //   map: this.amap
        // });
        //
        // let endMarker = new AMap.Marker({
        //   position: route.segments[route.segments.length - 1].transit.destination,
        //   icon: 'https://webapi.amap.com/theme/v1.3/markers/n/end.png',
        //   map: this.amap
        // });

        let routeLines = [];

        for (let i = 0, l = route.segments.length; i < l; i++) {
          let segment = route.segments[i];
          let line = null;

          // 绘制步行路线
          if (segment.transit_mode === 'WALK') {
            line = new AMap.Polyline({
              path: segment.transit.path,
              isOutline: true,
              outlineColor: '#ffeeee',
              borderWeight: 2,
              strokeWeight: 5,
              strokeColor: 'grey',
              lineJoin: 'round',
              strokeStyle: 'dashed'
            });


            line.setMap(this.amap);
            routeLines.push(line)
          } else if (segment.transit_mode === 'SUBWAY' || segment.transit_mode === 'BUS') {
            line = new AMap.Polyline({
              path: segment.transit.path,
              isOutline: true,
              outlineColor: '#ffeeee',
              borderWeight: 2,
              strokeWeight: 5,
              strokeColor: color,//'#0091ff',
              lineJoin: 'round',
              strokeStyle: 'solid'
            });

            line.setMap(this.amap);
            routeLines.push(line)
          }
        }

        // 调整视野达到最佳显示区域
        // this.amap.setFitView([startMarker, endMarker].concat(routeLines))
        // this.amap.setFitView(routeLines)

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
    max-height: 500px;
    margin-top: 20px;
    padding: 10px 20px;
    background: rgba(255, 255, 255, 0.9);
    border-radius: 5px;
    color: #606266;
    white-space: pre-wrap;
    overflow-y: auto;
  }

  .station-info-content::-webkit-scrollbar {
    width: 10px;
    height: 1px;
  }

  .station-info-content::-webkit-scrollbar-thumb {
    border-radius: 10px;
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    background: rgba(54, 54, 54, 0.14);
  }

  .station-info-content::-webkit-scrollbar-track {
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    border-radius: 10px;
    background: #EDEDED;
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
    font-size: 14px;
  }
</style>