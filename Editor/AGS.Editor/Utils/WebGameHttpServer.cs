using System;
using System.Collections.Generic;
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
        private const string _servingURL = "http://localhost:8000";

        public event Action<string> ServerStarted;
        public event Action<string> ServerStopped;

        public bool IsRunning
        {
            get
            {
                return _listener != null && _listener.IsListening;
            }
        }

        protected virtual void OnServerStarted()
        {
            ServerStarted?.Invoke("Server started on " + _servingURL);
        }

        protected virtual void OnServerStopped()
        {
            ServerStopped?.Invoke("Server stopped.");
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
            var request = context.Request;
            var response = context.Response;

            response.Headers["Cache-Control"] = "no-cache, no-store, must-revalidate";
            response.Headers["Pragma"] = "no-cache";
            response.Headers["Expires"] = "0";

            string requestedFile = Path.Combine(_directoryPath, request.Url.LocalPath.TrimStart('/'));

            if (Directory.Exists(requestedFile))
            {
                requestedFile = Path.Combine(_directoryPath, "index.html");
            }

            if (File.Exists(requestedFile))
            {
                byte[] fileBytes = File.ReadAllBytes(requestedFile);
                response.ContentType = GetMimeType(requestedFile);
                response.ContentLength64 = fileBytes.Length;

                using (Stream output = response.OutputStream)
                {
                    output.Write(fileBytes, 0, fileBytes.Length);
                }
            }
            else
            {
                response.StatusCode = (int)HttpStatusCode.NotFound;
                const string responseString = "<html><head><title>404 - File Not Found</title></head><body><h1>404 - File Not Found</h1></body></html>";
                byte[] buffer = Encoding.UTF8.GetBytes(responseString);
                response.ContentLength64 = buffer.Length;

                using (Stream output = response.OutputStream)
                {
                    output.Write(buffer, 0, buffer.Length);
                }
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
