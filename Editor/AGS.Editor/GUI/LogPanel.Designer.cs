
namespace AGS.Editor
{
    partial class LogPanel
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
            this.components = new System.ComponentModel.Container();
            this.logTextBox = new System.Windows.Forms.RichTextBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.comboBox_Script_LogLevel = new System.Windows.Forms.ComboBox();
            this.label_script_loglevel_desc = new System.Windows.Forms.Label();
            this.label_game_loglevel_desc = new System.Windows.Forms.Label();
            this.comboBox_Game_LogLevel = new System.Windows.Forms.ComboBox();
            this.timerLogBufferSync = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // logTextBox
            // 
            this.logTextBox.BackColor = System.Drawing.Color.Black;
            this.logTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logTextBox.ForeColor = System.Drawing.Color.White;
            this.logTextBox.Location = new System.Drawing.Point(0, 0);
            this.logTextBox.Name = "logTextBox";
            this.logTextBox.ReadOnly = true;
            this.logTextBox.Size = new System.Drawing.Size(800, 398);
            this.logTextBox.TabIndex = 0;
            this.logTextBox.Text = "";
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
            this.splitContainer1.Panel1.Controls.Add(this.comboBox_Script_LogLevel);
            this.splitContainer1.Panel1.Controls.Add(this.label_script_loglevel_desc);
            this.splitContainer1.Panel1.Controls.Add(this.label_game_loglevel_desc);
            this.splitContainer1.Panel1.Controls.Add(this.comboBox_Game_LogLevel);
            this.splitContainer1.Panel1MinSize = 48;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.logTextBox);
            this.splitContainer1.Size = new System.Drawing.Size(800, 450);
            this.splitContainer1.SplitterDistance = 48;
            this.splitContainer1.TabIndex = 1;
            // 
            // comboBox_Script_LogLevel
            // 
            this.comboBox_Script_LogLevel.FormattingEnabled = true;
            this.comboBox_Script_LogLevel.Items.AddRange(new object[] {
            "None",
            "Alert",
            "Fatal",
            "Error",
            "Warn",
            "Info",
            "Debug"});
            this.comboBox_Script_LogLevel.Location = new System.Drawing.Point(492, 22);
            this.comboBox_Script_LogLevel.Name = "comboBox_Script_LogLevel";
            this.comboBox_Script_LogLevel.Size = new System.Drawing.Size(121, 24);
            this.comboBox_Script_LogLevel.TabIndex = 5;
            this.comboBox_Script_LogLevel.SelectionChangeCommitted += new System.EventHandler(this.comboBox_Script_LogLevel_SelectionChangeCommitted);
            // 
            // label_script_loglevel_desc
            // 
            this.label_script_loglevel_desc.AutoSize = true;
            this.label_script_loglevel_desc.ForeColor = System.Drawing.Color.Black;
            this.label_script_loglevel_desc.Location = new System.Drawing.Point(321, 29);
            this.label_script_loglevel_desc.Name = "label_script_loglevel_desc";
            this.label_script_loglevel_desc.Size = new System.Drawing.Size(165, 17);
            this.label_script_loglevel_desc.TabIndex = 4;
            this.label_script_loglevel_desc.Text = "Script Minimal Log Level:";
            // 
            // label_game_loglevel_desc
            // 
            this.label_game_loglevel_desc.AutoSize = true;
            this.label_game_loglevel_desc.ForeColor = System.Drawing.Color.Black;
            this.label_game_loglevel_desc.Location = new System.Drawing.Point(12, 29);
            this.label_game_loglevel_desc.Name = "label_game_loglevel_desc";
            this.label_game_loglevel_desc.Size = new System.Drawing.Size(167, 17);
            this.label_game_loglevel_desc.TabIndex = 3;
            this.label_game_loglevel_desc.Text = "Game Minimal Log Level:";
            // 
            // comboBox_Game_LogLevel
            // 
            this.comboBox_Game_LogLevel.FormattingEnabled = true;
            this.comboBox_Game_LogLevel.Items.AddRange(new object[] {
            "None",
            "Alert",
            "Fatal",
            "Error",
            "Warn",
            "Info",
            "Debug"});
            this.comboBox_Game_LogLevel.Location = new System.Drawing.Point(185, 22);
            this.comboBox_Game_LogLevel.Name = "comboBox_Game_LogLevel";
            this.comboBox_Game_LogLevel.Size = new System.Drawing.Size(121, 24);
            this.comboBox_Game_LogLevel.TabIndex = 1;
            this.comboBox_Game_LogLevel.SelectionChangeCommitted += new System.EventHandler(this.comboBox_Game_LogLevel_SelectionChangeCommitted);
            // 
            // timerLogBufferSync
            // 
            this.timerLogBufferSync.Tick += new System.EventHandler(this.timerLogBufferSync_Tick);
            // 
            // LogPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.splitContainer1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "LogPanel";
            this.Text = "LogPanel";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox logTextBox;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ComboBox comboBox_Game_LogLevel;
        private System.Windows.Forms.Label label_game_loglevel_desc;
        private System.Windows.Forms.Timer timerLogBufferSync;
        private System.Windows.Forms.ComboBox comboBox_Script_LogLevel;
        private System.Windows.Forms.Label label_script_loglevel_desc;
    }
}