<template>
  <div id="schedule">
    <!--<h2 style="width: 100%; color: #656565; text-align: center; padding: 40px 0 20px;">排班表</h2>-->
   <!-- <el-table
        :data="tableData"
        style="width: auto; padding: 0 40px; border: none;">
      <el-table-column
          prop="a"
          width="auto">
      </el-table-column>
      <el-table-column>
        <el-table-column
            prop="a"
            width="auto">
        </el-table-column>
        <el-table-column>
          <el-table-column
              prop="a"
              width="auto">
          </el-table-column>
          <el-table-column
              prop="a"
              width="auto">
          </el-table-column>
          <el-table-column
              prop="a"
              width="auto">
          </el-table-column>
          <el-table-column
              prop="a"
              width="auto">
          </el-table-column>
        </el-table-column>
      </el-table-column>
    </el-table>-->
    <div class="schedule-content">

      <div class="block">
        <el-date-picker
            v-model="datetime"
            type="week"
            format="yyyy 第 WW 周"
            placeholder="选择周">
        </el-date-picker>
      </div>
      <div class="schedule-img">

        <img src="@/assets/schedule.png" draggable="false">
      </div>
    </div>
  </div>
</template>

<script>
  export default {
    name: "ScheduleMain",
    data() {
      return {
        datetime: '',
        tableData: [{
          a: ' '
        },{
          a: ' '
        },{
          a: ' '
        },{
          a: ' '
        },{
          a: ' '
        }, {
          a: ' '
        },]
      }
    }
  }
</script>

<style scoped>
  #schedule {
    position: relative;
    background: rgba(255, 255, 255, 0.9);
    width: calc(100% - 80px);
    height: calc(100% - 80px);
    margin: 20px;
    border-radius: 4px;
    display: flex;
    align-items: center;
    padding: 20px;
    flex-direction: column;
  }
  .schedule-content {
    width: 100%;
    max-width: 1210px;
    height: 100%;
  }
  .schedule-img{
    /*min-width: 600px;*/
    max-width: 1210px;
    height: calc(100% - 40px);
    overflow-y: auto;
    overflow-x: auto;
  }
  .schedule-img img {
    max-width: 1200px;
    width: auto;
  }
  .schedule-img::-webkit-scrollbar {
    width: 10px;
    height: 10px;
  }

  .schedule-img::-webkit-scrollbar-thumb {
    border-radius: 10px;
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    background: rgba(54, 54, 54, 0.14);
  }

  .schedule-img::-webkit-scrollbar-track {
    /*-webkit-box-shadow: inset 0 0 5px rgba(0, 0, 0, 0.2);*/
    border-radius: 10px;
    background: #EDEDED;
  }
</style>