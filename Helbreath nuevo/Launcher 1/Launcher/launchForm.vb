Imports System.IO
Imports System.Diagnostics

Public Class launchForm
    Inherits Form

    Private lblTitle, lblSubtitle, lblBefore As Label
    Private lblRes, lblAPI, lblRender, lblVSync As Label
    Private cboRes, cboAPI, cboRender, cboVSync As ComboBox
    Private btnStart As Button

    Private Sub launchForm_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        ' --- Reparar configuración corrupta antes de usar My.Settings ---
        RepairCorruptedSettings()

        ' --- Configuración del formulario ---
        Me.CenterToScreen()

        Me.Text = "Helbreath Launcher"
        Me.StartPosition = FormStartPosition.CenterScreen
        Me.FormBorderStyle = FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.BackColor = Color.FromArgb(15, 15, 15)
        Me.ClientSize = New Size(460, 520)

        ' === Títulos ===
        lblTitle = New Label() With {
            .Text = "HELBREATH",
            .Font = New Font("Segoe UI Semibold", 26, FontStyle.Bold),
            .ForeColor = Color.Gold,
            .AutoSize = False,
            .TextAlign = ContentAlignment.MiddleCenter,
            .Location = New Point(0, 35),
            .Width = Me.ClientSize.Width,
            .Height = 55
        }

        lblSubtitle = New Label() With {
            .Text = "CURSED",
            .Font = New Font("Segoe UI Semibold", 20, FontStyle.Bold),
            .ForeColor = Color.OrangeRed,
            .AutoSize = False,
            .TextAlign = ContentAlignment.MiddleCenter,
            .Location = New Point(0, 75),
            .Width = Me.ClientSize.Width,
            .Height = 45
        }

        lblBefore = New Label() With {
            .Text = "Before starting",
            .Font = New Font("Segoe UI", 11),
            .ForeColor = Color.Silver,
            .AutoSize = False,
            .TextAlign = ContentAlignment.MiddleCenter,
            .Location = New Point(0, 135),
            .Width = Me.ClientSize.Width,
            .Height = 30
        }

        ' === Controles principales ===
        Dim startY As Integer = 180
        Dim labelX As Integer = 60
        Dim comboX As Integer = 220
        Dim spacingY As Integer = 55
        Dim comboW As Integer = 180

        lblRes = MakeLabel("Resolution:", labelX, startY)
        cboRes = MakeComboBox({"640x480", "800x600", "1024x768"}, comboX, startY, comboW)

        lblAPI = MakeLabel("Graphics API:", labelX, startY + spacingY)
        cboAPI = MakeComboBox({"Automatic", "DirectX", "OpenGL", "DirectDraw", "gdi"}, comboX, startY + spacingY, comboW)

        lblRender = MakeLabel("Renderer Mode:", labelX, startY + spacingY * 2)
        cboRender = MakeComboBox({"Windowed", "Fullscreen"}, comboX, startY + spacingY * 2, comboW)

        lblVSync = MakeLabel("Vertical Sync:", labelX, startY + spacingY * 3)
        cboVSync = MakeComboBox({"Enabled", "Disabled"}, comboX, startY + spacingY * 3, comboW)

        ' === Cargar configuraciones previas o defaults ===
        LoadSettings()

        ' === Botón principal ===
        btnStart = New Button() With {
            .Text = "START GAME",
            .Font = New Font("Segoe UI Semibold", 13, FontStyle.Bold),
            .ForeColor = Color.FromArgb(255, 230, 200),
            .BackColor = Color.FromArgb(170, 40, 25),
            .FlatStyle = FlatStyle.Flat,
            .Width = 300,
            .Height = 55,
            .Location = New Point(CInt((Me.ClientSize.Width - 300) / 2), 410)
        }
        btnStart.FlatAppearance.BorderSize = 0
        btnStart.FlatAppearance.MouseOverBackColor = Color.FromArgb(200, 70, 45)
        AddHandler btnStart.Click, AddressOf btnStart_Click

        ' === Guardar configuración cuando cambie ===
        AddHandler cboRes.SelectedIndexChanged, AddressOf SaveSettings
        AddHandler cboAPI.SelectedIndexChanged, AddressOf SaveSettings
        AddHandler cboRender.SelectedIndexChanged, AddressOf SaveSettings
        AddHandler cboVSync.SelectedIndexChanged, AddressOf SaveSettings

        ' === Agregar todo ===
        Me.Controls.AddRange(New Control() {
            lblTitle, lblSubtitle, lblBefore,
            lblRes, cboRes, lblAPI, cboAPI,
            lblRender, cboRender, lblVSync, cboVSync,
            btnStart
        })
    End Sub

    ' === Reparar configuración corrupta ===
    Private Sub RepairCorruptedSettings()
        Try
            Dim configDir As String = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "Launcher")
            If Directory.Exists(configDir) Then
                For Each folder As String In Directory.GetDirectories(configDir)
                    Dim filePath As String = Path.Combine(folder, "1.0.0.0\user.config")
                    If File.Exists(filePath) Then
                        Dim content As String = File.ReadAllText(filePath)
                        If content.Contains(Chr(0)) OrElse Not content.Contains("<configuration>") Then
                            File.Delete(filePath)
                        End If
                    End If
                Next
            End If
        Catch
            ' Ignorar cualquier error y continuar
        End Try
    End Sub

    ' === Crear Labels y ComboBoxes ===
    Private Function MakeLabel(text As String, x As Integer, y As Integer) As Label
        Return New Label() With {
            .Text = text,
            .Font = New Font("Segoe UI", 10),
            .ForeColor = Color.Silver,
            .AutoSize = False,
            .TextAlign = ContentAlignment.MiddleLeft,
            .Location = New Point(x, y + 3),
            .Width = 150,
            .Height = 25
        }
    End Function

    Private Function MakeComboBox(items As String(), x As Integer, y As Integer, w As Integer) As ComboBox
        Dim c As New ComboBox()
        c.Items.AddRange(items)
        c.FlatStyle = FlatStyle.Flat
        c.BackColor = Color.FromArgb(35, 35, 35)
        c.ForeColor = Color.Silver
        c.Font = New Font("Segoe UI", 10)
        c.DropDownStyle = ComboBoxStyle.DropDownList
        c.Width = w
        c.Location = New Point(x, y)
        Return c
    End Function

    ' === Guardar / Cargar configuración ===
    Private Sub LoadSettings()
        If My.Settings.Resolution = "" Then
            My.Settings.Resolution = "1024x768"
            My.Settings.API = "Automatic"
            My.Settings.RenderMode = "Fullscreen"
            My.Settings.VSync = "Enabled"
            My.Settings.Save()
        End If

        cboRes.SelectedItem = My.Settings.Resolution
        cboAPI.SelectedItem = My.Settings.API
        cboRender.SelectedItem = My.Settings.RenderMode
        cboVSync.SelectedItem = My.Settings.VSync
    End Sub

    Private Sub SaveSettings(sender As Object, e As EventArgs)
        My.Settings.Resolution = cboRes.SelectedItem.ToString()
        My.Settings.API = cboAPI.SelectedItem.ToString()
        My.Settings.RenderMode = cboRender.SelectedItem.ToString()
        My.Settings.VSync = cboVSync.SelectedItem.ToString()
        My.Settings.Save()
    End Sub

    ' === Botón START GAME ===
    Private Sub btnStart_Click(sender As Object, e As EventArgs)
        SaveSettings(Nothing, Nothing)
        Try
            Dim exePath As String = Path.Combine(Application.StartupPath, "Helbreath Cursed.exe")
            If Not File.Exists(exePath) Then
                MessageBox.Show("No se encontró el ejecutable: " & exePath, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
                Return
            End If

            Dim resolutionArg As String = "3"
            Select Case cboRes.SelectedItem.ToString()
                Case "640x480" : resolutionArg = "1"
                Case "800x600" : resolutionArg = "2"
                Case "1024x768" : resolutionArg = "3"
            End Select

            Dim apiArg As String = "auto"
            Select Case cboAPI.SelectedItem.ToString()
                Case "Automatic" : apiArg = "auto"
                Case "DirectX" : apiArg = "dx9"
                Case "OpenGL" : apiArg = "ogl"
                Case "DirectDraw" : apiArg = "ddraw"
                Case "gdi" : apiArg = "gdi"
            End Select

            Dim renderArg As String = If(cboRender.SelectedItem.ToString() = "Fullscreen", "fullscreen", "windowed")
            Dim vsyncArg As String = If(cboVSync.SelectedItem.ToString() = "Enabled", "vsync_on", "vsync_off")

            Dim finalArgs As String = String.Format("{0} {1} {2} {3}", resolutionArg, apiArg, renderArg, vsyncArg)

            Process.Start(exePath, finalArgs)
            Application.Exit()
        Catch ex As Exception
            MessageBox.Show("Error al iniciar el juego: " & ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
    End Sub
End Class
