package cn.leetcode.helloworld;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import com.tencent.mm.network.Java2C;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView objTextView = (TextView) findViewById(R.id.helloTextView);
        Java2C objJava2C = new Java2C();
        objTextView.setText(objJava2C.getNetworkServerIp());
    }
}
