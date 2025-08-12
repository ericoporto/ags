namespace AGS.Editor
{
    partial class ProjectPanel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProjectPanel));
            this.btnElipsis = new System.Windows.Forms.Button();
            this.projectTree = new AGS.Editor.TreeViewWithDragDrop();
            this.SuspendLayout();
            // 
            // btnElipsis
            // 
            this.btnElipsis.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnElipsis.Location = new System.Drawing.Point(242, 12);
            this.btnElipsis.Name = "btnElipsis";
            this.btnElipsis.Size = new System.Drawing.Size(38, 25);
            this.btnElipsis.TabIndex = 3;
            this.btnElipsis.Text = "...";
            this.btnElipsis.UseVisualStyleBackColor = true;
            this.btnElipsis.Visible = false;
            // 
            // projectTree
            // 
            this.projectTree.AllowDrop = true;
            this.projectTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.projectTree.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.projectTree.HideSelection = false;
            this.projectTree.LabelEdit = true;
            this.projectTree.Location = new System.Drawing.Point(0, 0);
            this.projectTree.Name = "projectTree";
            this.projectTree.Size = new System.Drawing.Size(292, 260);
            this.projectTree.TabIndex = 2;
            // 
            // ProjectPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 260);
            this.Controls.Add(this.btnElipsis);
            this.Controls.Add(this.projectTree);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ProjectPanel";
            this.Load += new System.EventHandler(this.ProjectPanel_Load);
            this.ResumeLayout(false);

        }

        #endregion

        internal AGS.Editor.TreeViewWithDragDrop projectTree;
        public System.Windows.Forms.Button btnElipsis;
    }
}
