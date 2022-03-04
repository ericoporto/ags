namespace AGS.Editor
{
    partial class ViewEditor
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.chkShowPreview = new System.Windows.Forms.CheckBox();
            this.editorPanel = new System.Windows.Forms.Panel();
            this.btnDeleteLastLoop = new System.Windows.Forms.Button();
            this.btnNewLoop = new System.Windows.Forms.Button();
            this.sldZoomLevel = new AGS.Editor.ZoomTrackbar();
            this.viewPreview = new AGS.Editor.ViewPreview();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainerPrevAndEd = new System.Windows.Forms.SplitContainer();
            this.checkBoxLockZoom = new System.Windows.Forms.CheckBox();
            this.editorPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerPrevAndEd)).BeginInit();
            this.splitContainerPrevAndEd.Panel1.SuspendLayout();
            this.splitContainerPrevAndEd.Panel2.SuspendLayout();
            this.splitContainerPrevAndEd.SuspendLayout();
            this.SuspendLayout();
            // 
            // chkShowPreview
            // 
            this.chkShowPreview.AutoSize = true;
            this.chkShowPreview.Location = new System.Drawing.Point(3, 14);
            this.chkShowPreview.Name = "chkShowPreview";
            this.chkShowPreview.Size = new System.Drawing.Size(115, 21);
            this.chkShowPreview.TabIndex = 2;
            this.chkShowPreview.Text = "Show Preview";
            this.chkShowPreview.UseVisualStyleBackColor = true;
            this.chkShowPreview.CheckedChanged += new System.EventHandler(this.chkShowPreview_CheckedChanged);
            // 
            // editorPanel
            // 
            this.editorPanel.AutoScroll = true;
            this.editorPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.editorPanel.Controls.Add(this.btnDeleteLastLoop);
            this.editorPanel.Controls.Add(this.btnNewLoop);
            this.editorPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.editorPanel.Location = new System.Drawing.Point(0, 0);
            this.editorPanel.Name = "editorPanel";
            this.editorPanel.Size = new System.Drawing.Size(350, 465);
            this.editorPanel.TabIndex = 4;
            // 
            // btnDeleteLastLoop
            // 
            this.btnDeleteLastLoop.Location = new System.Drawing.Point(187, 72);
            this.btnDeleteLastLoop.Name = "btnDeleteLastLoop";
            this.btnDeleteLastLoop.Size = new System.Drawing.Size(132, 31);
            this.btnDeleteLastLoop.TabIndex = 3;
            this.btnDeleteLastLoop.Text = "Delete last loop";
            this.btnDeleteLastLoop.UseVisualStyleBackColor = true;
            this.btnDeleteLastLoop.Click += new System.EventHandler(this.btnDeleteLastLoop_Click);
            // 
            // btnNewLoop
            // 
            this.btnNewLoop.Location = new System.Drawing.Point(20, 72);
            this.btnNewLoop.Name = "btnNewLoop";
            this.btnNewLoop.Size = new System.Drawing.Size(132, 31);
            this.btnNewLoop.TabIndex = 2;
            this.btnNewLoop.Text = "Create new loop";
            this.btnNewLoop.UseVisualStyleBackColor = true;
            this.btnNewLoop.Click += new System.EventHandler(this.btnNewLoop_Click);
            // 
            // sldZoomLevel
            // 
            this.sldZoomLevel.Location = new System.Drawing.Point(228, 4);
            this.sldZoomLevel.Maximum = 600;
            this.sldZoomLevel.Minimum = 75;
            this.sldZoomLevel.Name = "sldZoomLevel";
            this.sldZoomLevel.Size = new System.Drawing.Size(221, 29);
            this.sldZoomLevel.Step = 25;
            this.sldZoomLevel.TabIndex = 5;
            this.sldZoomLevel.Value = 100;
            this.sldZoomLevel.ZoomScale = 1F;
            this.sldZoomLevel.ValueChanged += new System.EventHandler(this.sldZoomLevel_ValueChanged);
            // 
            // viewPreview
            // 
            this.viewPreview.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.viewPreview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.viewPreview.DynamicUpdates = false;
            this.viewPreview.IsCharacterView = false;
            this.viewPreview.Location = new System.Drawing.Point(0, 0);
            this.viewPreview.Margin = new System.Windows.Forms.Padding(4);
            this.viewPreview.MinimumSize = new System.Drawing.Size(320, 332);
            this.viewPreview.Name = "viewPreview";
            this.viewPreview.Size = new System.Drawing.Size(320, 465);
            this.viewPreview.TabIndex = 3;
            this.viewPreview.Title = "Preview";
            this.viewPreview.ViewToPreview = null;
            this.viewPreview.Visible = false;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.checkBoxLockZoom);
            this.splitContainer1.Panel1.Controls.Add(this.chkShowPreview);
            this.splitContainer1.Panel1.Controls.Add(this.sldZoomLevel);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainerPrevAndEd);
            this.splitContainer1.Size = new System.Drawing.Size(674, 514);
            this.splitContainer1.SplitterDistance = 45;
            this.splitContainer1.TabIndex = 6;
            // 
            // splitContainerPrevAndEd
            // 
            this.splitContainerPrevAndEd.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerPrevAndEd.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainerPrevAndEd.Location = new System.Drawing.Point(0, 0);
            this.splitContainerPrevAndEd.Name = "splitContainerPrevAndEd";
            // 
            // splitContainerPrevAndEd.Panel1
            // 
            this.splitContainerPrevAndEd.Panel1.AutoScroll = true;
            this.splitContainerPrevAndEd.Panel1.Controls.Add(this.viewPreview);
            this.splitContainerPrevAndEd.Panel1MinSize = 320;
            // 
            // splitContainerPrevAndEd.Panel2
            // 
            this.splitContainerPrevAndEd.Panel2.Controls.Add(this.editorPanel);
            this.splitContainerPrevAndEd.Size = new System.Drawing.Size(674, 465);
            this.splitContainerPrevAndEd.SplitterDistance = 320;
            this.splitContainerPrevAndEd.TabIndex = 5;
            // 
            // checkBoxLockZoom
            // 
            this.checkBoxLockZoom.AutoSize = true;
            this.checkBoxLockZoom.Location = new System.Drawing.Point(455, 12);
            this.checkBoxLockZoom.Name = "checkBoxLockZoom";
            this.checkBoxLockZoom.Size = new System.Drawing.Size(150, 21);
            this.checkBoxLockZoom.TabIndex = 6;
            this.checkBoxLockZoom.Text = "Lock Preview Zoom";
            this.checkBoxLockZoom.UseVisualStyleBackColor = true;
            this.checkBoxLockZoom.CheckedChanged += new System.EventHandler(this.checkBoxLockZoom_CheckedChanged);
            // 
            // ViewEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "ViewEditor";
            this.Size = new System.Drawing.Size(674, 514);
            this.Load += new System.EventHandler(this.ViewEditor_Load);
            this.Resize += new System.EventHandler(this.ViewEditor_Resize);
            this.editorPanel.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainerPrevAndEd.Panel1.ResumeLayout(false);
            this.splitContainerPrevAndEd.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerPrevAndEd)).EndInit();
            this.splitContainerPrevAndEd.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.CheckBox chkShowPreview;
		private ViewPreview viewPreview;
		private System.Windows.Forms.Panel editorPanel;
		private System.Windows.Forms.Button btnDeleteLastLoop;
		private System.Windows.Forms.Button btnNewLoop;
		private AGS.Editor.ZoomTrackbar sldZoomLevel;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainerPrevAndEd;
        private System.Windows.Forms.CheckBox checkBoxLockZoom;
    }
}
