namespace AGS.Editor
{
    partial class CharacterEditor
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.viewPreview2 = new AGS.Editor.ViewPreview();
            this.viewPreview1 = new AGS.Editor.ViewPreview();
            this.btnMakePlayer = new System.Windows.Forms.Button();
            this.lblIsPlayer = new System.Windows.Forms.Label();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.splitContainer1);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Size = new System.Drawing.Size(741, 551);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Selected character settings";
            // 
            // viewPreview2
            // 
            this.viewPreview2.AutoSize = true;
            this.viewPreview2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.viewPreview2.DynamicUpdates = false;
            this.viewPreview2.IsCharacterView = false;
            this.viewPreview2.Location = new System.Drawing.Point(357, 5);
            this.viewPreview2.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.viewPreview2.MinimumSize = new System.Drawing.Size(342, 440);
            this.viewPreview2.Name = "viewPreview2";
            this.viewPreview2.Size = new System.Drawing.Size(342, 440);
            this.viewPreview2.TabIndex = 10;
            this.viewPreview2.Title = "Speech view";
            this.viewPreview2.ViewToPreview = null;
            this.viewPreview2.ZoomScale = 1F;
            // 
            // viewPreview1
            // 
            this.viewPreview1.AutoSize = true;
            this.viewPreview1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.viewPreview1.DynamicUpdates = false;
            this.viewPreview1.IsCharacterView = false;
            this.viewPreview1.Location = new System.Drawing.Point(5, 5);
            this.viewPreview1.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.viewPreview1.MinimumSize = new System.Drawing.Size(342, 440);
            this.viewPreview1.Name = "viewPreview1";
            this.viewPreview1.Size = new System.Drawing.Size(342, 440);
            this.viewPreview1.TabIndex = 9;
            this.viewPreview1.Title = "Normal view";
            this.viewPreview1.ViewToPreview = null;
            this.viewPreview1.ZoomScale = 1F;
            // 
            // btnMakePlayer
            // 
            this.btnMakePlayer.Location = new System.Drawing.Point(4, 38);
            this.btnMakePlayer.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnMakePlayer.Name = "btnMakePlayer";
            this.btnMakePlayer.Size = new System.Drawing.Size(235, 29);
            this.btnMakePlayer.TabIndex = 6;
            this.btnMakePlayer.Text = "Make this the player character";
            this.btnMakePlayer.UseVisualStyleBackColor = true;
            this.btnMakePlayer.Click += new System.EventHandler(this.btnMakePlayer_Click);
            // 
            // lblIsPlayer
            // 
            this.lblIsPlayer.AutoSize = true;
            this.lblIsPlayer.Dock = System.Windows.Forms.DockStyle.Top;
            this.lblIsPlayer.Location = new System.Drawing.Point(0, 0);
            this.lblIsPlayer.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblIsPlayer.MaximumSize = new System.Drawing.Size(500, 0);
            this.lblIsPlayer.Name = "lblIsPlayer";
            this.lblIsPlayer.Size = new System.Drawing.Size(470, 34);
            this.lblIsPlayer.TabIndex = 5;
            this.lblIsPlayer.Text = "This character is OR IS NOT the player; the game will startin this character\'s ro" +
    "om or something";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(4, 21);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.lblIsPlayer);
            this.splitContainer1.Panel1.Controls.Add(this.btnMakePlayer);
            this.splitContainer1.Panel1MinSize = 78;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.flowLayoutPanel1);
            this.splitContainer1.Size = new System.Drawing.Size(733, 526);
            this.splitContainer1.SplitterDistance = 78;
            this.splitContainer1.TabIndex = 11;
            this.splitContainer1.TabStop = false;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.AutoSize = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.Controls.Add(this.viewPreview1);
            this.flowLayoutPanel1.Controls.Add(this.viewPreview2);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(733, 444);
            this.flowLayoutPanel1.TabIndex = 11;
            // 
            // CharacterEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(120F, 120F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "CharacterEditor";
            this.Size = new System.Drawing.Size(741, 551);
            this.groupBox1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnMakePlayer;
        private System.Windows.Forms.Label lblIsPlayer;
        private ViewPreview viewPreview2;
        private ViewPreview viewPreview1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    }
}
