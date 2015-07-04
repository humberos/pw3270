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
 * Este programa está nomeado como hllapi.h e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 * licinio@bb.com.br		(Licínio Luis Branco)
 * kraucer@bb.com.br		(Kraucer Fernandes Mazuco)
 *
 */

#ifndef HLLAPI_H_INCLUDED

	#define HLLAPI_H_INCLUDED 1
	#include <lib3270.h>

#ifdef __cplusplus
extern "C" {
#endif

 #define HLLAPI_MAXLENGTH					4096

 /* Function codes */
 #define HLLAPI_CMD_CONNECTPS				   1	/**< connect presentation space							*/
 #define HLLAPI_CMD_DISCONNECTPS			   2	/**< disconnect presentation space        				*/
 #define HLLAPI_CMD_INPUTSTRING				   3	/**< send string										*/
 #define HLLAPI_CMD_WAIT					   4	/**< Wait if the session is waiting for a host response	*/
 #define HLLAPI_CMD_COPYPS					   5	/**< Copies the contents of the presentation space into a string buffer. */
 #define HLLAPI_CMD_SEARCHPS				   6	/**< Search the presentation space for a specified string. */
 #define HLLAPI_CMD_QUERYCURSOR				   7	/**< Determines the location of the cursor in the presentation space. */
 #define HLLAPI_CMD_COPYPSTOSTR				   8	/**< Copy presentation space to string					*/
 #define HLLAPI_CMD_COPYSTRTOPS				  15	/**< Copies an ASCII string directly to a specified position in the presentation space. */
 #define HLLAPI_CMD_SETCURSOR				  40	/**< Places the cursor at a specified position in presentation space.*/
 #define HLLAPI_CMD_SENDFILE				  90	/**< Send file to the host */
 #define HLLAPI_CMD_RECEIVEFILE				  91	/**< Receive a file from the host */

 #define HLLAPI_CMD_GETREVISION				2000	/**< Get lib3270 revision								*/


 /* Result codes */
 #define HLLAPI_STATUS_SUCCESS				   0	/**< Good return code */
 #define HLLAPI_STATUS_DISCONNECTED			   1	/**< The presentation space was not valid or not connected. */
 #define HLLAPI_STATUS_BAD_PARAMETER		   2	/**< An incorrect option was specified. */
 #define HLLAPI_STATUS_TIMEOUT				   4	/**< Timeout */
 #define HLLAPI_STATUS_KEYBOARD_LOCKED		   5	/**< The keyboard is locked. */
 #define HLLAPI_STATUS_UNAVAILABLE			  11	/**< Resource unavailable at this time */
 #define HLLAPI_STATUS_SYSTEM_ERROR			   9 	/**< A system error occurred */

 #define HLLAPI_STATUS_WAITING	HLLAPI_STATUS_TIMEOUT

 #ifdef WIN32
	#include <windows.h>

	// http://www.mingw.org/wiki/Visual_Basic_DLL
	#define HLLAPI_API_CALL __declspec (dllexport) DWORD __stdcall

 #endif // WIN32

	HLLAPI_API_CALL hllapi(const LPWORD func, LPSTR str, LPWORD length, LPWORD rc);

	HLLAPI_API_CALL hllapi_init(LPSTR mode);
	HLLAPI_API_CALL hllapi_deinit(void);

	HLLAPI_API_CALL hllapi_get_revision(void);
	HLLAPI_API_CALL hllapi_get_datadir(LPSTR datadir);

	HLLAPI_API_CALL hllapi_connect(LPSTR uri, WORD wait);
	HLLAPI_API_CALL hllapi_disconnect(void);
	HLLAPI_API_CALL hllapi_get_message_id(void);
	HLLAPI_API_CALL hllapi_is_connected(void);
	HLLAPI_API_CALL hllapi_get_state(void);
	HLLAPI_API_CALL hllapi_get_screen_at(WORD row, WORD col, LPSTR buffer);
	HLLAPI_API_CALL hllapi_get_screen(WORD pos, LPSTR buffer, WORD len);
	HLLAPI_API_CALL hllapi_enter(void);
	HLLAPI_API_CALL hllapi_set_text_at(WORD row, WORD col, LPSTR text);
	HLLAPI_API_CALL hllapi_cmp_text_at(WORD row, WORD col, LPSTR text);
    HLLAPI_API_CALL hllapi_emulate_input(LPSTR buffer, WORD len, WORD pasting);
	HLLAPI_API_CALL hllapi_wait_for_ready(WORD seconds);
	HLLAPI_API_CALL hllapi_wait(WORD seconds);
	HLLAPI_API_CALL hllapi_pfkey(WORD key);
	HLLAPI_API_CALL hllapi_pakey(WORD key);
	HLLAPI_API_CALL hllapi_setcursor(WORD key);
	HLLAPI_API_CALL hllapi_getcursor();

	HLLAPI_API_CALL hllapi_erase(void);
	HLLAPI_API_CALL hllapi_erase_eof(void);
	HLLAPI_API_CALL hllapi_erase_eol(void);
	HLLAPI_API_CALL hllapi_erase_input(void);

	HLLAPI_API_CALL hllapi_print(void);

	HLLAPI_API_CALL hllapi(const LPWORD func, LPSTR str, LPWORD length, LPWORD rc);

	HLLAPI_API_CALL hllapi_init(LPSTR mode);
	HLLAPI_API_CALL hllapi_deinit(void);

	HLLAPI_API_CALL hllapi_get_revision(void);
	HLLAPI_API_CALL hllapi_get_datadir(LPSTR datadir);

	HLLAPI_API_CALL hllapi_connect(LPSTR uri, WORD wait);
	HLLAPI_API_CALL hllapi_disconnect(void);
	HLLAPI_API_CALL hllapi_get_message_id(void);
	HLLAPI_API_CALL hllapi_is_connected(void);
	HLLAPI_API_CALL hllapi_get_state(void);
	HLLAPI_API_CALL hllapi_get_screen_at(WORD row, WORD col, LPSTR buffer);
	HLLAPI_API_CALL hllapi_get_screen(WORD pos, LPSTR buffer, WORD len);
	HLLAPI_API_CALL hllapi_enter(void);
	HLLAPI_API_CALL hllapi_set_text_at(WORD row, WORD col, LPSTR text);
	HLLAPI_API_CALL hllapi_cmp_text_at(WORD row, WORD col, LPSTR text);
    HLLAPI_API_CALL hllapi_emulate_input(LPSTR buffer, WORD len, WORD pasting);
	HLLAPI_API_CALL hllapi_wait_for_ready(WORD seconds);
	HLLAPI_API_CALL hllapi_wait(WORD seconds);
	HLLAPI_API_CALL hllapi_pfkey(WORD key);
	HLLAPI_API_CALL hllapi_pakey(WORD key);
	HLLAPI_API_CALL hllapi_setcursor(WORD key);
	HLLAPI_API_CALL hllapi_getcursor();
	HLLAPI_API_CALL hllapi_erase_eof(void);
	HLLAPI_API_CALL hllapi_print(void);

#ifdef __cplusplus
}    /* end of extern "C" */
#endif

#endif // HLLAPI_H_INCLUDED
