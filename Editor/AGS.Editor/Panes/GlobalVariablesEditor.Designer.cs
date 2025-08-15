namespace AGS.Editor
{
    partial class GlobalVariablesEditor
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
            this.mainFrame = new System.Windows.Forms.GroupBox();
            this.globalVariableList = new System.Windows.Forms.ListView();
            this.clmName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clmType = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.clmDefaultValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.label1 = new System.Windows.Forms.Label();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.folderList = new AGS.Editor.TreeViewWithDragDrop();
            this.mainFrame.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainFrame
            // 
            this.mainFrame.Controls.Add(this.splitContainer1);
            this.mainFrame.Controls.Add(this.label1);
            this.mainFrame.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainFrame.Location = new System.Drawing.Point(3, 3);
            this.mainFrame.Name = "mainFrame";
            this.mainFrame.Size = new System.Drawing.Size(525, 513);
            this.mainFrame.TabIndex = 0;
            this.mainFrame.TabStop = false;
            this.mainFrame.Text = "Global Variables";
            // 
            // lvwWords
            // 
            this.globalVariableList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.clmName,
            this.clmType,
            this.clmDefaultValue});
            this.globalVariableList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.globalVariableList.FullRowSelect = true;
            this.globalVariableList.HideSelection = false;
            this.globalVariableList.Location = new System.Drawing.Point(0, 0);
            this.globalVariableList.MultiSelect = false;
            this.globalVariableList.Name = "globalVariableList";
            this.globalVariableList.Size = new System.Drawing.Size(342, 446);
            this.globalVariableList.TabIndex = 1;
            this.globalVariableList.UseCompatibleStateImageBehavior = false;
            this.globalVariableList.View = System.Windows.Forms.View.Details;
            this.globalVariableList.ItemActivate += new System.EventHandler(this.globalVariableList_ItemActivate);
            this.globalVariableList.MouseUp += new System.Windows.Forms.MouseEventHandler(this.globalVariableList_MouseUp);
            // 
            // clmName
            // 
            this.clmName.Text = "Name";
            this.clmName.Width = 150;
            // 
            // clmType
            // 
            this.clmType.Text = "Type";
            this.clmType.Width = 110;
            // 
            // clmDefaultValue
            // 
            this.clmDefaultValue.Text = "Initial Value";
            this.clmDefaultValue.Width = 190;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(3, 17);
            this.label1.MaximumSize = new System.Drawing.Size(502, 0);
            this.label1.Name = "label1";
            this.label1.Padding = new System.Windows.Forms.Padding(3, 9, 3, 12);
            this.label1.Size = new System.Drawing.Size(501, 47);
            this.label1.TabIndex = 0;
            this.label1.Text = "Global variables allow you to create variables which you can access from all your" +
    " scripts. Right-click in the list to add, modify and delete variables.";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 64);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.globalVariableList);
            this.splitContainer1.Size = new System.Drawing.Size(519, 446);
            this.splitContainer1.SplitterDistance = 173;
            this.splitContainer1.TabIndex = 2;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.folderList, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(173, 446);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // folderList
            // 
            this.folderList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.folderList.Location = new System.Drawing.Point(3, 23);
            this.folderList.Name = "folderList";
            this.folderList.Size = new System.Drawing.Size(167, 420);
            this.folderList.TabIndex = 0;
            // 
            // GlobalVariablesEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.mainFrame);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "GlobalVariablesEditor";
            this.Padding = new System.Windows.Forms.Padding(3);
            this.Size = new System.Drawing.Size(531, 519);
            this.Load += new System.EventHandler(this.GlobalVariablesEditor_Load);
            this.SizeChanged += new System.EventHandler(this.TextParserEditor_SizeChanged);
            this.mainFrame.ResumeLayout(false);
            this.mainFrame.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox mainFrame;
        private System.Windows.Forms.ListView globalVariableList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ColumnHeader clmName;
        private System.Windows.Forms.ColumnHeader clmType;
        private System.Windows.Forms.ColumnHeader clmDefaultValue;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private TreeViewWithDragDrop folderList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
    }
}
