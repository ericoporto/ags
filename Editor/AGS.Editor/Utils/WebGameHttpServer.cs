using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AGS.Editor.Utils
{
    public class WebGameHttpServer
    {
        private HttpListener _listener;
        private Thread _serverThread;
        private string _directoryPath;
        private const string _servingURL = "http://localhost:8000/";
        private const string _exitFile = "force_exit.json";

        public event Action ServerStarted;
        public event Action ServerStopped;

        public bool IsRunning
        {
            get
            {
                return _listener != null && _listener.IsListening;
            }
        }

        protected virtual void OnServerStarted()
        {
            OpenWebBrowser();
            ServerStarted?.Invoke();
        }

        protected virtual void OnServerStopped()
        {
            ServerStopped?.Invoke();
        }

        public void OpenWebBrowser()
        {
            Uri uri;
            if (!Uri.TryCreate(_servingURL, UriKind.Absolute, out uri))
            {
                throw new Exception("An error occurred when creating url: " + _servingURL );
            }

            try
            {
                Process.Start(uri.ToString());
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException("Failed to open the browser. Ensure a default browser is set.", ex);
            }
        }


        public void Start(string directoryPath)
        {
            if (IsRunning)
                throw new InvalidOperationException("Server is already running.");

            if (string.IsNullOrWhiteSpace(directoryPath) || !Directory.Exists(directoryPath))
                throw new ArgumentException("Invalid directory path.", nameof(directoryPath));

            _directoryPath = directoryPath;

            _listener = new HttpListener();
            _listener.Prefixes.Add(_servingURL);
            _listener.Start();

            _serverThread = new Thread(RunServer) { IsBackground = true };
            _serverThread.Start();
            OnServerStarted();
        }

        public void Stop()
        {
            if (_listener != null)
            {
                if(_listener.IsListening)
                    _listener.Stop();
                _listener.Close();
                _listener = null;

            }

            if (_serverThread != null)
            {
                _serverThread.Join();
                _serverThread = null;
            }

            OnServerStopped();
        }

        private void RunServer()
        {
            try
            {
                while (_listener.IsListening)
                {
                    HttpListenerContext context = _listener.GetContext();
                    ProcessRequest(context);
                }
            }
            catch (HttpListenerException)
            {
                // Listener stopped
                Stop();
            }
        }

        private void ProcessRequest(HttpListenerContext context)
        {
            byte[] buffer;
            var request = context.Request;
            var response = context.Response;

            response.Headers["Cache-Control"] = "no-cache, no-store, must-revalidate";
            response.Headers["Pragma"] = "no-cache";
            response.Headers["Expires"] = "0";

            string requestedFile = Path.Combine(_directoryPath, request.Url.LocalPath.TrimStart('/'));
            string fileName = Path.GetFileName(requestedFile);

            if(fileName == _exitFile)
            {
                throw new HttpListenerException(0, "browser tab closed");
                return;
            }

            if (Directory.Exists(requestedFile) && File.Exists(Path.Combine(_directoryPath, "index.html")))
            {
                // requested a directory, so try index.html instead
                requestedFile = Path.Combine(_directoryPath, "index.html");
                string fileContent = File.ReadAllText(requestedFile);

                // FIX-ME: the pagehide event is not working, and unload is obsolete in current browsers.
                // we will inject a fake file to exit the server when browser tab is closed
                const string script = "<script>window.addEventListener('pagehide', function (e) {" +
                    "e.preventDefault();" +
                    "fetch('" + _servingURL + _exitFile + "', { method: 'GET' });}, false);" +
                    "</script>";
                int bodyIndex = fileContent.IndexOf("</body>", StringComparison.OrdinalIgnoreCase);
                if (bodyIndex != -1)
                {
                    fileContent = fileContent.Insert(bodyIndex, script);
                }
                else
                {
                    fileContent += script;
                }

                buffer = Encoding.UTF8.GetBytes(fileContent);
                response.ContentType = GetMimeType(requestedFile);
            }
            else if (File.Exists(requestedFile))
            {
                buffer = File.ReadAllBytes(requestedFile);
                response.ContentType = GetMimeType(requestedFile);
            }
            else
            {
                response.StatusCode = (int)HttpStatusCode.NotFound;
                const string responseString = "<html><head><title>404 - File Not Found</title></head><body><h1>404 - File Not Found</h1></body></html>";
                buffer = Encoding.UTF8.GetBytes(responseString);
            }

            response.ContentLength64 = buffer.Length;
            using (Stream output = response.OutputStream)
            {
                output.Write(buffer, 0, buffer.Length);
            }
        }

        private string GetMimeType(string fileName)
        {
            string extension = Path.GetExtension(fileName).ToLower();
            switch (extension)
            {
                case ".html":
                case ".htm":
                    return "text/html";
                case ".css":
                    return "text/css";
                case ".js":
                    return "application/javascript";
                case ".json":
                    return "application/json";
                case ".png":
                    return "image/png";
                case ".jpg":
                case ".jpeg":
                    return "image/jpeg";
                case ".gif":
                    return "image/gif";
                case ".txt":
                case ".ini":
                case ".cfg":
                    return "text/plain";
                case ".wasm":
                    return "application/wasm";
                default:
                    return "application/octet-stream";
            }
        }
    }
}
