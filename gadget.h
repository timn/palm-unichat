/* $Id: gadget.h,v 1.1 2003/02/06 20:15:57 tim Exp $
 *
 * Gadget header
 */

#define GADGET_GREETING "\nMessage of the Day:\n\n/root: > kick windoze && install linux"

extern void GadgetRedraw(void);
extern Boolean GadgetHandler(FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP);
extern void GadgetTap(FormGadgetType *pGadget, EventType *event);
extern void GadgetSet(FormPtr frm, UInt16 gadgetID);
extern void GadgetInit(void);
extern void GadgetFree(void);
extern MemHandle GadgetGetHandle(void);
