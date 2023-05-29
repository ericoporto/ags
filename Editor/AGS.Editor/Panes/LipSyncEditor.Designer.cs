namespace AGS.Editor
{
    partial class LipSyncEditor
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
            this.labelLipSyncIntro1 = new System.Windows.Forms.Label();
            this.labelLipSyncIntro2 = new System.Windows.Forms.Label();
            this.panelLipSyncList = new System.Windows.Forms.Panel();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelLipSyncIntro1
            // 
            this.labelLipSyncIntro1.AutoSize = true;
            this.labelLipSyncIntro1.Location = new System.Drawing.Point(4, 4);
            this.labelLipSyncIntro1.Margin = new System.Windows.Forms.Padding(4);
            this.labelLipSyncIntro1.Name = "labelLipSyncIntro1";
            this.labelLipSyncIntro1.Size = new System.Drawing.Size(373, 17);
            this.labelLipSyncIntro1.TabIndex = 0;
            this.labelLipSyncIntro1.Text = "Use the property grid on the right to enable/disable lip sync.";
            // 
            // labelLipSyncIntro2
            // 
            this.labelLipSyncIntro2.AutoSize = true;
            this.labelLipSyncIntro2.Location = new System.Drawing.Point(4, 29);
            this.labelLipSyncIntro2.Margin = new System.Windows.Forms.Padding(4);
            this.labelLipSyncIntro2.MaximumSize = new System.Drawing.Size(400, 0);
            this.labelLipSyncIntro2.Name = "labelLipSyncIntro2";
            this.labelLipSyncIntro2.Size = new System.Drawing.Size(392, 34);
            this.labelLipSyncIntro2.TabIndex = 1;
            this.labelLipSyncIntro2.Text = "In the text boxes, type the letters that will cause that frame to be shown. Separ" +
    "ate multiple letters with slashes, eg.   C/D/G/K";
            // 
            // panelLipSyncList
            // 
            this.panelLipSyncList.Location = new System.Drawing.Point(3, 70);
            this.panelLipSyncList.Name = "panelLipSyncList";
            this.panelLipSyncList.Size = new System.Drawing.Size(565, 424);
            this.panelLipSyncList.TabIndex = 2;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Controls.Add(this.labelLipSyncIntro1);
            this.flowLayoutPanel1.Controls.Add(this.labelLipSyncIntro2);
            this.flowLayoutPanel1.Controls.Add(this.panelLipSyncList);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(661, 548);
            this.flowLayoutPanel1.TabIndex = 3;
            this.flowLayoutPanel1.Layout += new System.Windows.Forms.LayoutEventHandler(this.flowLayoutPanel1_Layout);
            // 
            // LipSyncEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.flowLayoutPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "LipSyncEditor";
            this.Size = new System.Drawing.Size(661, 548);
            this.Load += new System.EventHandler(this.LipSyncEditor_Load);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelLipSyncIntro1;
        private System.Windows.Forms.Label labelLipSyncIntro2;
        private System.Windows.Forms.Panel panelLipSyncList;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    }
}
