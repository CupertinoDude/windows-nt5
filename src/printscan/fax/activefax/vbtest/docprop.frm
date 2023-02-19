VERSION 5.00
Begin VB.Form DocProperty 
   Caption         =   "Document Properties"
   ClientHeight    =   13485
   ClientLeft      =   4650
   ClientTop       =   825
   ClientWidth     =   9375
   LinkTopic       =   "Form1"
   ScaleHeight     =   13485
   ScaleWidth      =   9375
   Begin VB.TextBox SenderAddress 
      Height          =   495
      Left            =   6480
      TabIndex        =   61
      Top             =   9000
      Width           =   2415
   End
   Begin VB.Frame Frame1 
      Caption         =   "File Name for transmission"
      Height          =   1455
      Left            =   4800
      TabIndex        =   57
      Top             =   6960
      Width           =   4215
      Begin VB.TextBox DisplayName 
         Height          =   375
         Left            =   240
         TabIndex        =   59
         Text            =   "Display Name"
         Top             =   960
         Width           =   3615
      End
      Begin VB.TextBox FileName 
         Height          =   375
         Left            =   240
         TabIndex        =   58
         Top             =   360
         Width           =   3615
      End
   End
   Begin VB.TextBox CoverpageNote 
      Height          =   495
      Left            =   4080
      TabIndex        =   54
      Top             =   2280
      Width           =   4815
   End
   Begin VB.TextBox CoverpageSubject 
      Height          =   495
      Left            =   1200
      TabIndex        =   53
      Top             =   2280
      Width           =   1935
   End
   Begin VB.TextBox EmailAddress 
      Height          =   495
      Left            =   6480
      TabIndex        =   48
      Top             =   11880
      Width           =   2415
   End
   Begin VB.TextBox SenderOfficePhone 
      Height          =   495
      Left            =   6480
      TabIndex        =   47
      Top             =   11160
      Width           =   2415
   End
   Begin VB.TextBox SenderHomePhone 
      Height          =   495
      Left            =   6480
      TabIndex        =   46
      Top             =   10440
      Width           =   2415
   End
   Begin VB.TextBox SenderOffice 
      Height          =   495
      Left            =   6480
      TabIndex        =   45
      Top             =   9720
      Width           =   2415
   End
   Begin VB.CommandButton Cancel 
      Caption         =   "Cancel"
      Height          =   615
      Left            =   5423
      TabIndex        =   44
      Top             =   12600
      Width           =   1455
   End
   Begin VB.CommandButton OK 
      Caption         =   "OK"
      Height          =   615
      Left            =   3263
      TabIndex        =   43
      Top             =   12600
      Width           =   1575
   End
   Begin VB.TextBox SenderTitle 
      Height          =   495
      Left            =   1800
      TabIndex        =   37
      Top             =   11880
      Width           =   2535
   End
   Begin VB.TextBox SenderCompany 
      Height          =   495
      Left            =   1800
      TabIndex        =   36
      Top             =   11160
      Width           =   2535
   End
   Begin VB.TextBox SenderDepartment 
      Height          =   495
      Left            =   1800
      TabIndex        =   35
      Top             =   10440
      Width           =   2535
   End
   Begin VB.TextBox SenderNumber 
      Height          =   495
      Left            =   1800
      TabIndex        =   34
      Top             =   9720
      Width           =   2535
   End
   Begin VB.TextBox SenderName 
      Height          =   495
      Left            =   1800
      TabIndex        =   33
      Top             =   9000
      Width           =   2535
   End
   Begin VB.TextBox RecipientCountry 
      Height          =   495
      Left            =   6480
      TabIndex        =   27
      Top             =   6120
      Width           =   2415
   End
   Begin VB.TextBox RecipientZip 
      Height          =   495
      Left            =   6480
      TabIndex        =   26
      Top             =   5400
      Width           =   2415
   End
   Begin VB.TextBox RecipientState 
      Height          =   495
      Left            =   6480
      TabIndex        =   25
      Top             =   4680
      Width           =   2415
   End
   Begin VB.TextBox RecipientCity 
      Height          =   495
      Left            =   6480
      TabIndex        =   24
      Top             =   3960
      Width           =   2415
   End
   Begin VB.TextBox RecipientAddress 
      Height          =   495
      Left            =   6480
      TabIndex        =   23
      Top             =   3240
      Width           =   2415
   End
   Begin VB.TextBox RecipientOffice 
      Height          =   495
      Left            =   1800
      TabIndex        =   15
      Top             =   6120
      Width           =   2535
   End
   Begin VB.TextBox RecipientOfficePhone 
      Height          =   495
      Left            =   1800
      TabIndex        =   14
      Top             =   7680
      Width           =   2535
   End
   Begin VB.TextBox RecipientHomePhone 
      Height          =   495
      Left            =   1800
      TabIndex        =   13
      Top             =   6840
      Width           =   2535
   End
   Begin VB.TextBox RecipientTitle 
      Height          =   495
      Left            =   1800
      TabIndex        =   12
      Top             =   5400
      Width           =   2535
   End
   Begin VB.TextBox RecipientCompany 
      Height          =   495
      Left            =   1800
      TabIndex        =   11
      Top             =   4680
      Width           =   2535
   End
   Begin VB.TextBox RecipientDepartment 
      Height          =   495
      Left            =   1800
      TabIndex        =   10
      Top             =   3960
      Width           =   2535
   End
   Begin VB.TextBox RecipientName 
      Height          =   495
      Left            =   1800
      TabIndex        =   9
      Top             =   3240
      Width           =   2535
   End
   Begin VB.TextBox BillingCode 
      Height          =   495
      Left            =   6720
      TabIndex        =   7
      Top             =   1320
      Width           =   2175
   End
   Begin VB.OptionButton SendNow 
      Caption         =   "Send Immediately"
      Height          =   615
      Left            =   6960
      TabIndex        =   6
      Top             =   240
      Width           =   1695
   End
   Begin VB.OptionButton DiscountRate 
      Caption         =   "Discount Rate"
      Height          =   615
      Left            =   5400
      TabIndex        =   5
      Top             =   240
      Width           =   1455
   End
   Begin VB.TextBox FaxNumber 
      Height          =   495
      Left            =   1800
      TabIndex        =   3
      Top             =   240
      Width           =   3015
   End
   Begin VB.Frame coverpage 
      Caption         =   "Coverpage"
      Height          =   1095
      Left            =   120
      TabIndex        =   0
      Top             =   960
      Width           =   5055
      Begin VB.TextBox CoverpageName 
         Height          =   495
         Left            =   1680
         TabIndex        =   2
         Top             =   360
         Width           =   3015
      End
      Begin VB.CheckBox SendCoverpage 
         Caption         =   "Cover Page?"
         Height          =   375
         Left            =   240
         TabIndex        =   1
         Top             =   480
         Width           =   1335
      End
   End
   Begin VB.Label Label26 
      Caption         =   "Email Address :"
      Height          =   375
      Left            =   5040
      TabIndex        =   60
      Top             =   12000
      Width           =   1215
   End
   Begin VB.Label Label25 
      Caption         =   "Note :"
      Height          =   255
      Left            =   3360
      TabIndex        =   56
      Top             =   2400
      Width           =   615
   End
   Begin VB.Label Label24 
      Caption         =   "Subject :"
      Height          =   375
      Left            =   240
      TabIndex        =   55
      Top             =   2400
      Width           =   855
   End
   Begin VB.Label Label23 
      Caption         =   "Sender Office Phone:"
      Height          =   495
      Left            =   5040
      TabIndex        =   52
      Top             =   11160
      Width           =   1215
   End
   Begin VB.Label Label22 
      Caption         =   "Sender Home Phone  :"
      Height          =   375
      Left            =   5040
      TabIndex        =   51
      Top             =   10440
      Width           =   1335
   End
   Begin VB.Label Label21 
      Caption         =   "Sender Office :"
      Height          =   255
      Left            =   5040
      TabIndex        =   50
      Top             =   9840
      Width           =   1335
   End
   Begin VB.Label Label20 
      Caption         =   "Sender Address :"
      Height          =   375
      Left            =   5040
      TabIndex        =   49
      Top             =   9240
      Width           =   1455
   End
   Begin VB.Label Label19 
      Caption         =   "Sender Title :"
      Height          =   255
      Left            =   240
      TabIndex        =   42
      Top             =   12000
      Width           =   1335
   End
   Begin VB.Label Label18 
      Caption         =   "Sender Company :"
      Height          =   375
      Left            =   240
      TabIndex        =   41
      Top             =   11280
      Width           =   1455
   End
   Begin VB.Label Label17 
      Caption         =   "Sender Department :"
      Height          =   495
      Left            =   240
      TabIndex        =   40
      Top             =   10560
      Width           =   1335
   End
   Begin VB.Label Label16 
      Caption         =   "Sender Fax Number :"
      Height          =   375
      Left            =   240
      TabIndex        =   39
      Top             =   9840
      Width           =   1335
   End
   Begin VB.Label Label15 
      Caption         =   "Sender Name :"
      Height          =   255
      Left            =   240
      TabIndex        =   38
      Top             =   9120
      Width           =   1455
   End
   Begin VB.Line Line3 
      BorderWidth     =   5
      X1              =   240
      X2              =   9000
      Y1              =   3000
      Y2              =   3000
   End
   Begin VB.Line Line2 
      BorderWidth     =   5
      X1              =   0
      X2              =   9120
      Y1              =   0
      Y2              =   0
   End
   Begin VB.Line Line1 
      BorderWidth     =   5
      X1              =   360
      X2              =   9000
      Y1              =   8640
      Y2              =   8640
   End
   Begin VB.Label Label14 
      Caption         =   "Recipient Country :"
      Height          =   255
      Left            =   4920
      TabIndex        =   32
      Top             =   6240
      Width           =   1455
   End
   Begin VB.Label Label13 
      Caption         =   "Recipient Zip Code :"
      Height          =   375
      Left            =   4920
      TabIndex        =   31
      Top             =   5520
      Width           =   1455
   End
   Begin VB.Label Label12 
      Caption         =   "Recipient State :"
      Height          =   255
      Left            =   4920
      TabIndex        =   30
      Top             =   4800
      Width           =   1215
   End
   Begin VB.Label Label11 
      Caption         =   "Recipient City :"
      Height          =   255
      Left            =   4920
      TabIndex        =   29
      Top             =   4080
      Width           =   1455
   End
   Begin VB.Label Label10 
      Caption         =   "Recipient Address :"
      Height          =   255
      Left            =   4920
      TabIndex        =   28
      Top             =   3360
      Width           =   1575
   End
   Begin VB.Label Label9 
      Caption         =   "Recipient Office Phone :"
      Height          =   495
      Left            =   240
      TabIndex        =   22
      Top             =   7800
      Width           =   1575
   End
   Begin VB.Label Label8 
      Caption         =   "Recipient Home Phone :"
      Height          =   495
      Left            =   240
      TabIndex        =   21
      Top             =   6960
      Width           =   1335
   End
   Begin VB.Label Label7 
      Caption         =   "Recipient Office :"
      Height          =   375
      Left            =   240
      TabIndex        =   20
      Top             =   6240
      Width           =   1575
   End
   Begin VB.Label Label6 
      Caption         =   "Recipient Title: "
      Height          =   255
      Left            =   240
      TabIndex        =   19
      Top             =   5520
      Width           =   1335
   End
   Begin VB.Label Label5 
      Caption         =   "Recipient Company:"
      Height          =   375
      Left            =   240
      TabIndex        =   18
      Top             =   4800
      Width           =   1455
   End
   Begin VB.Label Label4 
      Caption         =   "Recipient Dept:"
      Height          =   255
      Left            =   240
      TabIndex        =   17
      Top             =   4080
      Width           =   1215
   End
   Begin VB.Label Label3 
      Caption         =   "Recipient Name :"
      Height          =   375
      Left            =   240
      TabIndex        =   16
      Top             =   3360
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "Billing Code :"
      Height          =   375
      Left            =   5520
      TabIndex        =   8
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "Fax Number"
      Height          =   375
      Left            =   360
      TabIndex        =   4
      Top             =   360
      Width           =   1335
   End
