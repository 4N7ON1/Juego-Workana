Imports System.ComponentModel
Imports System.IO
Imports System.Net
Imports SharpCompress.Archive
Imports SharpCompress.Common

Public Class Form1

    Dim WithEvents WC As New WebClient
    Dim ZipName As String = "NewLauncher.zip"
    Dim DownloadUrl As String = "http://www.launcher.hbcursed.com/NewLauncher.zip"
    Dim ExtractPath As String = AppDomain.CurrentDomain.BaseDirectory

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Me.CenterToScreen()
        CheckForIllegalCrossThreadCalls = False
        Label1.Text = "Actualizando launcher, por favor espere..."
        ProgressBar1.Value = 0

        Try
            ' Cerrar cualquier launcher abierto
            Dim procs() As Process = Process.GetProcessesByName("Launcher")
            For Each p In procs
                p.Kill()
                p.WaitForExit()
            Next
        Catch ex As Exception
            ' Ignorar si no existe o no se puede cerrar
        End Try

        ' --- Eliminar archivos viejos si existen ---
        Dim filesToDelete() As String = {
            "ddraw.zip",
            "ddraw_dx9.zip",
            "ddraw_dx11.zip",
            "ddraw_opengl.zip",
            "ddraw.dll",
            "ddraw.ini"
        }

        For Each f In filesToDelete
            Dim fullPath As String = Path.Combine(ExtractPath, f)
            Try
                If File.Exists(fullPath) Then
                    File.Delete(fullPath)
                End If
            Catch ex As Exception
                ' Si no se puede borrar, ignorar
            End Try
        Next

        ' --- Si ya existe una versión vieja del ZIP, eliminarla ---
        If File.Exists(Path.Combine(ExtractPath, ZipName)) Then
            Try
                File.Delete(Path.Combine(ExtractPath, ZipName))
            Catch
            End Try
        End If

        ' --- Iniciar descarga ---
        WC.DownloadFileAsync(New Uri(DownloadUrl), Path.Combine(ExtractPath, ZipName))
    End Sub

    Private Sub WC_DownloadProgressChanged(sender As Object, e As DownloadProgressChangedEventArgs) Handles WC.DownloadProgressChanged
        ProgressBar1.Value = e.ProgressPercentage
        Label1.Text = "Descargando actualización... " & e.ProgressPercentage & "%"
    End Sub

    Private Sub WC_DownloadFileCompleted(sender As Object, e As AsyncCompletedEventArgs) Handles WC.DownloadFileCompleted
        Try
            Label1.Text = "Extrayendo archivos..."
            ProgressBar1.Value = 0

            Dim zipPath As String = Path.Combine(ExtractPath, ZipName)
            Dim archive As IArchive = ArchiveFactory.Open(zipPath)
            Dim entries = archive.Entries.Where(Function(entry) Not entry.IsDirectory).ToList()
            Dim total As Integer = entries.Count
            Dim current As Integer = 0

            For Each entry In entries
                current += 1
                entry.WriteToDirectory(ExtractPath, ExtractOptions.ExtractFullPath Or ExtractOptions.Overwrite)
                ProgressBar1.Value = CInt((current / total) * 100)
                Label1.Text = "Instalando archivos... " & ProgressBar1.Value & "%"
                Application.DoEvents()
            Next

            archive.Dispose()

            ' --- Eliminar el ZIP después de extraer ---
            Try
                If File.Exists("contents/version.txt") Then File.Delete("contents/version.txt")
            Catch
            End Try

            ' --- Eliminar el ZIP después de extraer ---
            Try
                File.Delete(zipPath)
            Catch
            End Try

            Label1.Text = "Actualización completada."
            ProgressBar1.Value = 100

            Application.Exit()

        Catch ex As Exception
            Application.Exit()
        End Try
    End Sub

End Class
