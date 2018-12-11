package com.jimi.bm.server.config;

import java.io.File;

import com.jfinal.config.Constants;
import com.jfinal.config.Handlers;
import com.jfinal.config.Interceptors;
import com.jfinal.config.JFinalConfig;
import com.jfinal.config.Plugins;
import com.jfinal.config.Routes;
import com.jfinal.core.JFinal;
import com.jfinal.kit.PropKit;
import com.jfinal.template.Engine;
import com.jimi.bm.server.controller.DeviceController;
import com.jimi.bm.server.controller.StationController;
import com.jimi.bm.server.interceptor.CORSInterceptor;
import com.jimi.bm.server.interceptor.ErrorLogInterceptor;
import com.jimi.bm.server.model._MappingKit;
import com.jfinal.ext.interceptor.SessionInViewInterceptor;
import com.jfinal.plugin.druid.DruidPlugin;
import com.jfinal.json.MixedJsonFactory;
import com.jfinal.plugin.activerecord.ActiveRecordPlugin;
import com.jfinal.plugin.activerecord.dialect.MysqlDialect;


public class MainConfig extends JFinalConfig {

	/**
	 * 配置JFinal常量
	 */
	@Override
	public void configConstant(Constants me) {
		// 读取数据库配置文件
		PropKit.use("config.properties");
		// 设置当前是否为开发模式
		me.setDevMode(PropKit.getBoolean("devMode"));
		// 设置默认上传文件保存路径 getFile等使用
		me.setBaseUploadPath("upload/temp/");
		// 设置上传最大限制尺寸
		// me.setMaxPostSize(1024*1024*10);
		// 设置默认下载文件路径 renderFile使用
		me.setBaseDownloadPath("download");
		// 设置404渲染视图
		// me.setError404View("404.html");
		// 设置json工厂
		me.setJsonFactory(MixedJsonFactory.me());
		me.setInjectDependency(true);

	}


	/**
	 * 配置JFinal路由映射
	 */
	@Override
	public void configRoute(Routes me) {

		me.add("/device", DeviceController.class);
		me.add("/station", StationController.class);
	}


	/**
	 * 配置JFinal插件
	 * 数据库连接池
	 * ORM
	 * 缓存等插件
	 * 自定义插件
	 */
	@Override
	public void configPlugin(Plugins me) {
		// 配置数据库连接池插件
		DruidPlugin dbPlugin = null;
		ActiveRecordPlugin arp = null;
		if (isProductionEnvironment()) {
			dbPlugin = new DruidPlugin(PropKit.get("p_jdbcUrl"), PropKit.get("p_user"), PropKit.get("p_password"));
			arp = new ActiveRecordPlugin(dbPlugin);
			arp.setShowSql(PropKit.getBoolean("devMode"));
			arp.setDialect(new MysqlDialect());
			dbPlugin.setDriverClass(PropKit.get("p_driver"));
		} else {
			dbPlugin = new DruidPlugin(PropKit.get("d_jdbcUrl"), PropKit.get("d_user"), PropKit.get("d_password"));
			arp = new ActiveRecordPlugin(dbPlugin);
			arp.setShowSql(PropKit.getBoolean("devMode"));
			arp.setDialect(new MysqlDialect());
			dbPlugin.setDriverClass(PropKit.get("d_driver"));
		}

		/********在此添加数据库 表-Model 映射*********/
		// 如果使用了JFinal Model 生成器 生成了BaseModel 把下面注释解开即可
		_MappingKit.mapping(arp);

		// 添加到插件列表中
		me.add(dbPlugin);
		me.add(arp);
	}


	/**
	 * 配置全局拦截器
	 */
	@Override
	public void configInterceptor(Interceptors me) {
		me.addGlobalActionInterceptor(new CORSInterceptor());
		me.addGlobalActionInterceptor(new ErrorLogInterceptor());
		me.addGlobalActionInterceptor(new SessionInViewInterceptor());
	}


	/**
	 * 配置全局处理器
	 */
	@Override
	public void configHandler(Handlers me) {

	}


	/**
	 * JFinal启动后调用
	 */
	@Override
	public void afterJFinalStart() {

	}


	/**
	 * JFinal Stop之前调用 
	 */
	@Override
	public void beforeJFinalStop() {

	}


	/**
	 * 配置模板引擎 
	 */
	@Override
	public void configEngine(Engine me) {

	}


	public static void main(String[] args) {
		JFinal.start("src/main/webapp", 80, "/", 5);
	}


	public static boolean isProductionEnvironment() {
		File[] roots = File.listRoots();
		for (int i = 0; i < roots.length; i++) {
			if (new File(roots[i].toString() + "PRODUCTION_ENVIRONMENT_FLAG").exists()) {
				return true;
			}
		}
		return false;
	}
}