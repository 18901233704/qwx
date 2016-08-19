package cn.leetcode.hellowechat;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import cn.leetcode.hellowechat.WechatWrapper;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView objHelloTextView = (TextView) findViewById(R.id.helloTextView);
        WechatWrapper objWechatWrapper = new WechatWrapper();
        String networkServerIp = objWechatWrapper.getNetworkServerIp();
        objHelloTextView.setText("DEBUG: networkServerIp " + networkServerIp);
    }
}
