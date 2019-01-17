<template>
  <div id="md-details">
    <el-row class="details-controller" type="flex" align="bottom" style="flex-wrap: wrap; width: 100%;">
      <el-col style="width: unset; margin: 5px 5px 0;">
        <span class="details-title">日期范围:</span>
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
        <el-button size="mini" type="primary" style="padding: 6px 10px;">
          查询
        </el-button>
      </el-col>
    </el-row>
    <div class="divider"></div>
    <div class="details-content">
      <el-row type="flex" >
        <el-col class="video-card">
          <div @click="openVideo('video/video1.mp4')">

            <img src="@/assets/poster1.png">
            <span class="details-title">2019-1-6 10:11:48</span>
          </div>
          <!--<video src="video/video1.mp4" width="211" height="158" controls></video>-->
        </el-col>
        <el-col class="video-card">
          <div @click="openVideo('video/video2.mp4')">

            <img src="@/assets/poster2.png">
            <span class="details-title">2019-1-6 10:11:48</span>
          </div>
          <!--<video src="video/video1.mp4" width="211" height="158" controls></video>-->
        </el-col>
        <el-col class="video-card">
          <div @click="openVideo('video/video3.mp4')">

            <img src="@/assets/poster3.png">
            <span class="details-title">2019-1-6 10:11:48</span>
          </div>
          <!--<video src="video/video1.mp4" width="211" height="158" controls></video>-->
        </el-col>
        <el-col class="video-card">
          <div @click="openVideo('video/video4.mp4')">

            <img src="@/assets/poster4.png">
            <span class="details-title">2019-1-6 10:11:48</span>
          </div>
          <!--<video src="video/video1.mp4" width="211" height="158" controls></video>-->
        </el-col>
      </el-row>
    </div>
    <el-dialog title="监控视频" :visible.sync="videoShowing" width="680px" @close="videoPath = ''">
      <video id="monitor-video" :src="videoPath" width="640" height="480" controls></video>
    </el-dialog>
  </div>
</template>

<script>
  export default {
    name: "MonitorDetails",
    data () {
      return {
        videoPath: '',
        videoShowing: false,
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
      }
    },
    methods: {
      openVideo: function (val) {
        this.videoShowing = true;
        this.videoPath = "";
        this.videoPath = val
      }
    }
  }
</script>

<style scoped>
  #md-details {
    position: relative;
    background: rgba(255,255,255,0.9);
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }
  .details-controller {
    position: relative;
    padding: 20px 30px;
  }
  .details-title {
    display: block;
    line-height: 10px;
    font-size: 10px;
    color: #888;
    margin-bottom: 12px;
  }
  .divider {
    width: calc(100% - 40px);
    height: 2px;
    border-top: 1px solid #dcdfe6;
    margin: 0 auto;
  }
  .video-card {
    width: 213px;
    height: 160px;
    border-radius: 5px;
    border: 1px solid #dcdfe6;
    margin: 0 30px 20px;
    cursor: pointer;
  }
  .details-content {

    position: relative;
    padding: 20px 0;
  }
  .details-content .el-row {
    flex-wrap: wrap;
  }
</style>