namespace AGS.Editor
{
    partial class PaletteEditor
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
            this.tabControl = new System.Windows.Forms.TabControl();
            this.palettePage = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panelPaletteIntroText = new System.Windows.Forms.Panel();
            this.flowLayoutPalettePanel = new System.Windows.Forms.FlowLayoutPanel();
            this.lblPaletteIntro = new System.Windows.Forms.Label();
            this.lblPaletteIntro2 = new System.Windows.Forms.Label();
            this.colourFinderPage = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.flowLayoutColorPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.lblColorFinderIntro = new System.Windows.Forms.Label();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.btnColorDialog = new System.Windows.Forms.Button();
            this.txtColourNumber = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.trackBarRed = new System.Windows.Forms.TrackBar();
            this.lblBlueVal = new System.Windows.Forms.Label();
            this.trackBarGreen = new System.Windows.Forms.TrackBar();
            this.lblGreenVal = new System.Windows.Forms.Label();
            this.trackBarBlue = new System.Windows.Forms.TrackBar();
            this.lblRedVal = new System.Windows.Forms.Label();
            this.lblFixedColorsWarning = new System.Windows.Forms.Label();
            this.palettePanel = new AGS.Editor.BufferedPanel();
            this.blockOfColour = new AGS.Editor.BufferedPanel();
            this.tabControl.SuspendLayout();
            this.palettePage.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.panelPaletteIntroText.SuspendLayout();
            this.flowLayoutPalettePanel.SuspendLayout();
            this.colourFinderPage.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.flowLayoutColorPanel.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarBlue)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.palettePage);
            this.tabControl.Controls.Add(this.colourFinderPage);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.MaximumSize = new System.Drawing.Size(800, 800);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(800, 800);
            this.tabControl.TabIndex = 2;
            // 
            // palettePage
            // 
            this.palettePage.Controls.Add(this.groupBox2);
            this.palettePage.Location = new System.Drawing.Point(4, 26);
            this.palettePage.Name = "palettePage";
            this.palettePage.Padding = new System.Windows.Forms.Padding(3);
            this.palettePage.Size = new System.Drawing.Size(792, 770);
            this.palettePage.TabIndex = 0;
            this.palettePage.Text = "Palette";
            this.palettePage.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.splitContainer1);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Location = new System.Drawing.Point(3, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(786, 765);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Palette";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(3, 20);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.palettePanel);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.panelPaletteIntroText);
            this.splitContainer1.Size = new System.Drawing.Size(780, 742);
            this.splitContainer1.SplitterDistance = 580;
            this.splitContainer1.TabIndex = 2;
            // 
            // panelPaletteIntroText
            // 
            this.panelPaletteIntroText.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panelPaletteIntroText.Controls.Add(this.flowLayoutPalettePanel);
            this.panelPaletteIntroText.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelPaletteIntroText.Location = new System.Drawing.Point(0, 0);
            this.panelPaletteIntroText.Name = "panelPaletteIntroText";
            this.panelPaletteIntroText.Size = new System.Drawing.Size(780, 158);
            this.panelPaletteIntroText.TabIndex = 1;
            this.panelPaletteIntroText.ClientSizeChanged += new System.EventHandler(this.panelPaletteIntroText_ClientSizeChanged);
            this.panelPaletteIntroText.Layout += new System.Windows.Forms.LayoutEventHandler(this.panelPaletteIntroText_Layout);
            // 
            // flowLayoutPalettePanel
            // 
            this.flowLayoutPalettePanel.AutoSize = true;
            this.flowLayoutPalettePanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPalettePanel.Controls.Add(this.lblPaletteIntro);
            this.flowLayoutPalettePanel.Controls.Add(this.lblPaletteIntro2);
            this.flowLayoutPalettePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPalettePanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPalettePanel.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPalettePanel.Name = "flowLayoutPalettePanel";
            this.flowLayoutPalettePanel.Size = new System.Drawing.Size(780, 158);
            this.flowLayoutPalettePanel.TabIndex = 0;
            // 
            // lblPaletteIntro
            // 
            this.lblPaletteIntro.AutoSize = true;
            this.lblPaletteIntro.Location = new System.Drawing.Point(3, 0);
            this.lblPaletteIntro.MaximumSize = new System.Drawing.Size(619, 40);
            this.lblPaletteIntro.Name = "lblPaletteIntro";
            this.lblPaletteIntro.Size = new System.Drawing.Size(565, 34);
            this.lblPaletteIntro.TabIndex = 0;
            this.lblPaletteIntro.Text = "This palette information will only be used for drawing any 8-bit graphics that yo" +
    "u may have imported.";
            // 
            // lblPaletteIntro2
            // 
            this.lblPaletteIntro2.AutoSize = true;
            this.lblPaletteIntro2.Location = new System.Drawing.Point(3, 34);
            this.lblPaletteIntro2.MaximumSize = new System.Drawing.Size(400, 0);
            this.lblPaletteIntro2.Name = "lblPaletteIntro2";
            this.lblPaletteIntro2.Size = new System.Drawing.Size(389, 0);
            this.lblPaletteIntro2.TabIndex = 2;
            this.lblPaletteIntro2.Text = "Click in the grid below to select a colour. Control-click to select additional co" +
    "lours; Shift-click to select a range.  Right click to import/export.";
            // 
            // colourFinderPage
            // 
            this.colourFinderPage.Controls.Add(this.groupBox1);
            this.colourFinderPage.Location = new System.Drawing.Point(4, 26);
            this.colourFinderPage.Name = "colourFinderPage";
            this.colourFinderPage.Padding = new System.Windows.Forms.Padding(3);
            this.colourFinderPage.Size = new System.Drawing.Size(792, 770);
            this.colourFinderPage.TabIndex = 1;
            this.colourFinderPage.Text = "Colour Finder";
            this.colourFinderPage.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.flowLayoutColorPanel);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(786, 765);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Colour Finder";
            // 
            // flowLayoutColorPanel
            // 
            this.flowLayoutColorPanel.AutoSize = true;
            this.flowLayoutColorPanel.Controls.Add(this.lblColorFinderIntro);
            this.flowLayoutColorPanel.Controls.Add(this.tableLayoutPanel2);
            this.flowLayoutColorPanel.Controls.Add(this.tableLayoutPanel1);
            this.flowLayoutColorPanel.Controls.Add(this.blockOfColour);
            this.flowLayoutColorPanel.Controls.Add(this.lblFixedColorsWarning);
            this.flowLayoutColorPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutColorPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutColorPanel.Location = new System.Drawing.Point(3, 20);
            this.flowLayoutColorPanel.Name = "flowLayoutColorPanel";
            this.flowLayoutColorPanel.Size = new System.Drawing.Size(780, 742);
            this.flowLayoutColorPanel.TabIndex = 17;
            this.flowLayoutColorPanel.ClientSizeChanged += new System.EventHandler(this.flowLayoutColorPanel_ClientSizeChanged);
            this.flowLayoutColorPanel.Layout += new System.Windows.Forms.LayoutEventHandler(this.flowLayoutColorPanel_Layout);
            // 
            // lblColorFinderIntro
            // 
            this.lblColorFinderIntro.AutoSize = true;
            this.lblColorFinderIntro.Location = new System.Drawing.Point(3, 0);
            this.lblColorFinderIntro.MaximumSize = new System.Drawing.Size(350, 0);
            this.lblColorFinderIntro.Name = "lblColorFinderIntro";
            this.lblColorFinderIntro.Size = new System.Drawing.Size(343, 0);
            this.lblColorFinderIntro.TabIndex = 0;
            this.lblColorFinderIntro.Text = "You can use the controls below to find the AGS Colour Number for a particular col" +
    "our.";
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 41.71598F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 58.28402F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 185F));
            this.tableLayoutPanel2.Controls.Add(this.btnColorDialog, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.txtColourNumber, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.label2, 0, 0);
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 37);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(522, 44);
            this.tableLayoutPanel2.TabIndex = 16;
            // 
            // btnColorDialog
            // 
            this.btnColorDialog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnColorDialog.Location = new System.Drawing.Point(339, 3);
            this.btnColorDialog.Name = "btnColorDialog";
            this.btnColorDialog.Size = new System.Drawing.Size(180, 38);
            this.btnColorDialog.TabIndex = 14;
            this.btnColorDialog.Text = "Find Colour...";
            this.btnColorDialog.UseVisualStyleBackColor = true;
            this.btnColorDialog.Click += new System.EventHandler(this.btnColorDialog_Click);
            // 
            // txtColourNumber
            // 
            this.txtColourNumber.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtColourNumber.Location = new System.Drawing.Point(143, 3);
            this.txtColourNumber.MaxLength = 5;
            this.txtColourNumber.Name = "txtColourNumber";
            this.txtColourNumber.Size = new System.Drawing.Size(190, 24);
            this.txtColourNumber.TabIndex = 2;
            this.txtColourNumber.Text = "0";
            this.txtColourNumber.TextChanged += new System.EventHandler(this.txtColourNumber_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(134, 44);
            this.label2.TabIndex = 1;
            this.label2.Text = "Colour number:";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.7734F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.2266F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 81F));
            this.tableLayoutPanel1.Controls.Add(this.label3, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.label4, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label5, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.trackBarRed, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblBlueVal, 2, 2);
            this.tableLayoutPanel1.Controls.Add(this.trackBarGreen, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblGreenVal, 2, 1);
            this.tableLayoutPanel1.Controls.Add(this.trackBarBlue, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.lblRedVal, 2, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 87);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(522, 158);
            this.tableLayoutPanel1.TabIndex = 15;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 17);
            this.label3.TabIndex = 4;
            this.label3.Text = "Red:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 54);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(49, 17);
            this.label4.TabIndex = 6;
            this.label4.Text = "Green:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 108);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(38, 17);
            this.label5.TabIndex = 8;
            this.label5.Text = "Blue:";
            // 
            // trackBarRed
            // 
            this.trackBarRed.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBarRed.LargeChange = 40;
            this.trackBarRed.Location = new System.Drawing.Point(143, 3);
            this.trackBarRed.Maximum = 255;
            this.trackBarRed.Name = "trackBarRed";
            this.trackBarRed.Size = new System.Drawing.Size(294, 48);
            this.trackBarRed.SmallChange = 8;
            this.trackBarRed.TabIndex = 3;
            this.trackBarRed.TickFrequency = 16;
            this.trackBarRed.Scroll += new System.EventHandler(this.trackBarRed_Scroll);
            // 
            // lblBlueVal
            // 
            this.lblBlueVal.AutoSize = true;
            this.lblBlueVal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblBlueVal.Location = new System.Drawing.Point(443, 108);
            this.lblBlueVal.Name = "lblBlueVal";
            this.lblBlueVal.Size = new System.Drawing.Size(76, 50);
            this.lblBlueVal.TabIndex = 11;
            this.lblBlueVal.Text = "0";
            // 
            // trackBarGreen
            // 
            this.trackBarGreen.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBarGreen.LargeChange = 40;
            this.trackBarGreen.Location = new System.Drawing.Point(143, 57);
            this.trackBarGreen.Maximum = 255;
            this.trackBarGreen.Name = "trackBarGreen";
            this.trackBarGreen.Size = new System.Drawing.Size(294, 48);
            this.trackBarGreen.SmallChange = 8;
            this.trackBarGreen.TabIndex = 5;
            this.trackBarGreen.TickFrequency = 16;
            this.trackBarGreen.Scroll += new System.EventHandler(this.trackBarGreen_Scroll);
            // 
            // lblGreenVal
            // 
            this.lblGreenVal.AutoSize = true;
            this.lblGreenVal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblGreenVal.Location = new System.Drawing.Point(443, 54);
            this.lblGreenVal.Name = "lblGreenVal";
            this.lblGreenVal.Size = new System.Drawing.Size(76, 54);
            this.lblGreenVal.TabIndex = 10;
            this.lblGreenVal.Text = "0";
            // 
            // trackBarBlue
            // 
            this.trackBarBlue.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBarBlue.LargeChange = 40;
            this.trackBarBlue.Location = new System.Drawing.Point(143, 111);
            this.trackBarBlue.Maximum = 255;
            this.trackBarBlue.Name = "trackBarBlue";
            this.trackBarBlue.Size = new System.Drawing.Size(294, 44);
            this.trackBarBlue.SmallChange = 8;
            this.trackBarBlue.TabIndex = 7;
            this.trackBarBlue.TickFrequency = 16;
            this.trackBarBlue.Scroll += new System.EventHandler(this.trackBarBlue_Scroll);
            // 
            // lblRedVal
            // 
            this.lblRedVal.AutoSize = true;
            this.lblRedVal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRedVal.Location = new System.Drawing.Point(443, 0);
            this.lblRedVal.Name = "lblRedVal";
            this.lblRedVal.Size = new System.Drawing.Size(76, 54);
            this.lblRedVal.TabIndex = 9;
            this.lblRedVal.Text = "0";
            // 
            // lblFixedColorsWarning
            // 
            this.lblFixedColorsWarning.AutoSize = true;
            this.lblFixedColorsWarning.Location = new System.Drawing.Point(3, 301);
            this.lblFixedColorsWarning.MaximumSize = new System.Drawing.Size(350, 0);
            this.lblFixedColorsWarning.Name = "lblFixedColorsWarning";
            this.lblFixedColorsWarning.Size = new System.Drawing.Size(350, 0);
            this.lblFixedColorsWarning.TabIndex = 13;
            this.lblFixedColorsWarning.Text = "NOTE: Colours 1-31 are locked to reflect special colours in the 8-bit palette. Fo" +
    "r shades of blue, set the Green slider to 4.";
            this.lblFixedColorsWarning.Visible = false;
            // 
            // palettePanel
            // 
            this.palettePanel.AutoSize = true;
            this.palettePanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.palettePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.palettePanel.Location = new System.Drawing.Point(0, 0);
            this.palettePanel.Name = "palettePanel";
            this.palettePanel.Size = new System.Drawing.Size(780, 580);
            this.palettePanel.TabIndex = 1;
            this.palettePanel.Paint += new System.Windows.Forms.PaintEventHandler(this.palettePanel_Paint);
            this.palettePanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.palettePanel_MouseDown);
            // 
            // blockOfColour
            // 
            this.blockOfColour.Location = new System.Drawing.Point(3, 251);
            this.blockOfColour.Name = "blockOfColour";
            this.blockOfColour.Size = new System.Drawing.Size(519, 47);
            this.blockOfColour.TabIndex = 12;
            this.blockOfColour.Paint += new System.Windows.Forms.PaintEventHandler(this.blockOfColour_Paint);
            // 
            // PaletteEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "PaletteEditor";
            this.Size = new System.Drawing.Size(800, 800);
            this.Load += new System.EventHandler(this.PaletteEditor_Load);
            this.tabControl.ResumeLayout(false);
            this.palettePage.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.panelPaletteIntroText.ResumeLayout(false);
            this.panelPaletteIntroText.PerformLayout();
            this.flowLayoutPalettePanel.ResumeLayout(false);
            this.flowLayoutPalettePanel.PerformLayout();
            this.colourFinderPage.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.flowLayoutColorPanel.ResumeLayout(false);
            this.flowLayoutColorPanel.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarBlue)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage palettePage;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label lblPaletteIntro;
        private System.Windows.Forms.TabPage colourFinderPage;
        private System.Windows.Forms.GroupBox groupBox1;
        private BufferedPanel blockOfColour;
        private System.Windows.Forms.Label lblBlueVal;
        private System.Windows.Forms.Label lblGreenVal;
        private System.Windows.Forms.Label lblRedVal;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar trackBarBlue;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar trackBarGreen;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar trackBarRed;
        private System.Windows.Forms.TextBox txtColourNumber;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblColorFinderIntro;
        private BufferedPanel palettePanel;
        private System.Windows.Forms.Label lblPaletteIntro2;
        private System.Windows.Forms.Label lblFixedColorsWarning;
		private System.Windows.Forms.Button btnColorDialog;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Panel panelPaletteIntroText;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPalettePanel;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutColorPanel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
    }
}
