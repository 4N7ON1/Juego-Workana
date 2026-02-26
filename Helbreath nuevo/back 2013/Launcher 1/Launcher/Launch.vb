Imports System.IO
Imports System.Net
Imports SharpCompress.Archive
Imports SharpCompress.Common

Public Class Launch
    Dim argument1 As String = "1"
    Dim argument2 As String = "1"
    Dim argument3 As String = "1"
    Dim optvideo As String = "DirectDraw"
    Dim drag As Boolean
    Dim mousex As Integer
    Dim mousey As Integer
    Dim WithEvents WC As New WebClient
    Dim versionfile As String = "version.txt"
    Dim Update1 As String = "Update.zip"
    Dim w10fix As String = "ddraw.zip"
    Dim versionurl As String = "http://www.launcher.hbcursed.com/version.txt"
    Dim FileDownload As String = "http://www.launcher.hbcursed.com/Update.zip"
    Dim fileName1 As String = "Helbreath Cursed.exe"
    Dim fileName2 As String = "\Helbreath Cursed.exe"

    Private Sub Launch_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Me.CenterToParent()
        ' ComboBox1.SelectedIndex = ComboBox1.FindStringExact(My.Settings.resolucion)
        ComboBox2.SelectedIndex = ComboBox2.FindStringExact(My.Settings.pantalla)
        ComboBox3.SelectedIndex = ComboBox3.FindStringExact(My.Settings.video)
        Dim osversion As Version = Environment.OSVersion.Version
        ComboBox3.Enabled = True
        ComboBox3.SelectedIndex = 3

    End Sub


    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        CargarOpciones()
        AbrirJuego()
    End Sub


    Private Sub ComboBox2_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox2.SelectedIndexChanged
        My.Settings.pantalla = ComboBox2.SelectedItem.ToString()
        My.Settings.Save()
    End Sub

    Private Sub ComboBox3_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ComboBox3.SelectedIndexChanged
        My.Settings.video = ComboBox3.SelectedItem.ToString()
        My.Settings.Save()
    End Sub

    Public Sub CargarOpciones()

        Dim pantalla As String = ComboBox2.SelectedItem.ToString()
        Dim video As String = ComboBox3.SelectedItem.ToString()

        If pantalla = "640x480" Then
            argument2 = "1"
        ElseIf pantalla = "800x600" Then
            argument2 = "2"
        ElseIf pantalla = "1024x768" Then
            argument2 = "3"
        End If

    End Sub

    Public Sub AbrirJuego()
        Dim osversion As Version = Environment.OSVersion.Version
        Dim filepath As String = fileName2
        Dim Filename
        Filename = fileName1
        Dim appPath As String = Application.StartupPath()

        If osversion.Major = 6 Then
            If osversion.Minor = 2 Then
                ConfigurarSistema()
            End If
        End If

        If osversion.Major = 6 Then
            If osversion.Minor = 3 Then
                ConfigurarSistema()
            End If
        End If

        If osversion.Major = 10 Then
            ConfigurarSistema()
        End If

        If File.Exists(fileName1) Then
            Process.Start(Filename, argument2)
            Application.Exit()
        Else
            Application.Exit()
        End If
    End Sub

    Public Sub ConfigurarSistema()
        If CheckProcss() = False Then

            Dim video As String = ComboBox3.SelectedItem.ToString()

            If video = "DirectDraw" Then
                If File.Exists("ddraw.dll") Then
                    File.Delete("ddraw.dll")
                End If
            ElseIf video = "Directx9" Then
                Extraer("ddraw_dx9.zip")
            ElseIf video = "Directx10" Then
                Extraer("ddraw.zip")
            ElseIf video = "Directx11" Then
                Extraer("ddraw_dx11.zip")
            ElseIf video = "OpenGL" Then
                Extraer("ddraw_opengl.zip")
            End If

        Else
            MsgBox("To change graphics engine options you must close all clients.")
        End If
    End Sub

    Public Sub Extraer(val As String)
        Dim appPath As String = AppDomain.CurrentDomain.BaseDirectory()
        Dim archive As IArchive

        If File.Exists(val) Then
            archive = ArchiveFactory.Open(val)
            For Each entry In archive.Entries
                If Not entry.IsDirectory Then
                    entry.WriteToDirectory(appPath, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
                End If
            Next
        End If
    End Sub

    Public Sub EscribirRegistro()
        'Dim appPath As String = AppDomain.CurrentDomain.BaseDirectory()
        'Dim filepath As String = fileName1
        'My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers",
        'appPath & filepath, "~ WIN7RTM DWM8And16BitMitigation")
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

    Private Sub Label4_Click(sender As Object, e As EventArgs) Handles Label4.Click

    End Sub
End Class