End
Attribute VB_Name = "DocProperty"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Cancel_Click()
    Unload DocProperty
End Sub

Private Sub Form_Load()
    FileName.Text = FaxDocument.FileName
    DisplayName.Text = FaxDocument.DisplayName
    
    FaxNumber.Text = FaxDocument.FaxNumber
    CoverpageName.Text = FaxDocument.CoverpageName
    BillingCode.Text = FaxDocument.BillingCode
    
    If (FaxDocument.DiscountSend <> False) Then
        DiscountRate.Value = True
        SendNow.Value = False
    Else
        DiscountRate.Value = False
        SendNow.Value = True
    End If
    
    If (FaxDocument.SendCoverpage <> False) Then
        SendCoverpage.Value = 1
    Else
        SendCoverpage.Value = 0
    End If
        
    EmailAddress.Text = FaxDocument.EmailAddress
    
    RecipientName.Text = FaxDocument.RecipientName
    RecipientCompany.Text = FaxDocument.RecipientCompany
    RecipientAddress.Text = FaxDocument.RecipientAddress
    RecipientCity.Text = FaxDocument.RecipientCity
    RecipientState.Text = FaxDocument.RecipientState
    RecipientCountry.Text = FaxDocument.RecipientCountry
    RecipientTitle.Text = FaxDocument.RecipientTitle
    RecipientZip.Text = FaxDocument.RecipientZip
    RecipientDepartment.Text = FaxDocument.RecipientDepartment
    RecipientOffice.Text = FaxDocument.RecipientOffice
    RecipientHomePhone.Text = FaxDocument.RecipientHomePhone
    RecipientOfficePhone.Text = FaxDocument.RecipientOfficePhone
    
    SenderName.Text = FaxDocument.SenderName
    SenderCompany.Text = FaxDocument.SenderCompany
    SenderAddress.Text = FaxDocument.SenderAddress
    SenderTitle.Text = FaxDocument.SenderTitle
    SenderDepartment.Text = FaxDocument.SenderDepartment
    SenderOffice.Text = FaxDocument.SenderOffice
    SenderHomePhone.Text = FaxDocument.SenderHomePhone
    SenderOfficePhone.Text = FaxDocument.SenderOfficePhone
    SenderNumber.Text = FaxDocument.Tsid
    
    CoverpageNote.Text = FaxDocument.CoverpageNote
    CoverpageSubject.Text = FaxDocument.CoverpageSubject
    
    
