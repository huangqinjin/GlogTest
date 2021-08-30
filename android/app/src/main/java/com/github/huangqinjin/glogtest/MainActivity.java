package com.github.huangqinjin.glogtest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.github.huangqinjin.glogtest.databinding.ActivityMainBinding;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private final String TAG = this.getClass().getSimpleName() + '@' + this.hashCode();
    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    @Override
    protected void onResume() {
        super.onResume();

        String app = getApplicationInfo().loadLabel(getPackageManager()).toString();
        String lib = getApplicationInfo().nativeLibraryDir; // android:extractNativeLibs
        File files = getExternalFilesDir(null);
        File cache = getExternalCacheDir(); // cache will be created on-demand automatically

        Log.i(TAG, "app: " + app);
        Log.i(TAG, "lib: " + lib);
        Log.i(TAG, "files: " + files);
        Log.i(TAG, "cache: " + cache);

        int ec = main(
            app,
            "--flagfile=" + new File(files, "flags.lst"),
            "--log_dir=" + cache
        );

        Log.i(TAG, "main exit with " + ec);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int main(String... args);
}