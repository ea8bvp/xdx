/*
 * xdx - GTK+ DX-cluster client for amateur radio
 * Copyright (C) 2002-2003 Joop Stakenborg <pg4i@amsat.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * gui_settingsdialog.c
 */

#include <gtk/gtk.h>
#include <string.h>
#include "gui_settingsdialog.h"
#include "utils.h"
#include "gui.h"
#include "preferences.h"

GtkWidget *preferencesdialog;
extern preferencestype preferences;

static void
on_pautologincheckbutton_toggled       (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *ploginhseparator, *pcallsignlabel, *pcallsignentry, 
    *pcommandslabel, *pcommandsentry;
  gboolean state;

  ploginhseparator = g_object_get_data (G_OBJECT (preferencesdialog), 
    "ploginhseparator");
  pcallsignlabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcallsignlabel");
  pcallsignentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcallsignentry");
  pcommandslabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandslabel");
  pcommandsentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "pcommandsentry");

  state = gtk_toggle_button_get_active (togglebutton);
  if (state)
  {
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcallsignlabel, TRUE);
    gtk_widget_set_sensitive (pcallsignentry, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcallsignlabel, FALSE);
    gtk_widget_set_sensitive (pcallsignentry, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
}


static void
on_phamlibcheckbutton_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *phamlibhseparator, *priglabel, *prigentry;
  gboolean state;

  phamlibhseparator = g_object_get_data (G_OBJECT (preferencesdialog), 
    "phamlibhseparator");
  priglabel = g_object_get_data (G_OBJECT (preferencesdialog), 
    "priglabel");
  prigentry = g_object_get_data (G_OBJECT (preferencesdialog), 
    "prigentry");

  state = gtk_toggle_button_get_active (togglebutton);
  if (state)
  {
    gtk_widget_set_sensitive (phamlibhseparator, TRUE);
    gtk_widget_set_sensitive (priglabel, TRUE);
    gtk_widget_set_sensitive (prigentry, TRUE);
  }
  else
  {
    gtk_widget_set_sensitive (phamlibhseparator, FALSE);
    gtk_widget_set_sensitive (priglabel, FALSE);
    gtk_widget_set_sensitive (prigentry, FALSE);
  }
}

static void
on_fontbutton_clicked (GtkButton *button, gpointer user_data)
{
  GtkWidget *fontselectiondialog, *pfontsdxentry, *pfontsallentry;
  GdkPixbuf *fontselectiondialog_icon_pixbuf;
  gchar *font, *path;
  gint response;

  fontselectiondialog = gtk_font_selection_dialog_new
    (_("xdx - Select a font"));
  path = g_build_filename (PACKAGE_DATA_DIR, "pixmaps", "xdx.png", NULL);
  fontselectiondialog_icon_pixbuf = gdk_pixbuf_new_from_file (path, NULL);
  g_free (path);
  if (fontselectiondialog_icon_pixbuf)
  {
    gtk_window_set_icon (GTK_WINDOW (fontselectiondialog),
      fontselectiondialog_icon_pixbuf);
    g_object_unref (fontselectiondialog_icon_pixbuf);
  }
  gtk_widget_destroy (GTK_FONT_SELECTION_DIALOG
    (fontselectiondialog)->apply_button);
  gtk_font_selection_dialog_set_preview_text (GTK_FONT_SELECTION_DIALOG
    (fontselectiondialog), _("How about this font?"));
  if (GPOINTER_TO_INT(user_data) == 1)
    gtk_font_selection_dialog_set_font_name
      (GTK_FONT_SELECTION_DIALOG(fontselectiondialog), preferences.dxfont);
  else
    gtk_font_selection_dialog_set_font_name
      (GTK_FONT_SELECTION_DIALOG(fontselectiondialog), preferences.allfont);
  gtk_widget_show_all (fontselectiondialog);
  response = gtk_dialog_run (GTK_DIALOG(fontselectiondialog));
  if (response == GTK_RESPONSE_OK)
  {
    font = gtk_font_selection_dialog_get_font_name
      (GTK_FONT_SELECTION_DIALOG (fontselectiondialog));
    if (GPOINTER_TO_INT(user_data) == 1)
    {
      pfontsdxentry = g_object_get_data (G_OBJECT (preferencesdialog), 
        "pfontsdxentry");
      gtk_entry_set_text (GTK_ENTRY (pfontsdxentry), font);
    }
	else
    {
      pfontsallentry = g_object_get_data (G_OBJECT (preferencesdialog), 
        "pfontsallentry");
      gtk_entry_set_text (GTK_ENTRY (pfontsallentry), font);
    }
    g_free (font);
  }
  gtk_widget_destroy (fontselectiondialog);
}

