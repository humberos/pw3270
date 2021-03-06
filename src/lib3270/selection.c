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
 * Este programa está nomeado como selection.c e possui - linhas de código.
 *
 * Contatos:
 *
 * perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
 * erico.mendonca@gmail.com	(Erico Mascarenhas Mendonça)
 *
 */

 #include "private.h"
// #include "appres.h"
 #include <lib3270.h>
 #include <lib3270/session.h>
 #include <lib3270/selection.h>
 #include "3270ds.h"

 #define SELECTION_LEFT			0x01
 #define SELECTION_TOP			0x02
 #define SELECTION_RIGHT		0x04
 #define SELECTION_BOTTOM		0x08

 #define SELECTION_SINGLE_COL	0x10
 #define SELECTION_SINGLE_ROW	0x20

 #define SELECTION_ACTIVE		0x80

 static void do_select(H3270 *h, int start, int end, int rect);

 /*--[ Implement ]------------------------------------------------------------------------------------*/

static void get_selected_addr(H3270 *session, int *start, int *end)
{
	if(session->select.start > session->select.end)
	{
		*end   = session->select.start;
		*start = session->select.end;
	}
	else
	{
		*start = session->select.start;
		*end   = session->select.end;
	}
}

static void update_selected_rectangle(H3270 *session)
{
	struct
	{
		int row;
		int col;
	} p[2];


	int begin, end, row, col, baddr;

	get_selected_addr(session,&begin,&end);

	// Get start & end posision
	p[0].row = (begin/session->cols);
	p[0].col = (begin%session->cols);
	p[1].row = (end/session->cols);
	p[1].col = (end%session->cols);

	if(p[0].row > p[1].row)
	{
		int swp = p[0].row;
		p[0].row = p[1].row;
		p[1].row = swp;
	}

	if(p[0].col > p[1].col)
	{
		int swp = p[0].col;
		p[0].col = p[1].col;
		p[1].col = swp;
	}

	// First remove unselected areas
	baddr = 0;
	for(row=0;row < session->rows;row++)
	{
		for(col = 0; col < session->cols;col++)
		{
			if(!(row >= p[0].row && row <= p[1].row && col >= p[0].col && col <= p[1].col) && (session->text[baddr].attr & LIB3270_ATTR_SELECTED))
			{
				session->text[baddr].attr &= ~LIB3270_ATTR_SELECTED;
				session->cbk.update(session,baddr,session->text[baddr].chr,session->text[baddr].attr,baddr == session->cursor_addr);
			}
			baddr++;
		}
	}

	// Then, draw selected ones
	baddr = 0;
	for(row=0;row < session->rows;row++)
	{
		for(col = 0; col < session->cols;col++)
		{
			if((row >= p[0].row && row <= p[1].row && col >= p[0].col && col <= p[1].col) && !(session->text[baddr].attr & LIB3270_ATTR_SELECTED))
			{
				session->text[baddr].attr |= LIB3270_ATTR_SELECTED;
				session->cbk.update(session,baddr,session->text[baddr].chr,session->text[baddr].attr,baddr == session->cursor_addr);
			}
			baddr++;
		}
	}

}

static void update_selected_region(H3270 *session)
{
	int baddr,begin,end;
	int len = session->rows*session->cols;

	get_selected_addr(session,&begin,&end);

	// First remove unselected areas
	for(baddr = 0; baddr < begin; baddr++)
	{
		if(session->text[baddr].attr & LIB3270_ATTR_SELECTED)
		{
			session->text[baddr].attr &= ~LIB3270_ATTR_SELECTED;
			session->cbk.update(session,baddr,session->text[baddr].chr,session->text[baddr].attr,baddr == session->cursor_addr);
		}
	}

	for(baddr = end+1; baddr < len; baddr++)
	{
		if(session->text[baddr].attr & LIB3270_ATTR_SELECTED)
		{
			session->text[baddr].attr &= ~LIB3270_ATTR_SELECTED;
			session->cbk.update(session,baddr,session->text[baddr].chr,session->text[baddr].attr,baddr == session->cursor_addr);
		}
	}

	// Then draw the selected ones
	for(baddr = begin; baddr <= end; baddr++)
	{
		if(!(session->text[baddr].attr & LIB3270_ATTR_SELECTED))
		{
			session->text[baddr].attr |= LIB3270_ATTR_SELECTED;
			session->cbk.update(session,baddr,session->text[baddr].chr,session->text[baddr].attr,baddr == session->cursor_addr);
		}
	}

}

