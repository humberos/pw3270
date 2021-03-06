/*
 * "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
 * (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
 * aplicativos mainframe. Registro no INPI sob o nome G3270. Registro no INPI sob o nome G3270.
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
 * Este programa está nomeado como session.c e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 * licinio@bb.com.br		(Licínio Luis Branco)
 * kraucer@bb.com.br		(Kraucer Fernandes Mazuco)
 *
 */

#ifndef ANDROID
	#include <stdlib.h>
#endif // !ANDROID

#include "private.h"
// #include "charsetc.h"
#include "kybdc.h"
#include "ansic.h"
#include "togglesc.h"
#include "screen.h"
#include "screenc.h"
#include "ctlrc.h"
#include "ftc.h"
#include "kybdc.h"
#include "3270ds.h"
// #include "tablesc.h"
#include "popupsc.h"
//#include "charset.h"

/*---[ Globals ]--------------------------------------------------------------------------------------------------------------*/

 static H3270 *default_session = NULL;

/*---[ Statics ]--------------------------------------------------------------------------------------------------------------*/

/*---[ Implement ]------------------------------------------------------------------------------------------------------------*/

void lib3270_session_free(H3270 *h)
{
	int f;

	if(!h)
		return;

	// Terminate session
	if(lib3270_connected(h))
		lib3270_disconnect(h);

	shutdown_toggles(h);

	// Release state change callbacks
	for(f=0;f<LIB3270_STATE_USER;f++)
	{
		while(h->st_callbacks[f])
		{
			struct lib3270_state_callback *next = h->st_callbacks[f]->next;
			lib3270_free(h->st_callbacks[f]);
			h->st_callbacks[f] = next;
		}
	}

	// Release memory
	#define release_pointer(x) lib3270_free(x); x = NULL;

	// release_pointer(h->charset.display);
	release_pointer(h->paste_buffer);

	release_pointer(h->ibuf);
	h->ibuf_size = 0;

	for(f=0;f<(sizeof(h->buffer)/sizeof(h->buffer[0]));f++)
	{
		release_pointer(h->buffer[f]);
	}

	if(h == default_session)
		default_session = NULL;

	// Release hostname info
	release_pointer(h->host.current);
	release_pointer(h->host.full);
	release_pointer(h->host.srvc);
	release_pointer(h->host.qualified);

	release_pointer(h->charset.host);
	release_pointer(h->charset.display);

	trace("Releasing session %p",h);
	lib3270_free(h);

}

static void update_char(H3270 *session, int addr, unsigned char chr, unsigned short attr, unsigned char cursor)
{
}

static void nop_char(H3270 *session, unsigned char chr)
{
}

static void nop(H3270 *session)
{
}

static void update_model(H3270 *session, const char *name, int model, int rows, int cols)
{
}

static void changed(H3270 *session, int bstart, int bend)
{
}

static void update_cursor(H3270 *session, unsigned short row, unsigned short col, unsigned char c, unsigned short attr)
{
}

static void update_oia(H3270 *session, LIB3270_FLAG id, unsigned char on)
{
}

static void update_selection(H3270 *session, int start, int end)
{
}

static void set_cursor(H3270 *session, LIB3270_POINTER id)
{
}

static int print(H3270 *session)
{
	lib3270_write_log(session, "print", "%s", "Printing is unavailable");
	return -1;
}

static void message(H3270 *session, LIB3270_NOTIFY id , const char *title, const char *msg, const char *text)
{
#ifdef ANDROID
	__android_log_print(ANDROID_LOG_VERBOSE, PACKAGE_NAME, "%s\n",title);
	__android_log_print(ANDROID_LOG_VERBOSE, PACKAGE_NAME, "%s\n",msg);
	__android_log_print(ANDROID_LOG_VERBOSE, PACKAGE_NAME, "%s\n",text);
#else
	lib3270_write_log(session,"lib3270","%s",title);
	lib3270_write_log(session,"lib3270","%s",msg);
	lib3270_write_log(session,"lib3270","%s",text);
#endif // ANDROID
}

static void update_ssl(H3270 *session, LIB3270_SSL_STATE state)
{
}

static void set_timer(H3270 *session, unsigned char on)
{
}

static void screen_disp(H3270 *session)
{
	CHECK_SESSION_HANDLE(session);
	screen_update(session,0,session->rows*session->cols);
}

static void nop_int(H3270 *session, int width)
{
	return;
}

