package com.hamza.sendanything.activities

import android.annotation.SuppressLint
import android.app.Activity
import android.content.Intent
import android.os.AsyncTask
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.hamza.sendanything.R
import kotlinx.android.synthetic.main.activity_send_file.*
import kotlinx.android.synthetic.main.dialog_ongoing_operation.view.*
import org.apache.commons.net.ftp.FTP
import org.apache.commons.net.ftp.FTPClient
import java.io.BufferedInputStream
import java.io.File
import java.io.FileInputStream
import java.io.FileNotFoundException
import java.lang.ref.WeakReference

class SendFileActivity : AppCompatActivity() {
    @Suppress("PropertyName")
    val REQUEST_CODE = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_send_file)
        val intent = Intent()
            .apply {
                addCategory(Intent.CATEGORY_OPENABLE)
                action = Intent.ACTION_GET_CONTENT
                type = "*/*"
            }
        startActivityForResult(intent, REQUEST_CODE)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == REQUEST_CODE && resultCode == Activity.RESULT_OK) {

            sendFileAct_send_button.setOnClickListener {
                val server = sendFileAct_ip_editText.text.toString()
                val port = sendFileAct_port_editText.text.toString().toInt()
                val user = "user"
                val password = sendFileAct_password_editText.text.toString()

                val dataFromIntent = data!!.data

                val filePath = dataFromIntent?.path!!.split(":")[1]
                Log.d("ddd", "data: $dataFromIntent")
                Log.d("ddd", "filePath: $filePath")
                val path = "${Environment.getExternalStorageDirectory()}/$filePath"
                Log.d("ddd", "filePath: $path")
                val file = File(path)

                try {
//                  SendFileTask(WeakReference(this), "192.168.0.102", 1026, "user", "1234").execute(file)
                    SendFileTask(WeakReference(this), server, port, user, password).execute(file)
                } catch (e: FileNotFoundException) {
                    Toast.makeText(
                        this,
                        "Sorry, we weren't able to find the file. Error: $e",
                        Toast.LENGTH_LONG
                    ).show()
                }
            }
        }
    }

    class SendFileTask(
        private val activity: WeakReference<SendFileActivity>,
        private val server: String,
        private val port: Int,
        private val user: String,
        private val password: String
    ) : AsyncTask<File, Unit, Boolean>() {
        private val ftp: FTPClient = FTPClient()
        private lateinit var sendingView: WeakReference<View>

        @SuppressLint("InflateParams")
        override fun onPreExecute() {
            super.onPreExecute()
            val view = LayoutInflater.from(activity.get()).inflate(R.layout.dialog_ongoing_operation, null)
            val sendingDialog = AlertDialog.Builder(activity.get()!!).setView(view).create()
            sendingView = WeakReference(view)
            sendingDialog.show()
        }

        override fun doInBackground(vararg params: File?): Boolean {
            val success: Boolean
            ftp.apply {
                connect(server, port)
                login(user, password)
                Log.d("ddd", ftp.status)
                enterLocalPassiveMode()
                setFileType(FTP.BINARY_FILE_TYPE)
                val fileToSend = params[0]
                val inputStream = BufferedInputStream(FileInputStream(fileToSend))
                success = storeFile("""/${fileToSend!!.name}""", inputStream)
                inputStream.close()
            }
            return success
        }

        override fun onPostExecute(result: Boolean?) {
            super.onPostExecute(result)

            val view = sendingView.get()!!
            view.sending_constraintLayout.visibility = View.GONE
            if (result!!) {
                view.sent_constraintLayout.visibility = View.VISIBLE
            } else {
                view.operationDialog_nameDone_textView.text = activity.get()!!.getText(R.string.task_failed)
                    .replace("%s".toRegex(), activity.get()!!.getString(R.string.sending))
                view.sent_constraintLayout.visibility = View.VISIBLE
            }
        }
    }
}