void toggle_rectselect(H3270 *session, struct lib3270_toggle *t, LIB3270_TOGGLE_TYPE tt)
{
	if(!session->selected)
		return;

	if(t->value)
		update_selected_rectangle(session);
	else
		update_selected_region(session);
}

LIB3270_EXPORT int lib3270_unselect(H3270 *hSession)
{
	int a;

	CHECK_SESSION_HANDLE(hSession);

	if(hSession->selected)
	{
		hSession->selected = 0;

		for(a = 0; a < hSession->rows*hSession->cols; a++)
		{
			if(hSession->text[a].attr & LIB3270_ATTR_SELECTED)
			{
				hSession->text[a].attr &= ~LIB3270_ATTR_SELECTED;
				if(hSession->cbk.update)
					hSession->cbk.update(hSession,a,hSession->text[a].chr,hSession->text[a].attr,a == hSession->cursor_addr);
			}
		}

		hSession->cbk.set_selection(hSession,0);
		hSession->cbk.update_selection(hSession,-1,-1);
	}

	return 0;
}

LIB3270_EXPORT void lib3270_select_to(H3270 *session, int baddr)
{
	int start, end;

	CHECK_SESSION_HANDLE(session);

	if(!lib3270_connected(session))
		return;

	start = session->selected ? session->select.start : session->cursor_addr;

	cursor_move(session,end = baddr);

	do_select(session,start,end,lib3270_get_toggle(session,LIB3270_TOGGLE_RECTANGLE_SELECT));

}

LIB3270_EXPORT int lib3270_select_region(H3270 *h, int start, int end)
{
	int maxlen;

	CHECK_SESSION_HANDLE(h);

	if(!lib3270_connected(h))
		return ENOTCONN;

	maxlen = (h->rows * h->cols);

	// Check bounds
	if(start < 0 || start > maxlen || end < 0 || end > maxlen || start > end)
		return EINVAL;

	do_select(h,start,end,lib3270_get_toggle(h,LIB3270_TOGGLE_RECTANGLE_SELECT));
	cursor_move(h,h->select.end);

	return 0;
}

static void do_select(H3270 *h, int start, int end, int rect)
{
	if(start < 0 || end > (h->rows * h->cols))
		return;

	// Do we really need to change selection?
	if(start == h->select.start && end == h->select.end && h->selected)
		return;

	// Start address is inside the screen?
	h->select.start		= start;
	h->select.end 		= end;

	if(rect)
	{
		h->rectsel = 1;
		update_selected_rectangle(h);
	}
	else
	{
		h->rectsel = 0;
		update_selected_region(h);
	}

	if(!h->selected)
	{
		h->selected = 1;
		h->cbk.set_selection(h,1);
	}

	h->cbk.update_selection(h,start,end);

}

LIB3270_EXPORT unsigned char lib3270_get_selection_flags(H3270 *hSession, int baddr)
{
	int row,col;
	unsigned char rc = 0;

	CHECK_SESSION_HANDLE(hSession);

	if(!(lib3270_connected(hSession) && (hSession->text[baddr].attr & LIB3270_ATTR_SELECTED)))
		return rc;

	row = baddr / hSession->cols;
	col = baddr % hSession->cols;
	rc |= SELECTION_ACTIVE;

	if( (hSession->select.start % hSession->cols) == (hSession->select.end % hSession->cols) )
	{
		rc |= SELECTION_SINGLE_COL;
	}
	else
	{
		if( (col == 0) || !(hSession->text[baddr-1].attr & LIB3270_ATTR_SELECTED) )
			rc |= SELECTION_LEFT;

		if( (col == hSession->cols) || !(hSession->text[baddr+1].attr & LIB3270_ATTR_SELECTED) )
			rc |= SELECTION_RIGHT;
	}

	if( (hSession->select.start / hSession->cols) == (hSession->select.end / hSession->cols) )
	{
		rc |= SELECTION_SINGLE_ROW;
	}
	else
	{
		if( (row == 0) || !(hSession->text[baddr-hSession->cols].attr & LIB3270_ATTR_SELECTED) )
			rc |= SELECTION_TOP;

		if( (row == hSession->rows) || !(hSession->text[baddr+hSession->cols].attr & LIB3270_ATTR_SELECTED) )
			rc |= SELECTION_BOTTOM;
	}

	return rc;
}

