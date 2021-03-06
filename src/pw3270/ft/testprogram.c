/*
 * "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
 * (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
 * aplicativos mainframe. Registro no INPI sob o nome G3270.
 *
 * Copyright (C) <2008> <Banco do Brasil S.A.>
 *
 * Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
 * os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
 * Free Software Foundation.
 *
 * Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
 * GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
 * A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
 * obter mais detalhes.
 *
 * Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
 * programa; se não, escreva para a Free Software Foundation, Inc., 51 Franklin
 * St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Este programa está nomeado como testprogram.c e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 #include <gtk/gtk.h>
 #include "v3270ft.h"

/*--[ Implement ]------------------------------------------------------------------------------------*/

int main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);

	{
		GtkWidget *win = v3270_ft_dialog_new(NULL,LIB3270_FT_OPTION_RECEIVE);
		v3270_ft_dialog_set_options(win,LIB3270_FT_OPTION_REMAP);
		gtk_dialog_run(GTK_DIALOG(win));
	}

	/*
	{
		GtkWidget *win = v3270_ft_dialog_new(NULL,LIB3270_FT_OPTION_SEND|LIB3270_FT_RECORD_FORMAT_VARIABLE);
		v3270_ft_dialog_set_tso(win,FALSE);
		gtk_dialog_run(GTK_DIALOG(win));
	}
	*/

	/*
	{
		GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_container_add(GTK_CONTAINER(win),v3270_ft_progress_new());
		gtk_widget_show_all(win);
		gtk_main ();
	}
	*/

	return 0;

}
