using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace AGS.Editor
{
    public partial class ProjectPanel : DockContent
    {
        public ProjectPanel()
        {
            InitializeComponent();
            Factory.GUIController.ColorThemes.Apply(LoadColorTheme);
        }

        public void LoadColorTheme(ColorTheme t)
        {
            BackColor = t.GetColor("project-panel/background");
            projectTree.BackColor = t.GetColor("project-panel/project-tree/background");
            projectTree.ForeColor = t.GetColor("project-panel/project-tree/foreground");
            projectTree.LineColor = t.GetColor("project-panel/project-tree/line");
        }

        private bool ContainsTextAtRight(string text, char sep, string search)
        {
            string[] split_txt = text?.Split(sep);
            if(split_txt.Length > 1)
            {
                string txt_to_search = split_txt[1].Trim();
                return txt_to_search == search;
            }
            return false;
        }

        private TreeNode SearchTreeView(string p_sSearchTerm, TreeNodeCollection p_Nodes)
        {
            foreach (TreeNode node in p_Nodes)
            {
                if (node.Text == p_sSearchTerm || ContainsTextAtRight(node.Text, ':', p_sSearchTerm))
                {
                    return node;
                }

                if (node.Nodes.Count > 0)
                {
                    var result = SearchTreeView(p_sSearchTerm, node.Nodes);
                    if (result != null)
                    {
                        return result;
                    }
                }
            }

            return null;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Length > 0)
            {
                TreeNode resultTreeNode = SearchTreeView(textBox1.Text, projectTree.Nodes);
                if (resultTreeNode != null)
                {
                    projectTree.SelectedNode = resultTreeNode;
                    resultTreeNode?.Parent?.Expand();
                    resultTreeNode?.Parent?.Parent?.Expand();
                    resultTreeNode.TreeView.Focus();
                }
            }
        }
    }
}