LIB3270_EXPORT int lib3270_select_word_at(H3270 *session, int baddr)
{
	int start, end;

	if(lib3270_get_word_bounds(session,baddr,&start,&end))
		return -1;

	trace("%s: baddr=%d start=%d end=%d",__FUNCTION__,baddr,start,end);

	do_select(session,start,end,0);

	return 0;
}

LIB3270_EXPORT int lib3270_select_field_at(H3270 *session, int baddr)
{
	int start, end;

	if(lib3270_get_field_bounds(session,baddr,&start,&end))
		return -1;

	do_select(session,start,end,0);

	return 0;
}

LIB3270_EXPORT int lib3270_select_field(H3270 *hSession)
{
	CHECK_SESSION_HANDLE(hSession);
	lib3270_select_field_at(hSession,hSession->cursor_addr);
	return 0;
}

LIB3270_EXPORT int lib3270_select_all(H3270 * hSession)
{
	CHECK_SESSION_HANDLE(hSession);

	do_select(hSession,0,(hSession->rows*hSession->cols)-1,0);

	return 0;
}

LIB3270_ACTION( reselect )
{
//	int start, end;

	CHECK_SESSION_HANDLE(hSession);

	if(!lib3270_connected(hSession) || hSession->select.start == hSession->select.end || hSession->selected)
		return 0;

	do_select(hSession, hSession->select.start,hSession->select.end,lib3270_get_toggle(hSession,LIB3270_TOGGLE_RECTANGLE_SELECT));

	return 0;
}

static char * get_text(H3270 *hSession,unsigned char all, unsigned char tok)
{
	int				  row, col, baddr;
	char 			* ret;
	size_t			  buflen	= (hSession->rows * (hSession->cols+1))+1;
	size_t			  sz		= 0;
    unsigned short	  attr		= 0xFFFF;

	if(!(lib3270_connected(hSession) && hSession->text))
	{
		errno = ENOTCONN;
		return NULL;
	}

	ret = lib3270_malloc(buflen);

	baddr = 0;

	for(row=0;row < hSession->rows;row++)
	{
		int cr = 0;

		for(col = 0; col < hSession->cols;col++)
		{
			if(all || hSession->text[baddr].attr & LIB3270_ATTR_SELECTED)
			{
				if(tok && attr != hSession->text[baddr].attr)
				{
					attr = hSession->text[baddr].attr;
					ret[sz++] = tok;
					ret[sz++] = (attr & 0x0F);
					ret[sz++] = ((attr & 0xF0) >> 4);
				}

				cr++;
				ret[sz++] = hSession->text[baddr].chr;
			}
			baddr++;
		}

		if(cr)
			ret[sz++] = '\n';

        if((sz+10) > buflen)
        {
            buflen += 100;
       		ret = lib3270_realloc(ret,buflen);
        }
	}

	if(!sz)
	{
		lib3270_free(ret);
		errno = ENOENT;
		return NULL;
	}
	else if(sz > 1 && ret[sz-1] == '\n') // Remove ending \n
	{
		ret[sz-1] = 0;
	}

	ret[sz++] = 0;

	if(sz != buflen)
		ret = lib3270_realloc(ret,sz);

	return ret;
}

LIB3270_EXPORT char * lib3270_get_region(H3270 *h, int start_pos, int end_pos, unsigned char all)
{
	char *	text;
	int 	maxlen;
	int		sz = 0;
	int		baddr;

	CHECK_SESSION_HANDLE(h);

	if(!lib3270_connected(h))
		return NULL;

	maxlen = h->rows * (h->cols+1);

	if(start_pos < 0 || start_pos > maxlen || end_pos < 0 || end_pos > maxlen || end_pos < start_pos)
		return NULL;

	text = lib3270_malloc(maxlen);

	for(baddr=start_pos;baddr<end_pos;baddr++)
	{
		if(all || h->text[baddr].attr & LIB3270_ATTR_SELECTED)
			text[sz++] = (h->text[baddr].attr & LIB3270_ATTR_CG) ? ' ' : h->text[baddr].chr;

		if((baddr%h->cols) == 0 && sz > 0)
			text[sz++] = '\n';
	}
	text[sz++] = 0;

	return lib3270_realloc(text,sz);
}