End Sub

Private Sub OK_Click()
    FaxDocument.FileName = FileName.Text
    FaxDocument.DisplayName = DisplayName.Text
    FaxDocument.SendCoverpage = SendCoverpage.Value
    FaxDocument.FaxNumber = FaxNumber.Text
    FaxDocument.CoverpageName = CoverpageName.Text
    FaxDocument.BillingCode = BillingCode.Text
    FaxDocument.DiscountSend = DiscountRate.Value
    FaxDocument.EmailAddress = EmailAddress.Text
    
    FaxDocument.RecipientName = RecipientName.Text
    FaxDocument.RecipientCompany = RecipientCompany.Text
    FaxDocument.RecipientAddress = RecipientAddress.Text
    FaxDocument.RecipientCity = RecipientCity.Text
    FaxDocument.RecipientState = RecipientState.Text
    FaxDocument.RecipientCountry = RecipientCountry.Text
    FaxDocument.RecipientTitle = RecipientTitle.Text
    FaxDocument.RecipientZip = RecipientZip.Text
    FaxDocument.RecipientDepartment = RecipientDepartment.Text
    FaxDocument.RecipientOffice = RecipientOffice.Text
    FaxDocument.RecipientHomePhone = RecipientHomePhone.Text
    FaxDocument.RecipientOfficePhone = RecipientOfficePhone.Text
    
    FaxDocument.SenderName = SenderName.Text
    FaxDocument.SenderCompany = SenderCompany.Text
    FaxDocument.SenderAddress = SenderAddress.Text
    FaxDocument.SenderTitle = SenderTitle.Text
    FaxDocument.SenderDepartment = SenderDepartment.Text
    FaxDocument.SenderOffice = SenderOffice.Text
    FaxDocument.SenderHomePhone = SenderHomePhone.Text
    FaxDocument.SenderOfficePhone = SenderOfficePhone.Text
    FaxDocument.Tsid = SenderNumber.Text
    
    FaxDocument.CoverpageNote = CoverpageNote.Text
    FaxDocument.CoverpageSubject = CoverpageSubject.Text
    
    Unload DocProperty
End Sub
