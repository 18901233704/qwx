package cn.leetcode.hellowechat;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

//import cn.leetcode.hellowechat.WechatWrapper;
import com.tencent.mm.network.Java2C;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView objHelloTextView = (TextView) findViewById(R.id.helloTextView);
        String networkServerIp = "127.0.0.1";
        //WechatWrapper objWechatWrapper = new WechatWrapper();
        //networkServerIp = objWechatWrapper.getNetworkServerIp();
        Java2C objJava2C = new Java2C();
        networkServerIp = objJava2C.getNetworkServerIp();
        objHelloTextView.setText("DEBUG: networkServerIp " + networkServerIp);
    }
}
