/* $Id: resources.h,v 1.1 2005/07/24 16:30:21 tim Exp $
 *
 * ID's for use in ressource definitions
 *
 * Definitions:
 * 1:         Version
 * 100-199:   app specific values
 * 1001-1999: Forms, form items
 * 2001-2999: Menus and menu items
 * 4001-4999: Help
 * 8001-8999: Special stuff, bitmaps etc.
 */

#ifndef __UNICHAT_RESOURCES_H_
#define __UNICHAT_RESOURCES_H_

#define APP_CREATOR 'UCHT'
#define APP_ID 100
#define APP_NAMEID 101
#define APP_CATEGORIES 102

#define UNIMATRIX_APP_CREATOR 'UMTX'
#define UNIMENSA_APP_CREATOR 'UMEN'


#define FORM_main 1001
#define GADGET_main 1010
#define FIELD_input 1011
#define BUTTON_beam 1012
#define SCROLLBAR_input 1013
#define BUTTON_del 1014
#define BUTTON_info 1015
#define BUTTON_help 1016

#define FORM_about 1020
#define BITMAP_logo 1021
#define BUTTON_about_ok 1022


#define MENU_main 2001
#define MENUITEM_about 2002
#define MENUITEM_matrix 2003
#define MENUITEM_addsep 2004
#define MENUITEM_mensa 2005
#define MENUITEM_send 2006


#define ALERT_ROMIncompatible 4001
#define ALERT_debug 4002
#define ALERT_notrunning 4003
#define HELP_main 4004

#define BITMAP_edit 5002
#define BITMAP_del 5003
#define BITMAP_del_clicked 5004
#define BITMAP_edit_clicked 5005
#define BITMAP_info 5006
#define BITMAP_info_clicked 5007
#define BITMAP_beam 5008
#define BITMAP_beam_clicked 5009
#define BITMAP_help 5010
#define BITMAP_help_clicked 5011
#define BITMAP_ok 5014
#define BITMAP_ok_clicked 5015
#define BITMAP_cancel 5016
#define BITMAP_cancel_clicked 5017

#endif /* __UNICHAT_RESOURCES_H_ */