LIB3270_EXPORT char * lib3270_get_text(H3270 *h, int offset, int len, char lf)
{
	char * buffer;
	int    maxlen;
	char * ptr;

	CHECK_SESSION_HANDLE(h);

	if(!lib3270_connected(h))
	{
		errno = ENOTCONN;
		return NULL;
	}

	maxlen = (h->rows * (h->cols+1)) - offset;
	if(maxlen <= 0 || offset < 0)
	{
		errno = EINVAL;
		return NULL;
	}

	if(len < 0 || len > maxlen)
		len = maxlen;

	buffer	= lib3270_malloc(len+1);
	ptr		= buffer;

//	trace("len=%d buffer=%p",len,buffer);

	while(len > 0)
	{
		if(h->text[offset].attr & LIB3270_ATTR_CG)
			*ptr = ' ';
		else if(h->text[offset].chr)
			*ptr = h->text[offset].chr;
		else
			*ptr = ' ';

		ptr++;
		offset++;
		len--;

		if(lf && (offset%h->cols) == 0 && len > 0)
		{
			*(ptr++) = lf;
			len--;
		}
	}
//	trace("len=%d buffer=%p pos=%d",len,buffer,ptr-buffer);

	*ptr = 0;

	return buffer;
}

LIB3270_EXPORT char * lib3270_get_text_at(H3270 *h, int row, int col, int len, char lf)
{
	CHECK_SESSION_HANDLE(h);
	return lib3270_get_text(h, ((row-1) * h->cols) + (col-1), len, lf);
}

LIB3270_EXPORT int lib3270_cmp_text_at(H3270 *h, int row, int col, const char *text, char lf)
{
	int		  rc;
	size_t	  sz		= strlen(text);
	char	* contents;

	contents = lib3270_get_text_at(h,row,col,sz,lf);
	if(!contents)
		return -1;

	rc = strncmp(contents,text,sz);

	lib3270_free(contents);

	return rc;
}


/**
 * Get field contents
 *
 * @param session	Session handle
 * @param baddr		Field addr
 *
 * @return String with the field contents (release it with lib3270_free()
 */
LIB3270_EXPORT char * lib3270_get_field_at(H3270 *session, int baddr)
{
	int first = lib3270_field_addr(session,baddr);

	if(first < 0)
		return NULL;

	return lib3270_get_text(session,first,lib3270_field_length(session,first)+1,0);
}

LIB3270_EXPORT int lib3270_has_selection(H3270 *hSession)
{
	CHECK_SESSION_HANDLE(hSession);
	return hSession->selected != 0;
}

LIB3270_EXPORT char * lib3270_get_selected(H3270 *hSession)
{
	CHECK_SESSION_HANDLE(hSession);

	if(!hSession->selected || hSession->select.start == hSession->select.end)
		return NULL;

	if(!lib3270_connected(hSession))
		return NULL;


	return get_text(hSession,0,0);
}

static void copy_chr(H3270 *hSession, int from, int to)
{
	if(hSession->text[from].chr == hSession->text[to].chr)
		return;

	hSession->text[to].chr = hSession->text[from].chr;

	memcpy(&hSession->ea_buf[to], &hSession->ea_buf[from],sizeof(struct lib3270_ea));
	hSession->ea_buf[from].fa = 0;

	hSession->cbk.update(	hSession,
							to,
							hSession->text[to].chr,
							hSession->text[to].attr,
							to == hSession->cursor_addr );
}

static void clear_chr(H3270 *hSession, int baddr)
{
	hSession->text[baddr].chr = ' ';

	hSession->ea_buf[baddr].cc = EBC_null;
	hSession->ea_buf[baddr].cs = 0;

	hSession->cbk.update(	hSession,
							baddr,
							hSession->text[baddr].chr,
							hSession->text[baddr].attr,
							baddr == hSession->cursor_addr );
}

int cut_addr(H3270 *hSession, int daddr, int saddr, int maxlen, int *sattr)
{
	if(hSession->ea_buf[saddr].fa)
		*sattr = hSession->ea_buf[saddr++].fa;

	if(FA_IS_PROTECTED(*sattr) || saddr >= maxlen)
		clear_chr(hSession,daddr);
	else
		copy_chr(hSession,saddr++,daddr);

	return saddr;
}

