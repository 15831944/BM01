<template>
    <el-container id="app-main">
      <el-header  height="64px">
        <page-header/>
      </el-header>
      <el-container>
        <el-aside width="130px">
          <side-bar/>
        </el-aside>
        <el-main>
          <router-view/>
        </el-main>
      </el-container>
    </el-container>

</template>

<script>
  import SideBar from '../components/SideBar'
  import PageHeader from "../components/PageHeader";

  export default {
    name: "Main",
    components: {
      PageHeader,
      SideBar
    }
  }
</script>

<style scoped>
  #app-main .el-header, .el-container, .el-main, .el-aside {
    padding: 0;
  }
  #app-main{
    height: 100%;
  }
  #app-main .el-main{
    padding: 0;
  }
  #app-main .el-aside {
    z-index: 100;
    border-right: 1px solid #ddd;
    box-shadow: 0 0 3px #ccc;
  }
  #app-main .el-header {
    z-index: 100;
  }
  .page-footer {
    margin-top: 20px;
  }


</style>