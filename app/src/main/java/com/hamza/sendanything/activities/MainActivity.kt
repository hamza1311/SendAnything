package com.hamza.sendanything.activities

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.hamza.sendanything.R
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mainAct_send_button.setOnClickListener {
            startActivity(Intent(this, SendFileActivity::class.java))
        }

        mainAct_receive_button.setOnClickListener {
            startActivity(Intent(this, ReceiveFileActivity::class.java))
        }
    }
}
