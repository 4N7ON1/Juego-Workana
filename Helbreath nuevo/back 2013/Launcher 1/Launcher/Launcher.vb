' vb.net Helbreath Launcher coded by prlucas

Imports System.ComponentModel
Imports System.IO
Imports System.Net
Imports SharpCompress.Archive
Imports SharpCompress.Common

Public Class Launcher

    Dim drag As Boolean
    Dim mousex As Integer
    Dim mousey As Integer
    Dim WithEvents WC As New WebClient
    Dim versionfile As String = "contents/version.txt"
    Dim Update1 As String = "NewUpdate.zip"
    Dim Update2 As String = "FullRebelion.zip"
    Dim versionurl As String = "http://www.launcher.hbcursed.com/version.txt"
    Dim FileDownload As String = "http://www.launcher.hbcursed.com/NewUpdate.zip"
    Dim FileFullDownload As String = "https://www./FullRebelion.zip"
    Dim fileName1 As String = "Helbreath Cursed.exe"
    Dim fileName2 As String = "\Helbreath Cursed.exe"
    Dim tmpfilename As String = ""
    Dim downloadmode As String = "Files"

    Private Sub Launcher_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Me.CenterToScreen()
        PictureBox1.Enabled = False
        PictureBox1_MouseLeave(sender, e)

        ' === BOTÓN CERRAR ===
        Dim closeButton As New Button()
        closeButton.Text = "X"
        closeButton.Font = New Font("Arial", 8, FontStyle.Bold)
        closeButton.Size = New Size(22, 22)
        closeButton.Location = New Point(Me.Width - closeButton.Width - 5, 5)
        closeButton.Anchor = AnchorStyles.Top Or AnchorStyles.Right
        closeButton.FlatStyle = FlatStyle.Flat
        closeButton.FlatAppearance.BorderSize = 0
        closeButton.BackColor = Color.DarkRed
        closeButton.ForeColor = Color.White
        AddHandler closeButton.Click, Sub() Me.Close()
        Me.Controls.Add(closeButton)

        ' === BOTÓN OPCIONES (TUERCA) ===
        Dim settingsButton As New Button()
        settingsButton.Text = "🛠"
        settingsButton.Font = New Font("Segoe UI Emoji", 10, FontStyle.Bold)
        settingsButton.Size = New Size(22, 22)
        settingsButton.Location = New Point(Me.Width - closeButton.Width - settingsButton.Width - 10, 5)
        settingsButton.Anchor = AnchorStyles.Top Or AnchorStyles.Right
        settingsButton.FlatStyle = FlatStyle.Flat
        settingsButton.FlatAppearance.BorderSize = 0
        settingsButton.BackColor = Color.Transparent
        settingsButton.ForeColor = Color.White
        AddHandler settingsButton.Click, Sub()
                                             If File.Exists("contents/version.txt") Then File.Delete("contents/version.txt")
                                             Application.Exit()
                                         End Sub
        Me.Controls.Add(settingsButton)

        closeButton.BringToFront()
        settingsButton.BringToFront()

        ' === RESTO DEL CÓDIGO ===
        CheckForIllegalCrossThreadCalls = False

        If File.Exists("FullRebelion.zip") Then File.Delete("FullRebelion.zip")
        If File.Exists("NewUpdate.zip") Then File.Delete("NewUpdate.zip")

        If File.Exists("contents/version.txt") = False Then
            downloadmode = "Files"
            DownloadUpdate()
            Return
        End If

        If (CheckProcss()) = True Then
            EnableStartGame()
        Else
            If (CheckVersion()) = False Then
                downloadmode = "Files"
                DownloadUpdate()
            Else
                EnableStartGame()
            End If
        End If
    End Sub

    Public Sub DownloadUpdate()
        If downloadmode = "Files" Then
            BackgroundWorker1.RunWorkerAsync()
        Else
            BackgroundWorker2.RunWorkerAsync()
        End If
    End Sub

    Public Sub UpdateLauncher()
        Process.Start("UpdateLauncher.exe")
        Application.Exit()
    End Sub

    Public Sub EnableStartGame()
        Label1.Text = "Ready to play."
        ProgressBar1.Value = 100
        PictureBox1.Enabled = True
    End Sub

    Public Function CheckProcss() As Boolean
        Dim p() As Process
        p = Process.GetProcessesByName("Helbreath Cursed")
        If p.Count > 0 Then
            Return True
        Else
            Return False
        End If
    End Function

    Public Function CheckVersion() As Boolean
        Dim fileReader As String = My.Computer.FileSystem.ReadAllText(versionfile)
        Dim request As System.Net.HttpWebRequest = System.Net.HttpWebRequest.Create(versionurl)
        Dim response As System.Net.HttpWebResponse = request.GetResponse()
        Dim sr As System.IO.StreamReader = New System.IO.StreamReader(response.GetResponseStream())
        Dim newestversion As String = sr.ReadToEnd()
        Dim currentversion As String = fileReader
        If newestversion.Contains(currentversion) Then
            Return True
        Else
            Return False
        End If
        Return True
    End Function

    Private Sub PictureBox1_MouseHover(ByVal sender As Object, ByVal e As System.EventArgs) Handles PictureBox1.MouseHover
        PictureBox1.Image = New Bitmap(My.Resources.button1)
    End Sub

    Private Sub PictureBox1_MouseLeave(ByVal sender As Object, ByVal e As System.EventArgs) Handles PictureBox1.MouseLeave
        PictureBox1.Image = New Bitmap(My.Resources.button2)
    End Sub

    Private Sub PictureBox1_Click(sender As Object, e As EventArgs) Handles PictureBox1.Click
        Dim A
        A = launchForm
        A.Show()
    End Sub

    Private Sub Form1_MouseDown(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseDown
        drag = True
        mousex = Windows.Forms.Cursor.Position.X - Me.Left
        mousey = Windows.Forms.Cursor.Position.Y - Me.Top
    End Sub

    Private Sub Form1_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseMove
        If drag Then
            Me.Top = Windows.Forms.Cursor.Position.Y - mousey
            Me.Left = Windows.Forms.Cursor.Position.X - mousex
        End If
    End Sub

    Private Sub Form1_MouseUp(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles Me.MouseUp
        drag = False
    End Sub

    Private Sub Label5_Click(sender As Object, e As EventArgs) Handles Label5.Click
        If File.Exists("contents/version.txt") Then File.Delete("contents/version.txt")
        Application.Exit()
    End Sub

    Private Sub PictureBox2_Click(sender As Object, e As EventArgs)
        Application.Exit()
    End Sub

    Private Sub backgroundWorker1_DoWork(sender As Object, e As DoWorkEventArgs) Handles BackgroundWorker1.DoWork
        Label1.Text = "Downloading update..."
        ProgressBar1.Value = 0
        ProgressBar1.Maximum = 100
        WC.DownloadFileAsync(New Uri(FileDownload), Update1)
        tmpfilename = Update1
    End Sub

    Private Sub backgroundWorker2_DoWork(sender As Object, e As DoWorkEventArgs) Handles BackgroundWorker2.DoWork
        Label1.Text = "Downloading full game..."
        WC.DownloadFileAsync(New Uri(FileFullDownload), Update2)
        tmpfilename = Update2
    End Sub

    Private Sub WC_DownloadProgressChanged(ByVal sender As Object, ByVal e As DownloadProgressChangedEventArgs) Handles WC.DownloadProgressChanged
        ProgressBar1.Value = e.ProgressPercentage
    End Sub

    Private Sub WC_DownloadFileCompleted(ByVal sender As Object, ByVal e As System.ComponentModel.AsyncCompletedEventArgs) Handles WC.DownloadFileCompleted
        If downloadmode = "Full" Then
            ProgressBar1.Value = 0
            ProgressBar1.Maximum = 921
            Label1.Text = "Installing full game..."
        Else
            ProgressBar1.Value = 0
            ProgressBar1.Maximum = 100
            Label1.Text = "Installing update, please wait..."
        End If

        Dim appPath As String = AppDomain.CurrentDomain.BaseDirectory()
        Dim archive As IArchive = ArchiveFactory.Open(tmpfilename)

        For Each entry In archive.Entries
            If Not entry.IsDirectory Then
                If tmpfilename = Update1 = False Then
                    If ProgressBar1.Value < 921 Then
                        ProgressBar1.Value += 1
                    End If
                End If
                entry.WriteToDirectory(appPath, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
            End If
        Next

        If tmpfilename = Update1 Then
            EnableStartGame()
        Else
            downloadmode = "Files"
            BackgroundWorker1.RunWorkerAsync()
        End If
    End Sub

End Class
