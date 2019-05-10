package com.hamza.sendanything.activities

import android.annotation.SuppressLint
import android.os.AsyncTask
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import com.hamza.sendanything.R
import kotlinx.android.synthetic.main.activity_recieve_file.*
import kotlinx.android.synthetic.main.dialog_ongoing_operation.view.*
import java.io.File
import java.lang.ref.WeakReference
import java.net.NetworkInterface
import java.net.ServerSocket
import java.net.SocketException
import java.util.*

class ReceiveFileActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_recieve_file)

        receiveFileAct_listeningAt_textView.text = getString(R.string.listening_at, getIPAddress())

        receiveFileAct_receive_button.setOnClickListener {
            val fileName = receiveFileAct_fileName_editText.text.toString()
            val filePath = "${Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)}/$fileName"
            Log.d("ddd", "path $filePath")
            ReceiveFileTask(filePath, WeakReference(this), 12345).execute()
        }
    }

    private fun getIPAddress(): String {
        try {
            Collections.list(NetworkInterface.getNetworkInterfaces()).forEach {
                Collections.list(it.inetAddresses).forEach { address ->
                    if (!address.isLoopbackAddress) {
                        val ip = address.hostAddress
                        if (ip.indexOf(':') < 0)
                            return ip
                    }
                }
            }
        } catch (e: Exception) {
            Toast.makeText(
                this,
                "Something went wrong in getting IP address of the device. Make sure your connected to a network (internet access not required) and try again Error: $e",
                Toast.LENGTH_LONG
            ).show()
        }
        return ""
    }

    private class ReceiveFileTask(
        private val fileName: String,
        private val activity: WeakReference<ReceiveFileActivity>,
        private val port: Int
    ) :
        AsyncTask<Unit, Unit, Boolean>() {
        private lateinit var sendingView: WeakReference<View>
        private lateinit var sendingDialog: AlertDialog

        override fun onPreExecute() {
            super.onPreExecute()
            @SuppressLint("InflateParams")
            val view = LayoutInflater.from(activity.get()).inflate(R.layout.dialog_ongoing_operation, null)
            view.operationDialog_name_textView.text = activity.get()!!.getText(R.string.receiving)
            sendingDialog = AlertDialog.Builder(activity.get()!!).setView(view).create()
            sendingView = WeakReference(view)
            sendingDialog.show()
        }

        override fun doInBackground(vararg params: Unit?): Boolean {
            val server = ServerSocket(port)
            var successStatus = false
            while (true) {
                try {
                    println("ddd Waiting for the client request")
                    val socket = server.accept()
                    File(fileName).writeBytes(socket.getInputStream().readBytes())
                    successStatus = true
                } catch (e: Exception) {
                    println("Shutting down Socket server!!")
                    server.close()
                    successStatus = false
                    Log.d("ddd", "error: $e")
                } finally {
                    server.close()
                    break
                }
            }
            return successStatus
        }

        override fun onPostExecute(result: Boolean?) {
            super.onPostExecute(result)

            val view = sendingView.get()!!
            view.sending_constraintLayout.visibility = View.GONE
            if (result!!) {
                view.operationDialog_nameDone_textView.text = activity.get()!!.getText(R.string.received)
                view.sent_constraintLayout.visibility = View.VISIBLE
            } else {
                view.operationDialog_nameDone_textView.text = activity.get()!!.getText(R.string.task_failed)
                    .replace("%s".toRegex(), activity.get()!!.getString(R.string.receiving))
                view.sent_constraintLayout.visibility = View.VISIBLE
            }
        }
    }
}