static void lib3270_session_init(H3270 *hSession, const char *model, const char *charset)
{
	int		f;

	memset(hSession,0,sizeof(H3270));
//	hSession->sz = sizeof(H3270);

	lib3270_set_host_charset(hSession,charset);

	// Default calls
	hSession->cbk.write				= lib3270_sock_send;
	hSession->cbk.disconnect		= lib3270_sock_disconnect;
	hSession->cbk.update 			= update_char;
	hSession->cbk.update_model		= update_model;
	hSession->cbk.update_cursor		= update_cursor;
	hSession->cbk.set_selection 	= nop_char;
	hSession->cbk.ctlr_done			= nop;
	hSession->cbk.changed			= changed;
	hSession->cbk.erase				= screen_disp;
	hSession->cbk.suspend			= nop;
	hSession->cbk.resume			= screen_disp;
	hSession->cbk.update_oia		= update_oia;
	hSession->cbk.update_selection	= update_selection;
	hSession->cbk.cursor 			= set_cursor;
	hSession->cbk.message			= message;
	hSession->cbk.update_ssl		= update_ssl;
	hSession->cbk.display			= screen_disp;
	hSession->cbk.set_width			= nop_int;
	hSession->cbk.update_status		= (void (*)(H3270 *, LIB3270_MESSAGE)) nop_int;
	hSession->cbk.autostart			= nop;
	hSession->cbk.set_timer			= set_timer;
	hSession->cbk.print				= print;

	// Set the defaults.
	hSession->extended  			=  1;
	hSession->typeahead				=  1;
	hSession->oerr_lock 			=  1;
	hSession->unlock_delay			=  1;
	hSession->icrnl 				=  1;
	hSession->onlcr					=  1;
	hSession->sock					= -1;
	hSession->model_num				= -1;
	hSession->cstate				= LIB3270_NOT_CONNECTED;
	hSession->oia_status			= -1;
	hSession->kybdlock 				= KL_NOT_CONNECTED;
	hSession->aid 					= AID_NO;
	hSession->reply_mode 			= SF_SRM_FIELD;
	hSession->linemode				= 1;
	hSession->tn3270e_submode		= E_NONE;
	hSession->scroll_top 			= -1;
	hSession->scroll_bottom			= -1;
	hSession->wraparound_mode 		= 1;
	hSession->saved_wraparound_mode	= 1;
	hSession->once_cset 			= -1;
	hSession->state					= LIB3270_ANSI_STATE_DATA;
	hSession->options				= LIB3270_OPTION_DEFAULTS;
	hSession->colors				= 16;
	hSession->m3279					= 1;
	hSession->unlock_delay_ms		= 350; // 0.35s after last unlock
	hSession->pointer				= (unsigned short) LIB3270_POINTER_LOCKED;

	// CSD
	for(f=0;f<4;f++)
		hSession->csd[f] = hSession->saved_csd[f] = LIB3270_ANSI_CSD_US;

	// Initialize toggles
	initialize_toggles(hSession);

	lib3270_set_model(hSession,model);

}

H3270 * lib3270_session_new(const char *model)
{
	H3270 * hSession;

	trace("%s - configured=%s",__FUNCTION__,default_session ? "Yes" : "No");

	hSession = lib3270_malloc(sizeof(H3270));

	if(!default_session)
		default_session = hSession;

	lib3270_session_init(hSession, model, _( "bracket" ) );

	if(screen_init(hSession))
		return NULL;

	trace("%s: Initializing KYBD",__FUNCTION__);
	lib3270_register_schange(hSession,LIB3270_STATE_CONNECT,kybd_connect,NULL);
	lib3270_register_schange(hSession,LIB3270_STATE_3270_MODE,kybd_in3270,NULL);

#if defined(X3270_ANSI)
	trace("%s: Initializing ANSI",__FUNCTION__);
	lib3270_register_schange(hSession,LIB3270_STATE_3270_MODE,ansi_in3270,NULL);
#endif // X3270_ANSI


#if defined(X3270_FT)
	ft_init(hSession);
#endif

	trace("%s finished",__FUNCTION__);

	errno = 0;
	return hSession;
}

#if defined(DEBUG)
void check_session_handle(H3270 **hSession, const char *fname)
#else
void check_session_handle(H3270 **hSession)
#endif // DEBUG
{
	if(*hSession)
		return;

	*hSession = lib3270_get_default_session_handle();

#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_VERBOSE, PACKAGE_NAME, "%s called with empty session\n", __FUNCTION__);
#elif defined(DEBUG)
	lib3270_write_log(*hSession, "lib3270", "%s called with empty session from %s",__FUNCTION__,fname);
#else
	lib3270_write_log(*hSession, "lib3270", "%s called with empty session",__FUNCTION__);
#endif // ANDROID
}

LIB3270_EXPORT H3270 * lib3270_get_default_session_handle(void)
{
	if(default_session)
		return default_session;

	return lib3270_session_new("");
}

LIB3270_EXPORT void lib3270_set_user_data(H3270 *h, void *ptr)
{
	CHECK_SESSION_HANDLE(h);
	h->user_data = ptr;
}

LIB3270_EXPORT void * lib3270_get_user_data(H3270 *h)
{
	CHECK_SESSION_HANDLE(h);
	return h->user_data;
}

struct lib3270_session_callbacks * lib3270_get_session_callbacks(H3270 *session, unsigned short sz)
{
	CHECK_SESSION_HANDLE(session);

	if(sz != sizeof(struct lib3270_session_callbacks))
		return NULL;

	return &session->cbk;
}


