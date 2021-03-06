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
 * Este programa está nomeado como lib3270.h e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 typedef enum _hllapi_packet
 {
		HLLAPI_PACKET_CONNECT,
		HLLAPI_PACKET_SET_HOST,
		HLLAPI_PACKET_DISCONNECT,
		HLLAPI_PACKET_GET_PROGRAM_MESSAGE,
		HLLAPI_PACKET_GET_TEXT_AT_OFFSET,
		HLLAPI_PACKET_GET_TEXT_AT,
		HLLAPI_PACKET_SET_TEXT_AT,
		HLLAPI_PACKET_CMP_TEXT_AT,
		HLLAPI_PACKET_ENTER,
		HLLAPI_PACKET_PFKEY,
		HLLAPI_PACKET_PAKEY,
		HLLAPI_PACKET_SET_CURSOR_POSITION,
		HLLAPI_PACKET_GET_CURSOR_POSITION,
		HLLAPI_PACKET_INPUT_STRING,
		HLLAPI_PACKET_IS_CONNECTED,
		HLLAPI_PACKET_SET_CURSOR,
		HLLAPI_PACKET_GET_CURSOR,
		HLLAPI_PACKET_EMULATE_INPUT,
		HLLAPI_PACKET_ERASE,
		HLLAPI_PACKET_ERASE_EOF,
		HLLAPI_PACKET_ERASE_EOL,
		HLLAPI_PACKET_ERASE_INPUT,
		HLLAPI_PACKET_PRINT,
		HLLAPI_PACKET_GET_CSTATE,
		HLLAPI_PACKET_IS_READY,
		HLLAPI_PACKET_SET_TOGGLE,
		HLLAPI_PACKET_FIELD_START,
		HLLAPI_PACKET_FIELD_LEN,
		HLLAPI_PACKET_NEXT_UNPROTECTED,
		HLLAPI_PACKET_IS_PROTECTED,
		HLLAPI_PACKET_IS_PROTECTED_AT,
		HLLAPI_PACKET_QUIT,
		HLLAPI_PACKET_ACTION,

		HLLAPI_PACKET_SET_HOST_CHARSET,
		HLLAPI_PACKET_GET_HOST_CHARSET,

		HLLAPI_PACKET_ASC2EBC,
		HLLAPI_PACKET_EBC2ASC,

		HLLAPI_PACKET_FILE_TRANSFER,

		HLLAPI_PACKET_GET_SSL_STATE,
		HLLAPI_PACKET_SET_UNLOCK_DELAY,

		HLLAPI_PACKET_GET_WIDTH,
		HLLAPI_PACKET_GET_HEIGHT,
		HLLAPI_PACKET_GET_LENGTH,
		HLLAPI_PACKET_GET_HOST,

		HLLAPI_PACKET_CONNECT_URL,

		HLLAPI_PACKET_INVALID

 } HLLAPI_PACKET;

#pragma pack(1)

struct hllapi_packet_result
{
	int 			rc;
};

struct hllapi_packet_text_result
{
	int 			rc;
	char 			text[1];
};

struct hllapi_packet_query
{
	unsigned char	packet_id;
};

struct hllapi_packet_connect
{
	unsigned char	packet_id;
	unsigned char	wait;
};

struct hllapi_packet_keycode
{
	unsigned char	packet_id;
	unsigned short	keycode;
};

struct hllapi_packet_cursor
{
	unsigned char	packet_id;
	unsigned short	row;
	unsigned short	col;
};

struct hllapi_packet_text
{
	unsigned char	packet_id;
	char 			text[1];
};

struct hllapi_packet_at
{
	unsigned char	packet_id;
	unsigned short	row;
	unsigned short	col;
	unsigned short	len;
	char			lf;
};

struct hllapi_packet_text_at
{
	unsigned char	packet_id;
	unsigned short	row;
	unsigned short	col;
	char			lf;
	char 			text[1];
};

struct hllapi_packet_query_at
{
	unsigned char	packet_id;
	unsigned short	row;
	unsigned short	col;
	unsigned short	len;
	char			lf;
};

struct hllapi_packet_wait
{
	unsigned char	packet_id;
	int				timeout;
};

struct hllapi_packet_addr
{
	unsigned char	packet_id;
	unsigned short	addr;
};

struct hllapi_packet_query_offset
{
	unsigned char	packet_id;
	unsigned short	addr;
	unsigned short	len;
	char			lf;
};

struct hllapi_packet_emulate_input
{
	unsigned char	packet_id;
	unsigned short	len;
	unsigned char	pasting;
	char 			text[1];
};

struct hllapi_packet_set
{
	unsigned char	packet_id;
	unsigned short	id;
	unsigned short	value;
};

struct hllapi_packet_set_text
{
	unsigned char	packet_id;
	unsigned short	len;
	char 			text[1];
};

struct hllapi_packet_set_int
{
	unsigned char	packet_id;
	int				value;
};

struct hllapi_packet_file_transfer
{
	unsigned char		packet_id;
	unsigned int		options;
	int					lrecl;
	int					blksize;
	int					primspace;
	int 				secspace;
	int 				dft;
	char				text[1];
};

#pragma pack()

