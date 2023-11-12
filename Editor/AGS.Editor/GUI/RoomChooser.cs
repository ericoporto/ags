using AGS.Types;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AGS.Editor.GUI
{
    public partial class RoomChooser : Form
    {
        private Room _selectedRoom;

        /// <summary>
        /// Resize the image to the specified width and height.
        /// </summary>
        /// <param name="image">The image to resize.</param>
        /// <param name="width">The width to resize to.</param>
        /// <param name="height">The height to resize to.</param>
        /// <returns>The resized image.</returns>
        private static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(System.Drawing.Drawing2D.WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }

            return destImage;
        }

        private void saveThumb(Room room)
        {
            int t_w = 256;
            int t_h = 256;
            using (Bitmap b = new Bitmap(t_w, t_h))
            {
                using (Graphics g = Graphics.FromImage(b))
                {
                    IntPtr hdc = g.GetHdc();
                    int r_w = room.Width;
                    int r_h = room.Height;

                    float w_scale = ((float)t_w) / ((float)r_w);
                    float h_scale = ((float)t_h) / ((float)r_h);
                    float scale = Math.Max(w_scale, h_scale);

                    int off_x = (int)((t_w - (r_w * scale)) / 2);
                    int off_y = (int)((t_h - (r_h * scale)) / 2);

                    Factory.NativeProxy.DrawRoomBackground(hdc, room, off_x, off_y, 0, scale, RoomAreaMaskType.None,
                        0, 0);
                    Factory.NativeProxy.RenderBufferToHDC(hdc);
                    g.ReleaseHdc(hdc);
                }
                Bitmap thumb = ResizeImage(b, 96, 96);
                thumb.Save(room.FileName.Replace(".crm",".png"), ImageFormat.Png);
            }
        }
        public RoomChooser(int existingRoom)
        {
            InitializeComponent();

            foreach (var ri in Factory.AGSEditor.CurrentGame.Rooms)
            {
                UnloadedRoom unloaded_room = new UnloadedRoom(ri.Number);
                Room room = Factory.NativeProxy.LoadRoom(unloaded_room);
                saveThumb(room);
            }
        }

        public Room SelectedRoom
        {
            get { return _selectedRoom; }
        }

        public static Room ShowRoomChooser(int currentRoom)
        {
            return ShowRoomChooser(currentRoom, null);
        }

        public static Room ShowRoomChooser(int currentRoom, string text)
        {
            Room selectedRoom = null;
            RoomChooser chooser = new RoomChooser(currentRoom);
            if (text != null)
            {
                chooser.Text = text;
            }
            if (chooser.ShowDialog() == DialogResult.OK)
            {
                selectedRoom = chooser.SelectedRoom;
            }
            chooser.Dispose();
            return selectedRoom;
        }
    }
}