char * cut_text(H3270 *hSession, char tok)
{
    unsigned short attr = 0xFFFF;

	CHECK_SESSION_HANDLE(hSession);

	if(!hSession->selected || hSession->select.start == hSession->select.end)
		return NULL;

	if(!(lib3270_connected(hSession) && hSession->text))
		return NULL;

	trace("Rectangle select is %s",lib3270_get_toggle(hSession,LIB3270_TOGGLE_RECTANGLE_SELECT) ? "Active" : "Inactive");

	if(lib3270_get_toggle(hSession,LIB3270_TOGGLE_RECTANGLE_SELECT))
	{
		// Rectangle cut is not implemented
		lib3270_popup_dialog(hSession, LIB3270_NOTIFY_INFO, _( "Not available" ), _( "Invalid cut action" ), "%s", _( "Can't cut rectangular regions") );
	}
	else
	{
		int end;
		size_t szText;
		size_t buflen;
		size_t bufpos = 0;
		int daddr;	/* Destination addr */
		int dattr;	/* Destination addr attribute */
		int saddr;	/* Source addr (First field after the selected area) */
		int sattr;	/* Source addr attribute */
		char *text;
		size_t maxlen = hSession->rows * hSession->cols;
		int f;

		get_selected_addr(hSession,&daddr,&end);

		lib3270_set_cursor_address(hSession,daddr);

		if(daddr >= end)
			return NULL;

		dattr = lib3270_field_attribute(hSession,daddr);	/* Get first attribute */

		szText = (end-daddr)+1;
		buflen = szText;
		bufpos = 0;

		text = lib3270_malloc(buflen+1);

		saddr = daddr+szText;
		sattr = lib3270_field_attribute(hSession,saddr);

		for(f=0;f<szText;f++)
		{
			if(hSession->ea_buf[daddr].fa)
				dattr = hSession->ea_buf[daddr].fa;

            if((bufpos+10) > buflen)
            {
                buflen += 100;
                text = lib3270_realloc(text,buflen);
            }

            if(tok && attr != hSession->text[daddr].attr)
            {
                attr = hSession->text[daddr].attr;
                text[bufpos++] = tok;
                text[bufpos++] = (attr & 0x0F);
                text[bufpos++] = ((attr & 0xF0) >> 4);
            }

			text[bufpos++] = hSession->text[daddr].chr;

			if(!FA_IS_PROTECTED(dattr))
				saddr = cut_addr(hSession,daddr,saddr,maxlen,&sattr);

			daddr++;
		}

        text[bufpos++] = 0;
        text = lib3270_realloc(text,bufpos);

		// Move contents of the current field
		while(daddr < (maxlen-1) && !hSession->ea_buf[daddr].fa)
		{
			saddr = cut_addr(hSession,daddr,saddr,maxlen,&sattr);
			daddr++;
		}

		if(!hSession->ea_buf[daddr].fa)
			clear_chr(hSession,daddr);

		hSession->cbk.changed(hSession,0,maxlen);

		lib3270_unselect(hSession);
		return text;
	}

	return NULL;
}

LIB3270_EXPORT char * lib3270_cut_selected(H3270 *hSession)
{
    return cut_text(hSession,0);
}

LIB3270_EXPORT int lib3270_get_selection_bounds(H3270 *hSession, int *start, int *end)
{
	int first, last;

	CHECK_SESSION_HANDLE(hSession);

	if(!hSession->selected || hSession->select.start == hSession->select.end)
		return 0;

	if(hSession->select.end > hSession->select.start)
	{
		first = hSession->select.start;
		last  = hSession->select.end;
	}
	else
	{
		first = hSession->select.end;
		last  = hSession->select.start;
	}

	if(start)
		*start = first;

	if(end)
		*end = last;

	return 1;
}

LIB3270_EXPORT int lib3270_move_selected_area(H3270 *hSession, int from, int to)
{
	int pos[2];
	int rows, cols, f, step;

	if(from == to)
		return from;

	if(!lib3270_get_selection_bounds(hSession,&pos[0],&pos[1]))
		return from;

	rows = (to / hSession->cols) - (from / hSession->cols);
	cols = (to % hSession->cols) - (from % hSession->cols);

	for(f=0;f<2;f++)
	{
		int row  = (pos[f] / hSession->cols) + rows;
		int col  = (pos[f] % hSession->cols) + cols;

		if(row < 0)
			rows = - (pos[f] / hSession->cols);

		if(col < 0)
			cols = - (pos[f] % hSession->cols);

		if(row >= (hSession->rows))
			rows = hSession->rows - ((pos[f] / hSession->cols)+1);

		if(col >= hSession->cols)
			cols = hSession->cols - ((pos[f] % hSession->cols)+1);
	}

	step = (rows * hSession->cols) + cols;

	do_select(hSession,hSession->select.start + step,hSession->select.end + step,hSession->rectsel);
	cursor_move(hSession,hSession->select.end);

	return from+step;
}

