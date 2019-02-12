package com.jimi.bm.parser.util;

import java.io.IOException;
import java.util.Map;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.FormBody.Builder;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;


public class HttpHelper {

	private final static OkHttpClient client = new OkHttpClient();


	/**@author HCJ
	 * http请求，参数存在时要求MAP，键为参数名，值为参数值
	 * @method requestHttp
	 * @param action
	 * @param args
	 * @return
	 * @throws IOException
	 * @return String
	 * @date 2018年9月25日 下午5:50:01
	 */
	public static String postRequestHttp(String url, String action, Map<String, String> args) throws IOException {
		String result;
		if (args != null) {
			Builder requestBuilder = new FormBody.Builder();
			for (Map.Entry<String, String> entry : args.entrySet()) {
				requestBuilder.add(entry.getKey(), entry.getValue());
			}
			RequestBody requestBody = requestBuilder.build();
			Request request = new Request.Builder().url(url + action).post(requestBody).build();
			Call call = client.newCall(request);
			result = call.execute().body().string();
		} else {
			Request request = new Request.Builder().url(url + action).build();
			Call call = client.newCall(request);
			result = call.execute().body().string();
		}
		return result;
	}


	/**
	 * http的get请求，同步
	 * @param url 链接
	 * @return
	 * @throws IOException
	 */
	public static String getRequestHttp(String url) throws IOException {
		Request request = new Request.Builder().url(url).build();
		Response response = client.newCall(request).execute();
		if (response.isSuccessful()) {
			return response.body().string();
		} else {
			throw new IOException("Unexpected code " + response);
		}
	}


	/**@author HCJ
	 * http请求，参数存在时要求MAP，键为参数名，值为参数值
	 * @method requestHttp
	 * @param action
	 * @param args
	 * @param callback
	 * @throws IOException
	 * @return void
	 * @date 2018年9月25日 下午5:50:31
	 */
	public static void postRequestHttp(String url, String action, Map<String, String> args, Callback callback) throws IOException {
		Builder requestBuilder = new FormBody.Builder();
		if (args != null) {
			for (Map.Entry<String, String> entry : args.entrySet()) {
				requestBuilder.add(entry.getKey(), entry.getValue());
			}
		}
		RequestBody requestBody = requestBuilder.build();
		Request request = new Request.Builder().url(url + action).post(requestBody).build();
		Call call = client.newCall(request);
		call.enqueue(callback);
	}

}
