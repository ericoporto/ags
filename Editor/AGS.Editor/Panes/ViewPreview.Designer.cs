namespace AGS.Editor
{
    partial class ViewPreview
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
            StopTimer();
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
            this.mainGroupBox = new System.Windows.Forms.GroupBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panel1 = new System.Windows.Forms.Panel();
            this.sldZoomLevel = new AGS.Editor.ZoomTrackbar();
            this.label1 = new System.Windows.Forms.Label();
            this.udLoop = new System.Windows.Forms.NumericUpDown();
            this.chkSkipFrame0 = new System.Windows.Forms.CheckBox();
            this.udFrame = new System.Windows.Forms.NumericUpDown();
            this.chkCentrePivot = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.chkAnimate = new System.Windows.Forms.CheckBox();
            this.udDelay = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.previewPanel = new AGS.Editor.BufferedPanel();
            this.mainGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udLoop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFrame)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDelay)).BeginInit();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainGroupBox
            // 
            this.mainGroupBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.mainGroupBox.Controls.Add(this.splitContainer1);
            this.mainGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainGroupBox.Location = new System.Drawing.Point(0, 0);
            this.mainGroupBox.Margin = new System.Windows.Forms.Padding(4);
            this.mainGroupBox.Name = "mainGroupBox";
            this.mainGroupBox.Padding = new System.Windows.Forms.Padding(4);
            this.mainGroupBox.Size = new System.Drawing.Size(342, 409);
            this.mainGroupBox.TabIndex = 0;
            this.mainGroupBox.TabStop = false;
            this.mainGroupBox.Text = "groupBox1";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(4, 19);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.panel1);
            this.splitContainer1.Panel1MinSize = 135;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel1);
            this.splitContainer1.Size = new System.Drawing.Size(334, 386);
            this.splitContainer1.SplitterDistance = 135;
            this.splitContainer1.TabIndex = 12;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.sldZoomLevel);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.udLoop);
            this.panel1.Controls.Add(this.chkSkipFrame0);
            this.panel1.Controls.Add(this.udFrame);
            this.panel1.Controls.Add(this.chkCentrePivot);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.chkAnimate);
            this.panel1.Controls.Add(this.udDelay);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Location = new System.Drawing.Point(3, 3);
            this.panel1.MaximumSize = new System.Drawing.Size(300, 135);
            this.panel1.MinimumSize = new System.Drawing.Size(300, 135);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(300, 135);
            this.panel1.TabIndex = 10;
            // 
            // sldZoomLevel
            // 
            this.sldZoomLevel.Location = new System.Drawing.Point(32, 103);
            this.sldZoomLevel.Maximum = 600;
            this.sldZoomLevel.Minimum = 25;
            this.sldZoomLevel.Name = "sldZoomLevel";
            this.sldZoomLevel.Size = new System.Drawing.Size(221, 29);
            this.sldZoomLevel.Step = 25;
            this.sldZoomLevel.TabIndex = 9;
            this.sldZoomLevel.Value = 100;
            this.sldZoomLevel.ZoomScale = 1F;
            this.sldZoomLevel.ValueChanged += new System.EventHandler(this.sldZoomLevel_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(28, 2);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Loop:";
            // 
            // udLoop
            // 
            this.udLoop.Location = new System.Drawing.Point(32, 22);
            this.udLoop.Margin = new System.Windows.Forms.Padding(4);
            this.udLoop.Name = "udLoop";
            this.udLoop.Size = new System.Drawing.Size(40, 22);
            this.udLoop.TabIndex = 0;
            this.udLoop.ValueChanged += new System.EventHandler(this.udLoop_ValueChanged);
            // 
            // chkSkipFrame0
            // 
            this.chkSkipFrame0.AutoSize = true;
            this.chkSkipFrame0.Location = new System.Drawing.Point(119, 82);
            this.chkSkipFrame0.Margin = new System.Windows.Forms.Padding(4);
            this.chkSkipFrame0.Name = "chkSkipFrame0";
            this.chkSkipFrame0.Size = new System.Drawing.Size(177, 21);
            this.chkSkipFrame0.TabIndex = 8;
            this.chkSkipFrame0.Text = "Skip frame 0 (standing)";
            this.chkSkipFrame0.UseVisualStyleBackColor = true;
            // 
            // udFrame
            // 
            this.udFrame.Location = new System.Drawing.Point(119, 22);
            this.udFrame.Margin = new System.Windows.Forms.Padding(4);
            this.udFrame.Name = "udFrame";
            this.udFrame.Size = new System.Drawing.Size(40, 22);
            this.udFrame.TabIndex = 2;
            this.udFrame.ValueChanged += new System.EventHandler(this.udFrame_ValueChanged);
            // 
            // chkCentrePivot
            // 
            this.chkCentrePivot.AutoSize = true;
            this.chkCentrePivot.Location = new System.Drawing.Point(32, 56);
            this.chkCentrePivot.Margin = new System.Windows.Forms.Padding(4);
            this.chkCentrePivot.Name = "chkCentrePivot";
            this.chkCentrePivot.Size = new System.Drawing.Size(211, 21);
            this.chkCentrePivot.TabIndex = 6;
            this.chkCentrePivot.Text = "Character view (centre pivot)";
            this.chkCentrePivot.UseVisualStyleBackColor = true;
            this.chkCentrePivot.CheckedChanged += new System.EventHandler(this.chkCentrePivot_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(116, 2);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(52, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "Frame:";
            // 
            // chkAnimate
            // 
            this.chkAnimate.AutoSize = true;
            this.chkAnimate.Location = new System.Drawing.Point(32, 82);
            this.chkAnimate.Margin = new System.Windows.Forms.Padding(4);
            this.chkAnimate.Name = "chkAnimate";
            this.chkAnimate.Size = new System.Drawing.Size(81, 21);
            this.chkAnimate.TabIndex = 7;
            this.chkAnimate.Text = "Animate";
            this.chkAnimate.UseVisualStyleBackColor = true;
            this.chkAnimate.CheckedChanged += new System.EventHandler(this.chkAnimate_CheckedChanged);
            // 
            // udDelay
            // 
            this.udDelay.Location = new System.Drawing.Point(208, 22);
            this.udDelay.Margin = new System.Windows.Forms.Padding(4);
            this.udDelay.Name = "udDelay";
            this.udDelay.Size = new System.Drawing.Size(40, 22);
            this.udDelay.TabIndex = 4;
            this.udDelay.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.udDelay.ValueChanged += new System.EventHandler(this.udDelay_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(204, 2);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(48, 17);
            this.label3.TabIndex = 5;
            this.label3.Text = "Delay:";
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.AutoSize = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.Controls.Add(this.previewPanel);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(334, 247);
            this.flowLayoutPanel1.TabIndex = 11;
            this.flowLayoutPanel1.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.flowLayoutPanel1_MouseWheel);
            // 
            // previewPanel
            // 
            this.previewPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.previewPanel.Location = new System.Drawing.Point(4, 4);
            this.previewPanel.Margin = new System.Windows.Forms.Padding(4);
            this.previewPanel.MinimumSize = new System.Drawing.Size(300, 224);
            this.previewPanel.Name = "previewPanel";
            this.previewPanel.Size = new System.Drawing.Size(300, 224);
            this.previewPanel.TabIndex = 9;
            this.previewPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.previewPanel_Paint);
            // 
            // ViewPreview
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(120F, 120F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.mainGroupBox);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "ViewPreview";
            this.Size = new System.Drawing.Size(342, 409);
            this.mainGroupBox.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.udLoop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udFrame)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.udDelay)).EndInit();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox mainGroupBox;
        private System.Windows.Forms.CheckBox chkAnimate;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown udDelay;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown udFrame;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown udLoop;
        private BufferedPanel previewPanel;
        private System.Windows.Forms.CheckBox chkSkipFrame0;
        private System.Windows.Forms.CheckBox chkCentrePivot;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private ZoomTrackbar sldZoomLevel;
        private System.Windows.Forms.SplitContainer splitContainer1;
    }
}
