package com.example.myjniapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        Button defaultBtn = findViewById(R.id.default_button);
        final TextView defaultLabel = findViewById(R.id.default_label);
        defaultBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                defaultLabel.setText(stringFromJNI());
            }
        });

        Button defaultBtn2 = findViewById(R.id.default_button2);
        final EditText defaultText2 = findViewById(R.id.default_text2);
        final TextView defaultLabel2 = findViewById(R.id.default_label2);
        defaultBtn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String expression = defaultText2.getText().toString();
                String value = expression + "=" + calculation(expression);
                defaultLabel2.setText(value);
            }
        });

        Button defaultBtn3 = findViewById(R.id.default_button3);
        defaultBtn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                callObject();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 实现四则运算
     * @param jsBuffer js
     * @return 所计算出来的值
     */
    public native double calculation(String jsBuffer);

    public native void callObject();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
