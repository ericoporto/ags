
namespace AGS.Editor
{
    partial class ConsolePanel
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.richTextConsole = new System.Windows.Forms.RichTextBox();
            this.button_console_clear = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.splitContainer1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(711, 419);
            this.panel1.TabIndex = 0;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.button_console_clear);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.richTextConsole);
            this.splitContainer1.Size = new System.Drawing.Size(711, 419);
            this.splitContainer1.SplitterDistance = 28;
            this.splitContainer1.TabIndex = 0;
            // 
            // richTextConsole
            // 
            this.richTextConsole.AcceptsTab = true;
            this.richTextConsole.BackColor = System.Drawing.Color.Black;
            this.richTextConsole.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextConsole.Font = new System.Drawing.Font("Consolas", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.richTextConsole.ForeColor = System.Drawing.Color.White;
            this.richTextConsole.Location = new System.Drawing.Point(0, 0);
            this.richTextConsole.Name = "richTextConsole";
            this.richTextConsole.ReadOnly = true;
            this.richTextConsole.Size = new System.Drawing.Size(711, 387);
            this.richTextConsole.TabIndex = 0;
            this.richTextConsole.Text = "";
            // 
            // button_console_clear
            // 
            this.button_console_clear.AutoSize = true;
            this.button_console_clear.Dock = System.Windows.Forms.DockStyle.Left;
            this.button_console_clear.Location = new System.Drawing.Point(0, 0);
            this.button_console_clear.Name = "button_console_clear";
            this.button_console_clear.Size = new System.Drawing.Size(75, 28);
            this.button_console_clear.TabIndex = 0;
            this.button_console_clear.Text = "Clear";
            this.button_console_clear.UseVisualStyleBackColor = true;
            // 
            // ConsolePane
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "ConsolePane";
            this.Size = new System.Drawing.Size(711, 419);
            this.panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button button_console_clear;
        private System.Windows.Forms.RichTextBox richTextConsole;
    }
}
