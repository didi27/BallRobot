package ch.ydt.ballbotremote;

import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    private String standardIP = "192.168.1.1";
    private String standardSSID = "\"Dude\"";
    Button buttonConnStat;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        buttonConnStat = (Button) findViewById(R.id.buttonConnStat);
        buttonConnStat.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                if (!checkConnection()) {
                    startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));

                }
            }
        });
        checkConnection();
        final Button buttonForwards = (Button) findViewById(R.id.buttonForwards);
        buttonForwards.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                if (checkConnection()) {
                    new sendCommandToBot().execute("forwards");
                }
            }
        });
            final Button buttonBackwards = (Button) findViewById(R.id.buttonBackwards);
            buttonBackwards.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    // Perform action on click
                    if (checkConnection()) {
                        new sendCommandToBot().execute("backwards");
                    }

                }
        });
        final Button buttonTurnLeft = (Button) findViewById(R.id.buttonTurnLeft);
        buttonTurnLeft.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                if (checkConnection()) {
                    new sendCommandToBot().execute("turnleft");
                }

            }
        });
        final Button buttonTurnRight = (Button) findViewById(R.id.buttonTurnRight);
        buttonTurnRight.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                if (checkConnection()) {
                    new sendCommandToBot().execute("turnright");
                }

            }
        });
        final Button buttonFlip = (Button) findViewById(R.id.buttonFlip);
        buttonFlip.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                if (checkConnection()) {
                    new sendCommandToBot().execute("flipout");
                }

            }
        });
        }
    @Override
    protected void onResume(){
        super.onResume();
        checkConnection();
        Log.v("BallBotApp", "Application resumes - Connection checked");
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.settings, menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    public boolean checkConnection(){
        Log.v("BallBotApp", "checking connection");

        boolean connected = false;
        WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            String SSID = wifiInfo.getSSID().trim();
            Log.v("BallBotApp", SSID);

        Log.v("BallBotApp", standardSSID);
            if (SSID.equalsIgnoreCase(standardSSID)){
                new PingIP().execute(standardIP);
                Log.v("BallBotApp", "AP "+ standardSSID + " connected");
                        connected = true;
            }
            else {
                Toast toast = Toast.makeText(getApplicationContext(),"Please connect to " + standardSSID,Toast.LENGTH_LONG);
                toast.show();
                buttonConnStat.setBackgroundColor(0xFFFF0000);
                buttonConnStat.setText("Connection Error");
            }
        return connected;
    }
private class PingIP extends AsyncTask<String, Void, Boolean> {
    @Override
    protected Boolean doInBackground(String... params) {
        boolean connectionToIP = false;
     try {
         InetAddress in = InetAddress.getByName(params[0]);
                    if (in.isReachable(5000)) {
                        connectionToIP = true;
                    }
     }
     catch (IOException e) {
     Log.e("BallBotApp", "Network error");
         connectionToIP = false;
     }
        return connectionToIP;
    }
    @Override
    protected void onPostExecute(Boolean result) {
        super.onPostExecute(result);
        if (result) {
            buttonConnStat.setBackgroundColor(0xFF00CC00);
            buttonConnStat.setText("You are connected");
            Log.v("BallBotApp", standardSSID +" IP found");
        }
        else {
            buttonConnStat.setBackgroundColor(0xFFFF0000);
            buttonConnStat.setText("Connection Error");
            Log.v("BallBotApp", standardSSID + " IP not found");
        }
    }
}
    private class sendCommandToBot extends AsyncTask<String, Void, Boolean> {
        @Override
        protected Boolean doInBackground(String... params) {
            boolean driven = false;
            try {
                URL url;
                url = new URL("http://" + standardIP + "/" + params[0]);
                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                InputStream in = urlConnection.getInputStream();
                in.close();
                driven = true;
                Log.v("BallBotApp", standardSSID + " drives " + params[0] + " URL:" + url.toString());
            }catch (MalformedURLException e){
                Log.e("BallBotApp", "URL Exception");
            } catch (IOException e) {
                e.printStackTrace();
            }
            return driven;
        }
        @Override
        protected void onPostExecute(Boolean result) {
            super.onPostExecute(result);
        }
    }
}

