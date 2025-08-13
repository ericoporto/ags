namespace AGS.Editor
{
    partial class GoToNumberDialog
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lblNodeNumberRange = new System.Windows.Forms.Label();
            this.upDownNumber = new System.Windows.Forms.NumericUpDown();
            this.btnOk = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.lstNodes = new System.Windows.Forms.ListBox();
            this.tableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanelBottom = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanelEntry = new System.Windows.Forms.TableLayoutPanel();
            this.textBoxFilter = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.upDownNumber)).BeginInit();
            this.tableLayoutPanel.SuspendLayout();
            this.tableLayoutPanelBottom.SuspendLayout();
            this.tableLayoutPanelEntry.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblNodeNumberRange
            // 
            this.lblNodeNumberRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblNodeNumberRange.Location = new System.Drawing.Point(6, 6);
            this.lblNodeNumberRange.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
            this.lblNodeNumberRange.Name = "lblNodeNumberRange";
            this.lblNodeNumberRange.Size = new System.Drawing.Size(214, 13);
            this.lblNodeNumberRange.TabIndex = 0;
            this.lblNodeNumberRange.Text = "Select or type room number:";
            // 
            // upDownNumber
            // 
            this.upDownNumber.Location = new System.Drawing.Point(3, 3);
            this.upDownNumber.Name = "upDownNumber";
            this.upDownNumber.Size = new System.Drawing.Size(100, 20);
            this.upDownNumber.TabIndex = 1;
            this.upDownNumber.ValueChanged += new System.EventHandler(this.upDownNumber_ValueChanged);
            this.upDownNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.upDownNumber_KeyPress);
            this.upDownNumber.KeyUp += new System.Windows.Forms.KeyEventHandler(this.upDownNumber_KeyUp);
            // 
            // btnOk
            // 
            this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOk.Location = new System.Drawing.Point(55, 3);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 4;
            this.btnOk.Text = "&OK";
            this.btnOk.UseVisualStyleBackColor = true;
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(136, 3);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 5;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // lstNodes
            // 
            this.lstNodes.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lstNodes.FormattingEnabled = true;
            this.lstNodes.Location = new System.Drawing.Point(5, 59);
            this.lstNodes.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.lstNodes.Name = "lstNodes";
            this.lstNodes.Size = new System.Drawing.Size(216, 76);
            this.lstNodes.TabIndex = 3;
            this.lstNodes.SelectedIndexChanged += new System.EventHandler(this.lstNodes_SelectedIndexChanged);
            this.lstNodes.Format += new System.Windows.Forms.ListControlConvertEventHandler(this.lstNodes_Format);
            this.lstNodes.SelectedValueChanged += new System.EventHandler(this.lstNodes_SelectedValueChanged);
            this.lstNodes.DoubleClick += new System.EventHandler(this.lstNodes_DoubleClick);
            // 
            // tableLayoutPanel
            // 
            this.tableLayoutPanel.AutoSize = true;
            this.tableLayoutPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel.ColumnCount = 1;
            this.tableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel.Controls.Add(this.lstNodes, 0, 2);
            this.tableLayoutPanel.Controls.Add(this.tableLayoutPanelBottom, 0, 4);
            this.tableLayoutPanel.Controls.Add(this.lblNodeNumberRange, 0, 0);
            this.tableLayoutPanel.Controls.Add(this.tableLayoutPanelEntry, 0, 1);
            this.tableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel.Margin = new System.Windows.Forms.Padding(3, 6, 3, 0);
            this.tableLayoutPanel.Name = "tableLayoutPanel";
            this.tableLayoutPanel.Padding = new System.Windows.Forms.Padding(3, 0, 3, 0);
            this.tableLayoutPanel.RowCount = 5;
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel.Size = new System.Drawing.Size(226, 180);
            this.tableLayoutPanel.TabIndex = 5;
            // 
            // tableLayoutPanelBottom
            // 
            this.tableLayoutPanelBottom.AutoSize = true;
            this.tableLayoutPanelBottom.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanelBottom.ColumnCount = 3;
            this.tableLayoutPanelBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanelBottom.Controls.Add(this.btnCancel, 2, 0);
            this.tableLayoutPanelBottom.Controls.Add(this.btnOk, 1, 0);
            this.tableLayoutPanelBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelBottom.Location = new System.Drawing.Point(6, 148);
            this.tableLayoutPanelBottom.Name = "tableLayoutPanelBottom";
            this.tableLayoutPanelBottom.RowCount = 1;
            this.tableLayoutPanelBottom.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanelBottom.Size = new System.Drawing.Size(214, 29);
            this.tableLayoutPanelBottom.TabIndex = 5;
            // 
            // tableLayoutPanelEntry
            // 
            this.tableLayoutPanelEntry.AutoSize = true;
            this.tableLayoutPanelEntry.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanelEntry.ColumnCount = 2;
            this.tableLayoutPanelEntry.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelEntry.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelEntry.Controls.Add(this.upDownNumber, 0, 0);
            this.tableLayoutPanelEntry.Controls.Add(this.textBoxFilter, 1, 0);
            this.tableLayoutPanelEntry.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelEntry.Location = new System.Drawing.Point(6, 28);
            this.tableLayoutPanelEntry.Name = "tableLayoutPanelEntry";
            this.tableLayoutPanelEntry.RowCount = 1;
            this.tableLayoutPanelEntry.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelEntry.Size = new System.Drawing.Size(214, 26);
            this.tableLayoutPanelEntry.TabIndex = 6;
            // 
            // textBoxFilter
            // 
            this.textBoxFilter.Location = new System.Drawing.Point(110, 3);
            this.textBoxFilter.Name = "textBoxFilter";
            this.textBoxFilter.Size = new System.Drawing.Size(100, 20);
            this.textBoxFilter.TabIndex = 2;
            this.textBoxFilter.TextChanged += new System.EventHandler(this.textBoxFilter_TextChanged);
            // 
            // GoToNumberDialog
            // 
            this.AcceptButton = this.btnOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(226, 180);
            this.Controls.Add(this.tableLayoutPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "GoToNumberDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Go To Number";
            this.TopMost = true;
            ((System.ComponentModel.ISupportInitialize)(this.upDownNumber)).EndInit();
            this.tableLayoutPanel.ResumeLayout(false);
            this.tableLayoutPanel.PerformLayout();
            this.tableLayoutPanelBottom.ResumeLayout(false);
            this.tableLayoutPanelEntry.ResumeLayout(false);
            this.tableLayoutPanelEntry.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblNodeNumberRange;
        private System.Windows.Forms.NumericUpDown upDownNumber;
        private System.Windows.Forms.Button btnOk;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ListBox lstNodes;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelBottom;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelEntry;
        private System.Windows.Forms.TextBox textBoxFilter;
    }
}