LIB3270_EXPORT int lib3270_drag_selection(H3270 *h, unsigned char flag, int origin, int baddr)
{
	int first, last, row, col;

	if(!lib3270_get_selection_bounds(h,&first,&last))
		return origin;

/*
	trace("%s: flag=%04x %s %s %s %s",__FUNCTION__,
				flag,
				flag & SELECTION_LEFT	? "Left"	: "-",
				flag & SELECTION_TOP	? "Top"		: "-",
				flag & SELECTION_RIGHT	? "Right"	: "-",
				flag & SELECTION_BOTTOM	? "Bottom"	: "-"
				);
*/

	if(!flag)
		return origin;
	else if((flag&0x8F) == SELECTION_ACTIVE)
		return lib3270_move_selected_area(h,origin,baddr);

	row = baddr/h->cols;
	col = baddr%h->cols;

	if(flag & SELECTION_LEFT)		// Update left margin
		origin = first = ((first/h->cols)*h->cols) + col;

	if(flag & SELECTION_TOP)		// Update top margin
		origin = first = (row*h->cols) + (first%h->cols);

	if(flag & SELECTION_RIGHT) 		// Update right margin
		origin = last = ((last/h->cols)*h->cols) + col;

	if(flag & SELECTION_BOTTOM)		// Update bottom margin
		origin = last = (row*h->cols) + (last%h->cols);

	trace("origin=%d first=%d last=%d",origin,first,last);

	if(first < last)
		do_select(h,first,last,h->rectsel);
	else
		do_select(h,last,first,h->rectsel);

	cursor_move(h,h->select.end);

	return origin;
}


LIB3270_EXPORT int lib3270_move_selection(H3270 *hSession, LIB3270_DIRECTION dir)
{
	int start, end;

	if(!hSession->selected || hSession->select.start == hSession->select.end)
		return ENOENT;

	start = hSession->select.start;
	end   = hSession->select.end;

	switch(dir)
	{
	case LIB3270_DIR_UP:
		if(start <= hSession->cols)
			return EINVAL;
		start -= hSession->cols;
		end   -= hSession->cols;
		break;

	case LIB3270_DIR_DOWN:
		if(end >= (hSession->cols * (hSession->rows-1)))
			return EINVAL;
		start += hSession->cols;
		end   += hSession->cols;
		break;

	case LIB3270_DIR_LEFT:
		if( (start % hSession->cols) < 1)
			return EINVAL;
		start--;
		end--;
		break;

	case LIB3270_DIR_RIGHT:
		if( (end % hSession->cols) >= (hSession->cols-1))
			return EINVAL;
		start++;
		end++;
		break;

	default:
		return -1;
	}

	do_select(hSession,start,end,hSession->rectsel);
	cursor_move(hSession,hSession->select.end);

	return 0;
}

LIB3270_EXPORT int lib3270_move_cursor(H3270 *hSession, LIB3270_DIRECTION dir, unsigned char sel)
{
	int cursor_addr = hSession->cursor_addr;
	int maxlen		= hSession->cols * hSession->rows;

	if(!lib3270_connected(hSession))
		return -1;

	switch(dir)
	{
	case LIB3270_DIR_UP:
		if(sel && cursor_addr <= hSession->cols)
			return EINVAL;
		cursor_addr -= hSession->cols;
		break;

	case LIB3270_DIR_DOWN:
		if(sel && cursor_addr >= (hSession->cols * (hSession->rows-1)))
			return EINVAL;
		cursor_addr += hSession->cols;
		break;

	case LIB3270_DIR_LEFT:
		if(sel &&  (cursor_addr % hSession->cols) < 1)
			return EINVAL;
		cursor_addr--;
		break;

	case LIB3270_DIR_RIGHT:
		if(sel &&  (cursor_addr % hSession->cols) >= (hSession->cols-1))
			return EINVAL;
		cursor_addr++;
		break;

	case LIB3270_DIR_END:
		cursor_addr = lib3270_get_field_end(hSession,cursor_addr);
		if(cursor_addr == -1)
			return EINVAL;
		break;

	default:
		return -1;
	}

	if(sel)
	{
		lib3270_select_to(hSession,cursor_addr);
	}
	else if(cursor_addr >= maxlen)
	{
		cursor_move(hSession,cursor_addr % maxlen);
	}
	else if(cursor_addr < 0)
	{
		cursor_move(hSession,cursor_addr + maxlen);
	}
	else
	{
		cursor_move(hSession,cursor_addr);
	}

	return 0;
}

