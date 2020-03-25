package com.example.sendanything

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import io.ktor.client.HttpClient
import io.ktor.client.engine.cio.CIO
import io.ktor.client.features.websocket.WebSockets
import io.ktor.client.features.websocket.ws
import io.ktor.http.HttpMethod
import io.ktor.http.cio.websocket.Frame
import io.ktor.http.cio.websocket.readBytes
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.io.BufferedInputStream
import java.io.ByteArrayInputStream
import java.io.FileOutputStream
import java.net.URLConnection

const val PORT = 9002

class MainActivity : AppCompatActivity() {
    private val client by lazy {
        @Suppress("EXPERIMENTAL_API_USAGE") HttpClient(CIO) { install(WebSockets) }
    }
    private var savedBuffer = byteArrayOf()
    private lateinit var host: String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        host = "10.0.2.2"

        receive_button.setOnClickListener {
            lifecycleScope.launch {
                client.ws(method = HttpMethod.Get, host = "10.0.2.2", port = PORT, path = "/") {
                    incoming.receive().readBytes().let {
                        savedBuffer = it
                        val inputStream = BufferedInputStream(ByteArrayInputStream(it))
                        val mimeType = withContext(Dispatchers.IO) {
                            URLConnection.guessContentTypeFromStream(inputStream)
                        }

                        createFile(mimeType)
                    }
                }
            }
        }

        send_button.setOnClickListener {
            openFile()
        }

    }

    override fun onActivityResult(
        requestCode: Int, resultCode: Int, resultData: Intent?
    ) {
        super.onActivityResult(requestCode, resultCode, resultData)
        if (resultCode == Activity.RESULT_OK) {
            println("yeet 1")
            resultData?.data?.also { uri ->
                when (requestCode) {
                    CREATE_FILE -> {
                        if (savedBuffer.isEmpty()) {
                            Log.e("File receiving failed??", "Buffer is empty")
                            return
                        }

                        this.contentResolver.openFileDescriptor(uri, "w")?.use {
                            FileOutputStream(it.fileDescriptor).use { fos ->
                                fos.write(savedBuffer)
                            }
                        }
                    }

                    PICK_FILE -> {
                        this.contentResolver.openInputStream(uri).use {
                            val bytes =
                                it?.readBytes() ?: error("Something went wrong in opening the file")

                            lifecycleScope.launch {
                                client.ws(
                                    method = HttpMethod.Get,
                                    host = host,
                                    port = PORT,
                                    path = "/"
                                ) {
                                    send(Frame.Binary(true, bytes))
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    private fun createFile(mimeType: String) {
        val intent = Intent(Intent.ACTION_CREATE_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = mimeType
            putExtra(Intent.EXTRA_TITLE, (System.currentTimeMillis() / 1000).toString())
        }
        startActivityForResult(intent, CREATE_FILE)
    }

    private fun openFile() {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT).apply {
            addCategory(Intent.CATEGORY_OPENABLE)
            type = "*/*"
        }
        startActivityForResult(intent, PICK_FILE)
    }


    companion object {
        const val CREATE_FILE = 1
        const val PICK_FILE = 2

    }
}
