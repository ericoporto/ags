package uk.co.adventuregamestudio.agsplayer

import android.Manifest
import android.app.Activity
import android.content.DialogInterface
import android.content.Intent
import android.content.SharedPreferences
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.DocumentsContract
import android.provider.Settings
import android.util.Log
import android.view.*
import android.widget.AdapterView
import android.widget.EditText
import android.widget.Toast
import androidx.annotation.Keep
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.recyclerview.widget.DefaultItemAnimator
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import uk.co.adventuregamestudio.runtime.PreferencesActivity
import uk.co.adventuregamestudio.runtime.CreditsActivity
import uk.co.adventuregamestudio.runtime.NativeHelper
import java.io.File
import java.io.FilenameFilter
import java.util.*
import kotlin.collections.ArrayList
import uk.co.adventuregamestudio.agsplayer.FileUtil
import uk.co.adventuregamestudio.agsplayer.AGSPlayerRuntimeActivity


class MainActivity : AppCompatActivity(), GameListRecyclerViewAdapter.ItemClickListener, GameListRecyclerViewAdapter.ItemCreateContextMenuListener {
    private val gameList = ArrayList<GameModel>()
    private var nativeHelper: NativeHelper = NativeHelper()
    protected var externalStorageRequestDummy = IntArray(1)
    val EXTERNAL_STORAGE_REQUEST_CODE = 2

    var filename: String = ""

    private var resumedOnce: Boolean = false;
    private var lastSelectedPosition: Int = 0
    private val folderList = ArrayList<String>()
    private val filenameList = ArrayList<String>()

    private var baseDirectory: String? = null
    private val REQUEST_TREE = 0xCAFE

    private lateinit var gameListRecyclerViewAdapter: GameListRecyclerViewAdapter
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        // Get base directory from data storage
        val settings = getSharedPreferences("gameslist", 0)

        baseDirectory = settings.getString("baseDirectory", Environment.getExternalStorageDirectory().toString() + "/ags");

        if (baseDirectory!= null && !baseDirectory!!.startsWith("/"))
            baseDirectory = "/" + baseDirectory;

        // set up the RecyclerView
        val recyclerView: RecyclerView = findViewById(R.id.rvGameList)
        gameListRecyclerViewAdapter = GameListRecyclerViewAdapter(gameList)
        gameListRecyclerViewAdapter.setClickListener(this);
        gameListRecyclerViewAdapter.setCreateContextMenuListener(this);
        val layoutManager = LinearLayoutManager(applicationContext)
        recyclerView.layoutManager = layoutManager
        recyclerView.itemAnimator = DefaultItemAnimator()
        recyclerView.adapter = gameListRecyclerViewAdapter

        // Build game list
        Log.d("AGSPlayer.MainActivity", "onCreate()")
        buildGamesListIfPossible()
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        val inflater = menuInflater
        inflater.inflate(R.menu.main_context_menu, menu)
        return true
    }

    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    fun browseAgsDirectory() {
        startActivityForResult(Intent(Intent.ACTION_OPEN_DOCUMENT_TREE), REQUEST_TREE)
    }

    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    fun setAgsBaseDirFromUri(uri: Uri) {
        val path: String = FileUtil.getFullPathFromTreeUri(uri,this).toString()
        baseDirectory = path
        // The launcher will accept paths that don't start with a slash, but
        // the engine cannot find the game later.
        if (!baseDirectory!!.startsWith("/")) baseDirectory = "/$baseDirectory"

        Log.d("AGSPlayer.MainActivity", "setAgsBaseDirFromUri()")
        buildGamesListIfPossible()
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == REQUEST_TREE && resultCode == Activity.RESULT_OK) {
            data?.data?.let {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                    setAgsBaseDirFromUri(it)
                }
            }
        }
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        val id: Int = item.itemId
        if (id == R.id.credits) {
            showCredits()
            return true
        } else if (id == R.id.global_preferences) {
            showPreferences(-1)
            return true
        } else if (id ==  R.id.setfolder) {
            val alert: AlertDialog.Builder = AlertDialog.Builder(this)
            alert.setTitle("Game folder")
            val input = EditText(this)
            input.setText(baseDirectory)
            alert.setView(input)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                alert.setNeutralButton(
                    "Browse"
                ) { dialog, whichButton ->
                    browseAgsDirectory()
                  }
            }
            alert.setPositiveButton("Ok"
            ) { _ /*dialog*/, _ /*whichButton*/ ->
                val value = input.text
                baseDirectory = value.toString()
                // The launcher will accept paths that don't start with a slash, but
                // the engine cannot find the game later.
                if (!baseDirectory!!.startsWith("/")) baseDirectory = "/$baseDirectory"

                Log.d("AGSPlayer.MainActivity", "onOptionsItemSelected()")
                buildGamesListIfPossible()
            }
            alert.setNegativeButton("Cancel",
                DialogInterface.OnClickListener { dialog, whichButton -> })
            alert.show()
        }
        return super.onOptionsItemSelected(item)
    }

    override fun onStop() {
        super.onStop()
        val settings: SharedPreferences = getSharedPreferences("gameslist", 0)
        val editor: SharedPreferences.Editor = settings.edit()
        editor.putString("baseDirectory", baseDirectory)
        editor.apply()
    }

    override fun onItemCreateContextMenu(
            menu: ContextMenu?,
            view: View?,
            menuInfo: ContextMenu.ContextMenuInfo?,
            position: Int
    ) {
        // menuInfo is null for some reason, so we have a helper position value here
        super.onCreateContextMenu(menu, view, menuInfo)
        val inflater: MenuInflater = menuInflater
        inflater.inflate(R.menu.game_context_menu, menu)
        lastSelectedPosition = position
        menu?.setHeaderTitle(folderList[position])
    }

    override fun onContextItemSelected(item: MenuItem): Boolean {
        val id: Int = item.itemId
        // val info: AdapterView.AdapterContextMenuInfo = item.menuInfo as AdapterView.AdapterContextMenuInfo // this is null for some reason
        val itm_pos = lastSelectedPosition

        return if (id == R.id.game_preferences) {
            showPreferences(itm_pos)
            true
        } else if (id == R.id.game_start) {
            startGame(filenameList[itm_pos], false)
            true
        } else if (id == R.id.game_continue) {
            startGame(filenameList[itm_pos], true)
            true
        } else {
            super.onContextItemSelected(item)
        }
    }

    private  fun showCredits() {
        val intent =
            Intent(this, CreditsActivity::class.java)
        startActivity(intent)
    }

    private  fun showPreferences(position: Int) {
        val intent =
            Intent(this, PreferencesActivity::class.java)
        val b = Bundle()
        b.putString("name", if (position < 0) "" else folderList[position])
        b.putString("filename", if (position < 0) null else filenameList[position])
        b.putString("directory", baseDirectory)
        intent.putExtras(b)
        startActivity(intent)
    }

    override fun onItemClick(view: View?, position: Int) {
        startGame(filenameList[position], false);
    }

