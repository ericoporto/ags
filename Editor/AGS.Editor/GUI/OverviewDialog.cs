using AGS.Editor.Utils;
using AGS.Types;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AGS.Editor
{
    public partial class OverviewDialog : Form
    {
        private int _selectedNumber;
        private List<NodeItemInfoWithSprite> _list;
        private Dictionary<int, Image> _thumbnailCache;
        private Dictionary<int, List<int>> _spriteToIndices;
        private CancellationTokenSource _cts;

        // avoid duplicate loads
        private readonly HashSet<int> _loadingSprites = new HashSet<int>();

        private readonly Bitmap _placeholder = SpriteTools.GetPlaceHolder(96, 96);

        public OverviewDialog()
        {
            InitializeComponent();

            _cts = new CancellationTokenSource();
            _thumbnailCache = new Dictionary<int, Image>();
            _spriteToIndices = new Dictionary<int, List<int>>();

            // smooth scroll, less flicker
            try
            {
                typeof(ListView).InvokeMember("DoubleBuffered",
                    System.Reflection.BindingFlags.NonPublic |
                    System.Reflection.BindingFlags.Instance |
                    System.Reflection.BindingFlags.SetProperty,
                    null, listViewAssets, new object[] { true });
            }
            catch { /* ignore if not available */ }

            // Image list for thumbnails
            var il = new ImageList();
            il.ImageSize = new Size(96, 96);
            il.ColorDepth = ColorDepth.Depth32Bit;
            listViewAssets.LargeImageList = il;
        }

        public List<NodeItemInfoWithSprite> List
        {
            get { return _list; }
            set
            {
                _list = value ?? new List<NodeItemInfoWithSprite>();

                // revise this and simplify?
                _spriteToIndices.Clear();
                for (int i = 0; i < _list.Count; i++)
                {
                    int sid = _list[i].Sprite;
                    List<int> arr;
                    if (!_spriteToIndices.TryGetValue(sid, out arr))
                    {
                        arr = new List<int>(4);
                        _spriteToIndices[sid] = arr;
                    }
                    arr.Add(i);
                }

                // Reflect count to the virtual list
                listViewAssets.VirtualListSize = _list.Count;
                listViewAssets.Invalidate();
            }
        }

        public int Number
        {
            get { return _selectedNumber; }
            set { _selectedNumber = value; }
        }

        // Ensure the image for this sprite key exists in LargeImageList.
        // If key already exists (placeholder or real), return its index.
        private int EnsureImageInList(NodeItemInfoWithSprite node, Image img)
        {
            ImageList il = listViewAssets.LargeImageList;
            string key = node.Sprite.ToString();

            int idx = il.Images.IndexOfKey(key);
            if (idx == -1)
            {
                il.Images.Add(key, img);
                idx = il.Images.Count - 1;
            }
            return idx;
        }

        // Replace the image associated with a sprite key (placeholder -> real)
        private void ReplaceImageInList(int spriteId, Image newImg)
        {
            ImageList il = listViewAssets.LargeImageList;
            string key = spriteId.ToString();
            int idx = il.Images.IndexOfKey(key);
            if (idx >= 0)
            {
                il.Images[idx] = newImg; // keeps the index stable
            }
            else
            {
                il.Images.Add(key, newImg);
            }
        }

        private async Task LoadThumbnailAsync(NodeItemInfoWithSprite nodeItem)
        {
            // De-duplicate loads for the same sprite
            lock (_loadingSprites)
            {
                if (_loadingSprites.Contains(nodeItem.Sprite))
                    return;
                _loadingSprites.Add(nodeItem.Sprite);
            }

            try
            {
                Image img = await Task.Run(() =>
                {
                    Sprite sprite = Factory.AGSEditor.CurrentGame.RootSpriteFolder.FindSpriteByID(nodeItem.Sprite, true);
                    return Utilities.GetBitmapForSpriteResizedKeepingAspectRatio(sprite, 96, 96, true, true, listViewAssets.BackColor);
                }, _cts.Token);

                if (img == null) img = _placeholder;

                // Cache and swap into the imagelist on UI thread
                _thumbnailCache[nodeItem.Sprite] = img;

                listViewAssets.BeginInvoke(new Action(() =>
                {
                    ReplaceImageInList(nodeItem.Sprite, img);

                    // Redraw all items that use this sprite (batched contiguous ranges)
                    List<int> indices;
                    if (_spriteToIndices.TryGetValue(nodeItem.Sprite, out indices) && indices.Count > 0)
                    {
                        int start = indices[0];
                        int end = start;
                        for (int k = 1; k < indices.Count; k++)
                        {
                            if (indices[k] == end + 1) end = indices[k];
                            else
                            {
                                listViewAssets.RedrawItems(start, end, false);
                                start = end = indices[k];
                            }
                        }
                        listViewAssets.RedrawItems(start, end, false);
                    }
                }));
            }
            catch
            {
                // ignore; we keep placeholder
            }
            finally
            {
                lock (_loadingSprites) { _loadingSprites.Remove(nodeItem.Sprite); }
            }
        }

        private void listViewAssets_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            if (_list == null || _list.Count == 0)
            {
                e.Item = new ListViewItem(string.Empty);
                return;
            }

            NodeItemInfoWithSprite node = _list[e.ItemIndex];

            Image img;
            if (!_thumbnailCache.TryGetValue(node.Sprite, out img))
            {
                // Use shared placeholder for now
                img = _placeholder;

                // Ensure the placeholder is present and get its index
                int _ = EnsureImageInList(node, img);

                // background load
                Task t = LoadThumbnailAsync(node);
            }

            // Map to the image key (same for all items sharing the sprite)
            int imageIndex = EnsureImageInList(node, img);

            var lvi = new ListViewItem(node.Name);
            lvi.ToolTipText = node.Description;
            lvi.ImageIndex = imageIndex;
            e.Item = lvi;
        }

        private void SyncSelectedNumber()
        {
            if (listViewAssets.SelectedIndices.Count > 0)
            {
                _selectedNumber = _list[listViewAssets.SelectedIndices[0]].ID;
            }
            else
            {
                _selectedNumber = -1;
            }
        }

        private void listViewAssets_ItemActivate(object sender, EventArgs e)
        {
            SyncSelectedNumber();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            _cts.Cancel();
            base.OnFormClosing(e);
        }

        private void ConfirmAndExit()
        {
            SyncSelectedNumber();
            DialogResult = DialogResult.OK;
            Close();
        }

        private void btnGoTo_Click(object sender, EventArgs e)
        {
            ConfirmAndExit();
        }

        private void listViewAssets_DoubleClick(object sender, EventArgs e)
        {
            ConfirmAndExit();
        }
    }
}
