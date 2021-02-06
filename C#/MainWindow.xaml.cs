using System;
using System.Windows;
using System.Windows.Controls;


namespace TreeView
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        private void AddRootNode_Click(object sender, RoutedEventArgs e)
        {
            var item = new TreeViewItem();
            item.Header = txtNodeName.Text;
            treeView1.Items.Add(item);
        }
        private void AddSonNode_Click(object sender, RoutedEventArgs e)
        {
            var subItem = new TreeViewItem();
            subItem.Header = txtNodeName.Text;
            var parent = treeView1.SelectedItem as TreeViewItem;  // Checking for selected element
            parent.Items.Add(subItem);
        }
        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            var item = treeView1.SelectedItem as TreeViewItem;
            var parent = item.Parent as TreeViewItem;
            if (parent != null)
            {
                parent.Items.Remove(item);
            }
            else
                treeView1.Items.RemoveAt(treeView1.Items.IndexOf(treeView1.SelectedItem));
        }
        private void Esc_Click(object sender, EventArgs e)
        {
            Application.Current.MainWindow.Close();
        }
    }
}
