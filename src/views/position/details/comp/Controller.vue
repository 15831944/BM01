<template>
  <el-row id="details-controller" :gutter="20">
    <el-col :lg="7" :md="24" style="height: 100%;">
      <el-row class="details-block">
        <el-col :span="12">
          <span class="details-title">信息:</span>
          <span class="details-text">●线路: {{busData.groupName}}</span>
          <span class="details-text">●车牌: {{busData.numPlate}}</span>
          <span class="details-text">●速度: {{busData.speed}}km/h</span>
          <span class="details-text">●状态: {{busData.statusString}}</span>
        </el-col>
        <el-col :span="12" style="padding-left: 0; height: 100%;">
          <el-row style="height: 100%;" type="flex" justify="space-around" align="bottom">
            <el-col>
              <el-button size="mini" @click="locateDevice">追踪</el-button>
            </el-col>
            <el-col>
              <el-button size="mini" @click="trackDevice">轨迹</el-button>
            </el-col>
          </el-row>
        </el-col>
      </el-row>
    </el-col>
    <el-col :lg="17" :md="24" style="height: 100%; ">
      <div class="details-block" style="box-sizing: border-box; min-height: 135px;">
        <div class="details-info">
          <span class="details-text">●定位方式: {{busData.locationTypeString}}</span>
          <span class="details-text">●最后定位时间: {{busData.locatedTime}}</span>
          <br/>
          <!--<span class="details-text">●最后通讯时间: {{busData.lastCommunicationTime}}</span>-->
          <span class="details-text">●最新位置: {{busData.location}}</span>
        </div>
        <el-row class="details-controller" type="flex" align="bottom" style="flex-wrap: wrap; width: 100%;"
                v-if="pageType === 'track'">
          <el-col style="width: unset; margin: 5px 5px 0;">
            <span class="details-title">轨迹控制栏:</span>
            <el-date-picker
                size="mini"
                v-model="timeRange"
                type="datetimerange"
                :picker-options="pickerOptions"
                range-separator="-"
                prefix-icon="el-icon-date"
                start-placeholder="开始日期"
                end-placeholder="结束日期"
                value-format="yyyy-MM-dd HH:mm:ss">
            </el-date-picker>
          </el-col>
          <el-col style="width: unset; margin: 0 5px;">
            <el-button size="mini" type="primary" style="padding: 6px 10px;" @click="setTrackOptions">
              查询
            </el-button>
          </el-col>
          <el-col style="width: unset; margin: 0 5px;">
            <el-button icon="el-icon-t-play" size="mini" type="primary" style="padding: 5px 10px;" @click="startTrack">
              播放
            </el-button>
          </el-col>

          <!--<el-col style="max-width: 360px; margin: 0 10px;" v-if="timeRange !== '' || this.timeRange !== null">-->
            <!--<span class="details-title" style="margin: 10px 0 0">{{currentTime}}</span>-->
            <!--<el-slider v-model="sliderValue" style="height: 34px;"></el-slider>-->
          <!--</el-col>-->
        </el-row>
        <el-row class="details-controller" type="flex" align="middle" style="height: 60px;"
                v-else-if="pageType === 'locate'">
          <el-col style="max-width: 100px;">
            <el-select
                v-model="refreshTime"
                placeholder="刷新间隔"
                @change="toggleAutoState"
                size="mini">
              <el-option
                  v-for="item in refreshTimeOptions"
                  :key="item.value"
                  :label="item.label"
                  :value="item.value">
              </el-option>
            </el-select>
          </el-col>
          <el-col style="width: unset; margin: 0 5px;">
            <el-button
                @click="refreshData"
                icon="el-icon-refresh"
                circle size="mini"></el-button>
          </el-col>
          <el-col style="width: unset; margin: 0 5px;">
            <span style="font-size: 10px; color: #636363;">自动刷新: </span>
            <el-switch
                @change="toggleAutoState"
                v-model="autoRefresh">
            </el-switch>
          </el-col>
        </el-row>

      </div>
    </el-col>
  </el-row>
</template>