///// This method can be used to test the Recycler View in the emulator without data
//    private fun prepareGameData() {
//        var game = GameModel("Roger Quest","rogerquest.ags","/agsgames/rogerquest",false)
//        gameList.add(game)
//        game = GameModel("Future Flashback","futureflashback.ags","/agsgames/futureflashback",false)
//        gameList.add(game)
//        game = GameModel("Sierra Demo","sierrademo.ags","/agsgames/sierrademo",false)
//        gameList.add(game)
//
//        gameListRecyclerViewAdapter.notifyDataSetChanged()
//    }

    private fun startGame(filename: String, loadLastSave: Boolean) {
        val intent = Intent(this, AGSPlayerRuntimeActivity::class.java)
        val b = Bundle()
        b.putString("filename", filename)
        b.putString("directory", baseDirectory)
        b.putBoolean("loadLastSave", loadLastSave)
        intent.putExtras(b)
        startActivity(intent)
        finish()
    }

    private fun buildGamesListIfPossible() {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:R:hasAllFilesPermission()")
            if (hasAllFilesPermission()) {
                Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:buildGamesList()")
                buildGamesList();
            } else {
                Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:Ask ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION")
                val uri = Uri.parse("package:${BuildConfig.APPLICATION_ID}")

                // this request is asynchronous, we should really use registerForActivityResult to launch this
                // but this means we no longer have an action bar for the app since we inherit from CoponentActivity instead
                startActivity(
                    Intent(
                        Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,
                        uri
                    )
                )
            }
        } else if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:M:hasStoragePermission()")
            if (hasStoragePermission()) {
                Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:buildGamesList()")
                buildGamesList();
            } else {
                Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:Ask READ_EXTERNAL_STORAGE")

                ActivityCompat.requestPermissions(
                    this,
                    arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                    EXTERNAL_STORAGE_REQUEST_CODE
                )
            }
        } else {
            Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible::hasExternalStoragePermission()")
            if(hasExternalStoragePermission()) {
                Log.d("AGSPlayer.MainActivity", "buildGamesListIfPossible:buildGamesList()")
                buildGamesList();
            }
        }
    }

    private fun getDirectoryTree(baseDirectory: String, path: String?): LinkedList<String> {
        val result = LinkedList<String>()
        var searchDirectory = File(baseDirectory)
        if (path != null) {
            searchDirectory = File("$baseDirectory/$path")
        }
        if (searchDirectory.isDirectory) {
            val tempList =
                searchDirectory.list { dir, filename -> File("$dir/$filename").isDirectory }
            for (subpath in tempList!!) {
                var entry = subpath
                if (path != null) {
                    entry = "$path/$entry"
                }
                result.add(entry)
                result.addAll(getDirectoryTree(baseDirectory, entry)!!)
            }
        }
        return result
    }

    private fun buildGamesList() {
        gameList.clear()

        val tempFilename = searchForGames()
        if(tempFilename != null) {
            filename = tempFilename
                if (filename.isNotEmpty() && filename.length > 1) {
                    startGame(filename, false)
                    return
                }
        }
        if (folderList.isNotEmpty()) {
            folderList.forEachIndexed { index, folder ->
                val game = GameModel(folder,filenameList[index],folder,false)
                gameList.add(game)
            }
        } else {
            showMessage("No games found in \"$baseDirectory\"")
        }
        gameListRecyclerViewAdapter.notifyDataSetChanged()
    }

    private fun showMessage(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show()
    }


    private fun searchForGames(): String? {
        var dirList: LinkedList<String>? = null
        var dirArray: Array<String>? = null
        folderList.clear()
        filenameList.clear()

        // Check for the game files in the base directory
        var agsgame_path: String? = nativeHelper.findGameDataInDirectory(baseDirectory)
        if (agsgame_path != null && File(agsgame_path).isFile()) {
            return agsgame_path;
        }

        // Check for games in folders
        val agsDirectory = File(baseDirectory)
        if (agsDirectory.isDirectory() && agsDirectory.exists() && agsDirectory.canRead()) {
            dirList = getDirectoryTree(baseDirectory!!, null)
            if (dirList == null || dirList.isEmpty()) { return null }
            dirArray = Array(dirList.size) {
                dirList[it]
            }

            dirArray = dirArray.distinct().toTypedArray()
            val finalDirList = dirArray.sortedWith(compareBy(String.CASE_INSENSITIVE_ORDER, { it }))
            
            for (subpath in finalDirList!!) {
                agsgame_path = nativeHelper.findGameDataInDirectory("$baseDirectory/$subpath")
                if (agsgame_path != null && File(agsgame_path).isFile) {
                    folderList.add(subpath)
                    filenameList.add(agsgame_path)
                }
            }
        }

        return null
    }

    @RequiresApi(Build.VERSION_CODES.R)
    private fun hasAllFilesPermission() = Environment.isExternalStorageManager()

    @RequiresApi(Build.VERSION_CODES.M)
    private fun hasStoragePermission() =
        checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
                PackageManager.PERMISSION_GRANTED

    fun showExternalStoragePermissionMissingDialog() {
        val dialog =
            android.app.AlertDialog.Builder(this)
                .setTitle("Storage Permission Missing")
                .setMessage("AGS for Android will not be able to run non-packaged games without storage permission.")
                .setNeutralButton("Continue", null)
                .create()
        dialog.show()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        if (grantResults.isNotEmpty()) {
            Log.d("AGSPlayer.MainActivity", "Received a request permission result")
            when (requestCode) {
                this.EXTERNAL_STORAGE_REQUEST_CODE -> {
                    if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                        Log.d("AGSPlayer.MainActivity", "Permission granted")
                    } else {
                        Log.d("AGSPlayer.MainActivity", "Did not get permission.")
                        if (ActivityCompat.shouldShowRequestPermissionRationale(
                                this,
                                Manifest.permission.WRITE_EXTERNAL_STORAGE
                            )
                        ) {
                            showExternalStoragePermissionMissingDialog()
                        }
                    }
                    Log.d("AGSPlayer.MainActivity", "Unlocking AGS thread")
                    synchronized(externalStorageRequestDummy) {
                        externalStorageRequestDummy[0] = grantResults[0]
                    }
                }
                else -> super.onRequestPermissionsResult(requestCode, permissions, grantResults)
            }
        }
    }

    override fun onResume() {
        super.onResume()

        Log.d("AGSPlayer.MainActivity", "onResume()")
        if(resumedOnce) {
            buildGamesListIfPossible()
        }
        resumedOnce = true;
    }

    @Keep
    fun hasExternalStoragePermission(): Boolean {
        return hasPermissionSpecific(Manifest.permission.WRITE_EXTERNAL_STORAGE, this.EXTERNAL_STORAGE_REQUEST_CODE, "external storage")
    }

    @Keep
    fun hasPermissionSpecific(permission: String, request_code: Int, permission_description: String): Boolean {
        if (ActivityCompat.checkSelfPermission(
                this,
                permission
            )
            == PackageManager.PERMISSION_GRANTED
        ) {
            return true
        }
        Log.d(
            "AGSRuntimeActivity",
            "Requesting permission and locking AGS thread until we have an answer."
        )
        ActivityCompat.requestPermissions(
            this,
            arrayOf(permission),
            request_code
        )
        synchronized(externalStorageRequestDummy) {
            try {
                externalStorageRequestDummy.get(0)
            } catch (e: InterruptedException) {
                Log.d(
                    "AGSRuntimeActivity",
                    "requesting $permission_description permission",
                    e
                )
                return false
            }
        }
        return ActivityCompat.checkSelfPermission(
            this,
            permission
        ) == PackageManager.PERMISSION_GRANTED
    }

    // Prevent the activity from being destroyed on a configuration change
    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
    }

}

private fun AlertDialog.Builder.setNeutralButton(s: String, function: () -> Unit) {

}
