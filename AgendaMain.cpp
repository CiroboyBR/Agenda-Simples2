/***************************************************************
 * Name:      AgendaMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ciro Meneses (ciroccuva@hotmail.com)
 * Created:   2015-10-18
 * Copyright: Ciro Meneses (www.programacaopradiversao.blogspot.com.br)
 * License:
 **************************************************************/

#include "AgendaMain.h"
#include <wx/msgdlg.h>
#include <winsock2.h>
#include <mysql.h>

#define SERVIDOR_MYSQL "127.0.0.1"
#define USUARIO_MYSQL "root"
#define SENHA_MYSQL ""
#define BD_MYSQL "agenda"
#define ATUALIZA_DADOS

MYSQL conexao;
char comando[500];
char MSG_conectado[100];
int tam = -1;

//(*InternalHeaders(AgendaFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)
void insere_tabela_pessoa(wxString nom, wxString tel);
void remove_tabela_pessoa(wxString nome) ;
void consulta_tabela();
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(AgendaFrame)
const long AgendaFrame::ID_BUTTON1 = wxNewId();
const long AgendaFrame::ID_BUTTON2 = wxNewId();
const long AgendaFrame::ID_BUTTON3 = wxNewId();
const long AgendaFrame::ID_STATICTEXT1 = wxNewId();
const long AgendaFrame::ID_TEXTCTRL1 = wxNewId();
const long AgendaFrame::ID_STATICTEXT2 = wxNewId();
const long AgendaFrame::ID_TEXTCTRL2 = wxNewId();
const long AgendaFrame::ID_LISTCTRL1 = wxNewId();
const long AgendaFrame::ID_BUTTON4 = wxNewId();
const long AgendaFrame::ID_SASHLAYOUTWINDOW1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AgendaFrame,wxFrame)
    //(*EventTable(AgendaFrame)
    //*)

END_EVENT_TABLE()