<script>
  import eventBus from '../../../../utils/eventBus'
  import {getRealtimeDataUrl, getDeviceTrackUrl} from "../../../../config/globalUrl";
  import {axiosPost} from "../../../../utils/fetchData";

  export default {
    name: "Controller",
    data() {
      return {
        pageType: '',
        autoRefresh: true,
        refreshTime: '5',
        refreshTimeOptions: [
          {
            label: '5秒',
            value: '5'
          },
          {
            label: '10秒',
            value: '10'
          },
          {
            label: '20秒',
            value: '20'
          }
        ],
        timeRange: '',
        pickerOptions: {
          shortcuts: [{
            text: '最近一天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近三天',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 3);
              picker.$emit('pick', [start, end]);
            }
          }, {
            text: '最近一周',
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit('pick', [start, end]);
            }
          }]
        },
        sliderValue: 0,
        currentTime: '2018-12-6 19:41:11',
        isPending: false,
        isPlaying: false
      }
    },
    computed: {
      /*format*/
      /* busData: {
          "groupId": 1,
          "groupName": "7路",
          "numPlate": "京A 69999",
          "imei": 980182301249,
          "direction": "320",
          "locatedTime": "2017-10-26 11:15:12",
          "recvTime": "2017-10-26 11:15:12",
          "speed": 36,
          "lat": 22.763443321627392,
          "lng": 113.88657877780227,
          "location": "XX省XX市XX县XX乡",
          "locationType": 3,
          "locationTypeString": "WiFi定位",
          "reportMode": "",
          "reportModeString": "",
          "status": 2,
          "statusString": "静止",
      }
       },*/
      busData: function () {
        return this.$store.state.busData
      }
    },
    updated() {
      // let imeiA = this.busData.imei;
      // let imeiB = this.$route.query.imei;
      // if (!imeiA || imeiA.toString() !== imeiB) {
      //   this.$router.replace('/position')
      // }
    },
    mounted() {
      let imeiA = this.busData.imei;
      let imeiB = this.$route.query.imei;
      if (!imeiA || imeiA.toString() !== imeiB) {
        this.$router.replace('/position')
      }
    },
    methods: {
      locateDevice: function () {
        this.pageType = 'locate';
        this.getRealtimeData(this.busData.imei.toString())
      },
      trackDevice: function () {
        this.pageType = 'track';

      },
      setTrackOptions: function () {
        if (this.timeRange === "" || !this.timeRange) {
          this.$alertWarning("日期未选择");
          return;
        }
        this.getTrackData(this.busData.imei.toString())
      },
      getRealtimeData: function (imei) {
          if (!this.isPending) {
            this.isPending = true;
            axiosPost({
              url: getRealtimeDataUrl,
              data: {
                imei: imei
              }
            }).then(res => {
              if (res.data.result === 200) {
                this.$store.commit('setBusData', res.data.data);
                eventBus.$emit('locateBus', this.refreshTime)
              } else {
                this.$alertWarning(res.response.data);
              }
              this.isPending = false;
            }).catch(err => {
              this.isPending = false;
              this.$alertWarning(err.response.statusText);
            })
          }
      },
      getTrackData: function (imei) {
          if (!this.isPending) {
            this.isPending = true;
            axiosPost({
              url: getDeviceTrackUrl,
              data: {
                imei: imei,
                beginTime: this.timeRange[0],
                endTime: this.timeRange[1]
              }
            }).then(res => {
              if (res.data.result === 200) {
                let data = res.data.data;
                if (data.list.length === 0) {
                  this.$alertInfo('所选日期内无数据');
                  this.isPending = false;
                  return
                }
                this.$store.commit('setBusData', data.list[data.list.length - 1]);
                let lnglat = [];
                let parkinglist = [];
                for (let i = 0; i < data.list.length; i++) {
                  lnglat.unshift([data.list[i].lng, data.list[i].lat]);
                  if (data.list[i].type === 'busStop') {
                    parkinglist.unshift({
                      time: data.list[i].communicatedTime,
                      downCount: data.list[i].downCount,
                      upCount: data.list[i].upCount,
                      total: data.list[i].total,
                      lng: data.list[i].lng,
                      lat: data.list[i].lat
                    })
                  }
                }
                eventBus.$emit('trackBus', [data.list, lnglat, parkinglist])
              } else {
                this.$alertWarning(err.response.data);
              }
              this.isPending = false;
            }).catch(err => {
              this.isPending = false;
              this.$alertWarning(err);
            })
          }

      },
      startTrack: function() {
        eventBus.$emit('startTrack')
      },
      toggleAutoState: function (val) {
        if (!val) {
          eventBus.$emit('clearLocateInterval');
          this.$store.commit('setAutoRefreshState', false);
        } else {
          eventBus.$emit('locateBus', this.refreshTime);
          this.$store.commit('setAutoRefreshState', true);
        }
      },
      refreshData: function () {
        eventBus.$emit('locateBus', this.refreshTime);
      },
      dM: function () {
        eventBus.$emit('destoryMap');
      }
    }
  }
</script>

<style scoped>
  #details-controller {
    min-width: 600px;
    width: 100%;
    min-height: 140px;
  }

  .details-block {
    background-color: rgba(255, 255, 255, 0.8);
    border: 1px solid #ddd;
    border-radius: 20px 0 20px 0;
    height: 100%;
    box-shadow: 1px 1px 10px #ddd;
    padding: 16px 20px;
  }

  /*.details-block .el-col {*/
  /*padding: 16px 20px*/
  /*}*/

  .details-title {
    display: block;
    line-height: 10px;
    font-size: 10px;
    color: #888;
    margin-bottom: 12px;
  }

  .details-text {
    display: block;
    line-height: 14px;
    font-size: 14px;
    color: #636363;
    margin-bottom: 6px;
  }

  .details-info .details-text {
    display: inline-block;
    margin-right: 16px;
  }
</style>