/*
 * called from the menu
 */
void on_settings_activate (GtkMenuItem * menuitem, gpointer user_data)
{
  GtkWidget *pdialog_vbox, *pvbox1, *pvbox2, *pvbox3,

    *pnotebook, *plabel1, *plabel2, *plabel3,

    *ploginframe, *ploginvbox, *pautologincheckbutton, *ploginhseparator, 
    *ploginhbox, *pcallsignlabel, *pcallsignentry, *pcommandshbox, 
    *pcommandslabel, *pcommandsentry, *ploginlabel,

    *psavingframe, *psavingvbox, *psavedxcheckbutton, *psavewwvcheckbutton,
    *psavetoallcheckbutton, *psavewxcheckbutton, *psavinglabel, 

    *phamlibframe, *phamlibvbox, *phamlibcheckbutton, *phamlibhseparator, 
    *phamlibhbox, *priglabel, *prigentry, *phamliblabel, 

    *pprogframe, *pprogvbox, *pproghbox1, *pprogbrowserlabel, 
    *pprogbrowserentry, *pproghbox2, *pprogmaillabel,
    *pprogmailentry, *pproglabel,

    *pcolumnsframe, *pcolumnsvbox, *pcolumnsvboxlabel, *pcolumnslabel,
    *pcolumnshseparator, *pspottercheckbutton, *pqrgcheckbutton,
    *pdxcheckbutton, *premarkscheckbutton, *ptimecheckbutton, 
    *pinfocheckbutton,

	*pfontsframe, *pfontslabel, *pfontsvbox, *pfontsdxlabel, *pfontsalllabel,
	*pfontsdxentry, *pfontsallentry, *pfontsdxbutton, *pfontsallbutton,
	*pfontshseparator, *pfontshbox1, *pfontshbox2,

	*phighframe, *phighlabel, *phighvbox;

  GtkTreeViewColumn *column;
  GtkWidget *treeview, *maintext, *mainentry;
  PangoFontDescription *font_description;
  gint response;
  gboolean state;
  gchar *str;
  GtkTooltips *tooltips;

  gtk_widget_set_sensitive (gui->window, 0);
  preferencesdialog = gtk_dialog_new_with_buttons (_("xdx - preferences"),
					    GTK_WINDOW (gui->window),
					    GTK_DIALOG_MODAL |
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OK,
					    GTK_RESPONSE_OK, NULL);

  
  pdialog_vbox = GTK_DIALOG (preferencesdialog)->vbox;

  pnotebook = gtk_notebook_new ();
  pvbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox1);
  pvbox2 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox2);
  pvbox3 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pnotebook), pvbox3);
  plabel1 = gtk_label_new (_("General"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 0), plabel1);
  plabel2 = gtk_label_new (_("Output"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 1), plabel2);
  plabel3 = gtk_label_new (_("Fonts"));
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (pnotebook), 
    gtk_notebook_get_nth_page (GTK_NOTEBOOK (pnotebook), 2), plabel3);
  gtk_box_pack_start (GTK_BOX (pdialog_vbox), pnotebook, TRUE, TRUE, 0);


  ploginframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox1), ploginframe, TRUE, TRUE, 0);
  ploginvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (ploginframe), ploginvbox);
  pautologincheckbutton = gtk_check_button_new_with_mnemonic (_("Enable autologin"));
  gtk_box_pack_start (GTK_BOX (ploginvbox), pautologincheckbutton, FALSE, FALSE, 0);
  ploginhseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhseparator, TRUE, TRUE, 0);
  ploginhbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (ploginvbox), ploginhbox, TRUE, TRUE, 0);
  pcallsignlabel = gtk_label_new (_("Callsign"));
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignlabel, FALSE, FALSE, 0);
  pcallsignentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (ploginhbox), pcallsignentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcallsignentry), 15);
  pcommandshbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (ploginvbox), pcommandshbox, TRUE, TRUE, 0);
  pcommandslabel = gtk_label_new (_("Commands"));
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandslabel, FALSE, FALSE, 0);
  pcommandsentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pcommandshbox), pcommandsentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pcommandsentry), 80);
  tooltips = gtk_tooltips_new ();
  gtk_tooltips_set_tip (tooltips, pcommandsentry, 
    _("Comma separated list of commands to send at login"), NULL);
  gtk_tooltips_set_tip (tooltips, pcallsignentry, 
    _("Callsign to be used for login"), NULL);

  ploginlabel = gtk_label_new (_("Login"));
  gtk_frame_set_label_widget (GTK_FRAME (ploginframe), ploginlabel);

  if (preferences.autologin == 1)
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), TRUE);
    gtk_widget_set_sensitive (ploginhseparator, TRUE);
    gtk_widget_set_sensitive (pcallsignlabel, TRUE);
    gtk_widget_set_sensitive (pcallsignentry, TRUE);
    gtk_widget_set_sensitive (pcommandslabel, TRUE);
    gtk_widget_set_sensitive (pcommandsentry, TRUE);
  }
  else
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pautologincheckbutton), FALSE);
    gtk_widget_set_sensitive (ploginhseparator, FALSE);
    gtk_widget_set_sensitive (pcallsignlabel, FALSE);
    gtk_widget_set_sensitive (pcallsignentry, FALSE);
    gtk_widget_set_sensitive (pcommandslabel, FALSE);
    gtk_widget_set_sensitive (pcommandsentry, FALSE);
  }
  if (g_ascii_strcasecmp (preferences.callsign, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcallsignentry), preferences.callsign);
  if (g_ascii_strcasecmp (preferences.commands, "?"))
    gtk_entry_set_text (GTK_ENTRY(pcommandsentry), preferences.commands);

  phamlibframe = gtk_frame_new (NULL);
  gtk_widget_show (phamlibframe);
  gtk_box_pack_start (GTK_BOX (pvbox1), phamlibframe, TRUE, TRUE, 0);
  phamlibvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (phamlibframe), phamlibvbox);
  phamlibcheckbutton = gtk_check_button_new_with_mnemonic (_("Enable hamlib"));
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibcheckbutton, FALSE, FALSE, 0);
  phamlibhseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhseparator, TRUE, TRUE, 0);
  phamlibhbox = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (phamlibvbox), phamlibhbox, TRUE, TRUE, 0);
  priglabel = gtk_label_new (_("Command for rigctl"));
  gtk_box_pack_start (GTK_BOX (phamlibhbox), priglabel, FALSE, FALSE, 0);
  prigentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (phamlibhbox), prigentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (prigentry), 80);
  phamliblabel = gtk_label_new (_("Hamlib"));
  gtk_frame_set_label_widget (GTK_FRAME (phamlibframe), phamliblabel);
  gtk_tooltips_set_tip (tooltips, prigentry, _(
    "When double clicking on a dx-spot this will set the frequency of your "
    "rig using rigctl (%d = the frequency retrieved from the DX spot)"
    ), NULL);

  if (preferences.hamlib == 1)
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(phamlibcheckbutton), TRUE);
    gtk_widget_set_sensitive (phamlibhseparator, TRUE);
    gtk_widget_set_sensitive (priglabel, TRUE);
    gtk_widget_set_sensitive (prigentry, TRUE);
  }
  else
  {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(phamlibcheckbutton), FALSE);
    gtk_widget_set_sensitive (phamlibhseparator, FALSE);
    gtk_widget_set_sensitive (priglabel, FALSE);
    gtk_widget_set_sensitive (prigentry, FALSE);
  }
  if (g_ascii_strcasecmp (preferences.rigctl, "?"))
    gtk_entry_set_text (GTK_ENTRY(prigentry), preferences.rigctl);
  
  pprogframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox1), pprogframe, TRUE, TRUE, 0);
  pprogvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pprogframe), pprogvbox);
  pproghbox1 = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox1, TRUE, TRUE, 0);
  pprogbrowserlabel = gtk_label_new (_("Web browser"));
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserlabel, FALSE, FALSE, 0);
  pprogbrowserentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pproghbox1), pprogbrowserentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pprogbrowserentry), 80);
  pproghbox2 = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pprogvbox), pproghbox2, TRUE, TRUE, 0);
  pprogmaillabel = gtk_label_new (_("Mail program"));
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmaillabel, FALSE, FALSE, 0);
  pprogmailentry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (pproghbox2), pprogmailentry, TRUE, TRUE, 5);
  gtk_entry_set_max_length (GTK_ENTRY (pprogmailentry), 80);
  pproglabel = gtk_label_new (_("Programs"));
  gtk_frame_set_label_widget (GTK_FRAME (pprogframe), pproglabel);
  gtk_tooltips_set_tip (tooltips, pprogbrowserentry, 
    _("Web browser to start after clicking on a url (%s = url)"), NULL);
  gtk_tooltips_set_tip (tooltips, pprogmailentry, 
    _("Mail program to start after clicking on a mail url (%s = mail url)"), NULL);

  if (g_ascii_strcasecmp (preferences.browserapp, "?"))
    gtk_entry_set_text (GTK_ENTRY(pprogbrowserentry), preferences.browserapp);
  if (g_ascii_strcasecmp (preferences.mailapp, "?"))
    gtk_entry_set_text (GTK_ENTRY(pprogmailentry), preferences.mailapp);

  pcolumnsframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox2), pcolumnsframe, TRUE, TRUE, 0);
  pcolumnsvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pcolumnsframe), pcolumnsvbox);
  pcolumnslabel = gtk_label_new (_("Columns"));
  gtk_frame_set_label_widget (GTK_FRAME (pcolumnsframe), pcolumnslabel);
  pcolumnsvboxlabel = gtk_label_new (_("Columns to show on the screen"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pcolumnsvboxlabel, FALSE, FALSE, 0);
  pcolumnshseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pcolumnshseparator, FALSE, FALSE, 0);
  pspottercheckbutton = gtk_check_button_new_with_mnemonic (_("Spotter"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pspottercheckbutton, FALSE, FALSE, 0);
  pqrgcheckbutton = gtk_check_button_new_with_mnemonic ("QRG");
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pqrgcheckbutton, FALSE, FALSE, 0);
  pdxcheckbutton = gtk_check_button_new_with_mnemonic ("DX");
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pdxcheckbutton, FALSE, FALSE, 0);
  premarkscheckbutton = gtk_check_button_new_with_mnemonic (_("Remarks"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), premarkscheckbutton, FALSE, FALSE, 0);
  ptimecheckbutton = gtk_check_button_new_with_mnemonic (_("Time"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), ptimecheckbutton, FALSE, FALSE, 0);
  pinfocheckbutton = gtk_check_button_new_with_mnemonic (_("Info"));
  gtk_box_pack_start (GTK_BOX (pcolumnsvbox), pinfocheckbutton, FALSE, FALSE, 0);
  if (preferences.col0visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pspottercheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pspottercheckbutton), FALSE);
  if (preferences.col1visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pqrgcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pqrgcheckbutton), FALSE);
  if (preferences.col2visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pdxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pdxcheckbutton), FALSE);
  if (preferences.col3visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(premarkscheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(premarkscheckbutton), FALSE);
  if (preferences.col4visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ptimecheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(ptimecheckbutton), FALSE);
  if (preferences.col5visible == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pinfocheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(pinfocheckbutton), FALSE);


  psavingframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox2), psavingframe, TRUE, TRUE, 0);
  psavingvbox = gtk_vbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (psavingframe), psavingvbox);
  psavedxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save DX spots"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavedxcheckbutton, FALSE, FALSE, 0);
  psavewwvcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WCY/WWV"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewwvcheckbutton, FALSE, FALSE, 0);
  psavetoallcheckbutton = gtk_check_button_new_with_mnemonic (_("Save \"To all\""));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavetoallcheckbutton, FALSE, FALSE, 0);
  psavewxcheckbutton = gtk_check_button_new_with_mnemonic (_("Save WX"));
  gtk_box_pack_start (GTK_BOX (psavingvbox), psavewxcheckbutton, FALSE, FALSE, 0);
  psavinglabel = gtk_label_new (_("Saving"));
  gtk_frame_set_label_widget (GTK_FRAME (psavingframe), psavinglabel);

  pfontsframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox3), pfontsframe, TRUE, TRUE, 0);
  pfontslabel = gtk_label_new (_("Fonts"));
  gtk_frame_set_label_widget (GTK_FRAME (pfontsframe), pfontslabel);
  pfontsvbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pfontsframe), pfontsvbox);
  pfontsdxlabel = gtk_label_new (_("Font for DX messages"));
  pfontshbox1 = gtk_hbox_new (FALSE, 0);
  pfontsdxentry = gtk_entry_new ();
  pfontsdxbutton = gtk_button_new_from_stock (GTK_STOCK_SELECT_FONT);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontsdxlabel, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshbox1, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox1), pfontsdxentry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox1), pfontsdxbutton, FALSE, FALSE, 0);
  pfontshseparator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshseparator, FALSE, FALSE, 0);
  pfontsalllabel = gtk_label_new (_("Font for other messages"));
  pfontshbox2 = gtk_hbox_new (FALSE, 0);
  pfontsallentry = gtk_entry_new ();
  pfontsallbutton = gtk_button_new_from_stock (GTK_STOCK_SELECT_FONT);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontsalllabel, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontsvbox), pfontshbox2, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox2), pfontsallentry, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (pfontshbox2), pfontsallbutton, FALSE, FALSE, 0);
  gtk_entry_set_text (GTK_ENTRY (pfontsdxentry), preferences.dxfont);
  gtk_entry_set_text (GTK_ENTRY (pfontsallentry), preferences.allfont);
  gtk_editable_set_editable (GTK_EDITABLE (pfontsdxentry), FALSE);
  gtk_editable_set_editable (GTK_EDITABLE (pfontsallentry), FALSE);

  phighframe = gtk_frame_new (NULL);
  gtk_box_pack_start (GTK_BOX (pvbox3), phighframe, TRUE, TRUE, 0);
  phighlabel = gtk_label_new (_("Highlighting"));
  gtk_frame_set_label_widget (GTK_FRAME (phighframe), phighlabel);
  phighvbox = gtk_vbox_new (TRUE, 0);
  gtk_container_add (GTK_CONTAINER (phighframe), phighvbox);

  if (preferences.savedx == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavedxcheckbutton), FALSE);
  if (preferences.savewwv == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewwvcheckbutton), FALSE);
  if (preferences.savetoall == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavetoallcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavetoallcheckbutton), FALSE);
  if (preferences.savewx == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewxcheckbutton), TRUE);
  else
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(psavewxcheckbutton), FALSE);
 
  g_signal_connect ((gpointer) pautologincheckbutton, "toggled",
                    G_CALLBACK (on_pautologincheckbutton_toggled), NULL);
  g_signal_connect ((gpointer) phamlibcheckbutton, "toggled",
                    G_CALLBACK (on_phamlibcheckbutton_toggled), NULL);
  g_signal_connect ((gpointer) pfontsdxbutton, "clicked",
                    G_CALLBACK (on_fontbutton_clicked), GINT_TO_POINTER(1));
  g_signal_connect ((gpointer) pfontsallbutton, "clicked",
                    G_CALLBACK (on_fontbutton_clicked), GINT_TO_POINTER(2));

  g_object_set_data (G_OBJECT (preferencesdialog), "ploginhseparator", 
    ploginhseparator);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcallsignlabel", 
    pcallsignlabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcallsignentry", 
    pcallsignentry);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcommandslabel", 
    pcommandslabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "pcommandsentry", 
    pcommandsentry);

  g_object_set_data (G_OBJECT (preferencesdialog), "phamlibhseparator", 
    phamlibhseparator);
  g_object_set_data (G_OBJECT (preferencesdialog), "priglabel", 
    priglabel);
  g_object_set_data (G_OBJECT (preferencesdialog), "prigentry", 
    prigentry);

  g_object_set_data (G_OBJECT (preferencesdialog), "pfontsdxentry", 
    pfontsdxentry);
  g_object_set_data (G_OBJECT (preferencesdialog), "pfontsallentry", 
    pfontsallentry);

  gtk_widget_show_all (pnotebook);
  response = gtk_dialog_run (GTK_DIALOG (preferencesdialog));

  if (response == GTK_RESPONSE_OK)
  {
    /* login frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pautologincheckbutton));
    if (state) 
      preferences.autologin = 1; 
    else 
      preferences.autologin = 0;
    str = gtk_editable_get_chars (GTK_EDITABLE (pcallsignentry), 0, -1);
    if (strlen(str) == 0)
      preferences.callsign = g_strdup ("?");
    else
      preferences.callsign = g_strdup (str);
    str = gtk_editable_get_chars (GTK_EDITABLE (pcommandsentry), 0, -1);
    if (strlen(str) == 0)
      preferences.commands = g_strdup ("?");
    else
      preferences.commands = g_strdup (str);

    /* saving frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavedxcheckbutton));
    if (state) 
      preferences.savedx = 1; 
    else 
      preferences.savedx = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavewwvcheckbutton));
    if (state) 
      preferences.savewwv = 1; 
    else 
      preferences.savewwv = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavetoallcheckbutton));
    if (state) 
      preferences.savetoall = 1; 
    else 
      preferences.savetoall = 0;
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(psavewxcheckbutton));
    if (state) 
      preferences.savewx = 1; 
    else 
      preferences.savewx = 0;

    /* hamlib frame */
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(phamlibcheckbutton));
    if (state) 
      preferences.hamlib = 1; 
    else 
      preferences.hamlib = 0;
    str = gtk_editable_get_chars (GTK_EDITABLE (prigentry), 0, -1);
    if (strlen(str) == 0)
      preferences.rigctl = g_strdup ("?");
    else
      preferences.rigctl = g_strdup (str);

    /* programs frame */
    str = gtk_editable_get_chars (GTK_EDITABLE (pprogbrowserentry), 0, -1);
    if (strlen(str) == 0)
      preferences.browserapp = g_strdup ("?");
    else
      preferences.browserapp = g_strdup (str);
    str = gtk_editable_get_chars (GTK_EDITABLE (pprogmailentry), 0, -1);
    if (strlen(str) == 0)
      preferences.mailapp = g_strdup ("?");
    else
      preferences.mailapp = g_strdup (str);

    /* columns frame */
    treeview = g_object_get_data (G_OBJECT (gui->window), "treeview");
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pspottercheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 0);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col0visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL0WIDTH);
      preferences.col0visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col0visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pqrgcheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 1);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col1visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL1WIDTH);
      preferences.col1visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col1visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pdxcheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 2);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col2visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL2WIDTH);
      preferences.col2visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col2visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(premarkscheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 3);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col3visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL3WIDTH);
      preferences.col3visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col3visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(ptimecheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 4);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col4visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL4WIDTH);
      preferences.col4visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col4visible = 0;
    }
    state = gtk_toggle_button_get_active 
      (GTK_TOGGLE_BUTTON(pinfocheckbutton));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (treeview), 5);
    if (state)
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), TRUE);
      if (preferences.col5visible == 0)
        gtk_tree_view_column_set_fixed_width (column, COL5WIDTH);
      preferences.col5visible = 1;
    }	    
    else
    {
      gtk_tree_view_column_set_visible (GTK_TREE_VIEW_COLUMN(column), FALSE);
      preferences.col5visible = 0;
    }

    /* fonts frame */
    str = gtk_editable_get_chars (GTK_EDITABLE (pfontsdxentry), 0, -1);
    font_description = pango_font_description_from_string (str);
    gtk_widget_modify_font (GTK_WIDGET(treeview), font_description);
    pango_font_description_free (font_description);
    preferences.dxfont = g_strdup (str);

    str = gtk_editable_get_chars (GTK_EDITABLE (pfontsallentry), 0, -1);
    font_description = pango_font_description_from_string (str);
    maintext = g_object_get_data (G_OBJECT (gui->window), "maintext");
    gtk_widget_modify_font (GTK_WIDGET(maintext), font_description);
    pango_font_description_free (font_description);
    preferences.allfont = g_strdup (str);

    g_free (str);
  }

  gtk_widget_destroy (preferencesdialog);
  mainentry = g_object_get_data (G_OBJECT (gui->window), "mainentry");
  gtk_widget_grab_focus (GTK_WIDGET (mainentry));
  gtk_widget_set_sensitive (gui->window, 1);

}