AgendaFrame::AgendaFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(AgendaFrame)
    Create(parent, wxID_ANY, _("Agenda Simples V1.2"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(640,480));
    SashLayoutWindow1 = new wxSashLayoutWindow(this, ID_SASHLAYOUTWINDOW1, wxPoint(128,192), wxSize(640,480), wxSW_3D|wxCLIP_CHILDREN, _T("ID_SASHLAYOUTWINDOW1"));
    Button1 = new wxButton(SashLayoutWindow1, ID_BUTTON1, _("Cadastrar"), wxPoint(60,20), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button2 = new wxButton(SashLayoutWindow1, ID_BUTTON2, _("Deletar"), wxPoint(140,20), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button3 = new wxButton(SashLayoutWindow1, ID_BUTTON3, _("Atualizar"), wxPoint(420,20), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticText1 = new wxStaticText(SashLayoutWindow1, ID_STATICTEXT1, _("Nome:"), wxPoint(35,60), wxSize(41,13), 0, _T("ID_STATICTEXT1"));
    TextCtrl1 = new wxTextCtrl(SashLayoutWindow1, ID_TEXTCTRL1, _("Nome"), wxPoint(80,55), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText2 = new wxStaticText(SashLayoutWindow1, ID_STATICTEXT2, _("Telefone:"), wxPoint(20,90), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl2 = new wxTextCtrl(SashLayoutWindow1, ID_TEXTCTRL2, _("(88)996112200"), wxPoint(80,90), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    ListCtrl1 = new wxListCtrl(SashLayoutWindow1, ID_LISTCTRL1, wxPoint(347,64), wxSize(260,395), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
    Button4 = new wxButton(SashLayoutWindow1, ID_BUTTON4, _("Deletar"), wxPoint(500,20), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    SashLayoutWindow1->SetSashVisible(wxSASH_TOP,    true);
    SashLayoutWindow1->SetSashVisible(wxSASH_BOTTOM, true);
    SashLayoutWindow1->SetSashVisible(wxSASH_LEFT,   true);
    SashLayoutWindow1->SetSashVisible(wxSASH_RIGHT,  true);
    SashLayoutWindow1->SetAlignment(wxLAYOUT_LEFT);
    SashLayoutWindow1->SetOrientation(wxLAYOUT_HORIZONTAL);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AgendaFrame::OnButton1Click1);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AgendaFrame::OnButton2Click);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AgendaFrame::OnButton3Click);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AgendaFrame::OnButton4Click);
    //*)
    ListCtrl1->InsertColumn(0, "Nome", wxLIST_FORMAT_LEFT, 140);
    ListCtrl1->InsertColumn(1, "Numero", wxLIST_FORMAT_LEFT, 140);
}

AgendaFrame::~AgendaFrame()
{
    //(*Destroy(AgendaFrame)
    //*)
}

void AgendaFrame::OnQuit(wxCommandEvent& event) {
    Close();
}

void AgendaFrame::OnAbout(wxCommandEvent& event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void AgendaFrame::OnButton1Click(wxCommandEvent& event) {
}

//deletar da agenda
void AgendaFrame::OnButton2Click(wxCommandEvent& event) {
  mysql_init(&conexao);

  if ( mysql_real_connect(&conexao, SERVIDOR_MYSQL, USUARIO_MYSQL, SENHA_MYSQL, BD_MYSQL, 0, NULL, 0) ) {

    //printf("MySQL client version: %s\n", mysql_get_client_info());
   // sprintf(MSG_conectado, "Conectado ao MySQL versão %s", mysql_get_client_info());
  //  wxMessageBox(MSG_conectado);

    remove_tabela_pessoa(TextCtrl1->GetValue() );
    //consulta_tabela();
   // sprintf(comando,"insert into pessoa(nome, telefone) values('%s', '%s')", nome, telefone);
  }
  else
    wxMessageBox(("\nFalha na conexão com o DB!\n"));

  mysql_close(&conexao);
}


//Cadastra o nome e telefone.
void AgendaFrame::OnButton1Click1(wxCommandEvent& event) {
  mysql_init(&conexao);

  if ( mysql_real_connect(&conexao, SERVIDOR_MYSQL, USUARIO_MYSQL, SENHA_MYSQL, BD_MYSQL, 0, NULL, 0) ) {

    //printf("MySQL client version: %s\n", mysql_get_client_info());
   // sprintf(MSG_conectado, "Conectado ao MySQL versão %s", mysql_get_client_info());
  //  wxMessageBox(MSG_conectado);

    insere_tabela_pessoa(TextCtrl1->GetValue(), TextCtrl2->GetValue() );
    //consulta_tabela();
   // sprintf(comando,"insert into pessoa(nome, telefone) values('%s', '%s')", nome, telefone);
  }
  else
    wxMessageBox(("\nFalha na conexão com o DB!\n"));

  mysql_close(&conexao);
}

void insere_tabela_pessoa(wxString nome, wxString telefone) {
  sprintf(comando,"INSERT INTO pessoa(nome, telefone) VALUES('%s', '%s')", (const char*)nome.mbc_str(), (const char*)telefone.mbc_str() );
 /* // Para diagnóstico
  printf("\n%s", (const char*)nome.mbc_str());
  printf("\n%s", (const char*)telefone.mbc_str());
  printf("\n%s", comando);
*/
  if (mysql_query(&conexao, comando) == 0 )
    printf("\nInseriu com sucesso");
  else
    printf("\nErro ao Inserir dados no DB\n");
}

void remove_tabela_pessoa(wxString nome) {
  sprintf(comando,"DELETE FROM pessoa WHERE nome='%s'", (const char*)nome.mbc_str() );
 /* // Para diagnóstico
  printf("\n%s", (const char*)nome.mbc_str());
  printf("\n%s", (const char*)telefone.mbc_str());
  printf("\n%s", comando);
*/
  if (mysql_query(&conexao, comando) == 0 ) {
    printf("\nRemoveu com sucesso");
    tam--;
  }
  else
    printf("\nErro ao Remover %s\n", (const char*)nome.mbc_str() );
}

void consulta_tabela() {
// Faz uma busca na tabela
 if (mysql_query(&conexao, "SELECT * FROM pessoa") != 0) {
      printf("Falha de cconsulta\n");
  }

  MYSQL_RES *result = mysql_store_result(&conexao);

  if (result == NULL) {
    printf("Falha no result\n");
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result))) {
    for(int i = 0; i < num_fields; i++)
      printf("%s ", row[i] ? row[i] : "NULL");

    printf("\n");
  }
  mysql_free_result(result);
}
// Botão Atualizar
void AgendaFrame::OnButton3Click(wxCommandEvent& event) {
  mysql_init(&conexao);

  system("cls"); // Só funciona no windows


  if ( mysql_real_connect(&conexao, "127.0.0.1", "root", "", "agenda", 0, NULL, 0) ) {
    // Faz uma busca na tabela
    if (mysql_query(&conexao, "SELECT * FROM pessoa") != 0)
      printf("Falha de cconsulta\n");


    MYSQL_RES *result = mysql_store_result(&conexao);

    if (result == NULL)
      printf("Falha no result\n");


    int num_fields = mysql_num_fields(result);


    ListCtrl1->DeleteAllItems ();
    wxListItem* item = new wxListItem();
    item->SetBackgroundColour(*wxRED);
    item->SetText(wxT("Programmer"));
    item->SetId(0);


    MYSQL_ROW row;
    tam = -1;
    while ((row = mysql_fetch_row(result))) {
      for(int i = 0; i < num_fields; i++) {
        printf("\nField: %d - ", i);
        printf("%s ", row[i] ? row[i] : "NULL");

        if (i == 0) {
          ListCtrl1->InsertItem(0, *item);
        }
        if (i != 0)
          if (strcmp(row[i], "NULL" ) != 0 )
            if (i == 1)
              ListCtrl1->SetItem(0, 0, wxString::FromUTF8(row[i]), -1);
            if (i == 2)
              ListCtrl1->SetItem(0, 1, wxString::FromUTF8(row[i]), -1);

      }

      printf("\n");
      tam ++;
    }
    mysql_free_result(result);
  }
  mysql_close(&conexao);
}

void AgendaFrame::OnButton4Click(wxCommandEvent& event) {
  int i;

  mysql_init(&conexao);

  printf("\n%d", ListCtrl1->GetItemState(0, -1));
  printf("\nTam: %d", tam);
  if ( mysql_real_connect(&conexao, SERVIDOR_MYSQL, USUARIO_MYSQL, SENHA_MYSQL, BD_MYSQL, 0, NULL, 0) ) {
  if (tam > -1)
    for (i = 0; i < tam+1; i++)
      if ( (ListCtrl1->GetItemState(i, -1)) != 0) {
        remove_tabela_pessoa(ListCtrl1->GetItemText(i, 0) );
        ListCtrl1->DeleteItem(i);
      }
  }
  else
    wxMessageBox(("\nFalha na conexão com o DB!\n"));

  mysql_close(&conexao);
}
