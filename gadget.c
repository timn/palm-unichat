/* $Id: gadget.c,v 1.1 2003/02/06 20:15:57 tim Exp $
 *
 * Text gadget related implementation
 */

#include "UniChat.h"
#include "gadget.h"
#include "tnglue.h"

FormPtr gForm=NULL;
UInt16 gGadgetID=0;
MemHandle gGadgetHandle;


/*****************************************************************************
* Function: GadgetDraw
*
* Description: Draws the basic gadget (grid, days, times)
*****************************************************************************/
void GadgetRedraw(void) {
  RectangleType bounds;
  UInt16 gadgetIndex, curLength=0;
  FormType *frm=gForm;
  Char *string;
  UInt8 curLine=0, numLines=0;
  RectangleType rect;

  // Check if GadgetSet has already been called. If not => die
  if (! frm) return;

  gadgetIndex = FrmGetObjectIndex(frm, gGadgetID);
  FrmGetObjectBounds(frm, gadgetIndex, &bounds);
  numLines = bounds.extent.y / FntLineHeight();

  // Erase background
  WinEraseRectangle(&bounds, 0);

  // draw bottom line
  WinDrawLine(bounds.topLeft.x, bounds.topLeft.y+bounds.extent.y,
              bounds.topLeft.x+bounds.extent.x, bounds.topLeft.y+bounds.extent.y);

  // Draw the current string
  string = MemHandleLock(gGadgetHandle);
  curLength = FntWordWrap(string, bounds.extent.x);

  // Set for first printed string
  RctSetRectangle(&rect, bounds.topLeft.x, bounds.topLeft.y, bounds.extent.x, bounds.extent.y);

  while (StrLen(string) != curLength) {
    Char *temp;

    temp=(Char *)MemPtrNew(curLength+1);
    MemSet(temp, MemPtrSize(temp), 0);
    StrNCopy(temp, string, curLength);

    TNDrawCharsToFitWidth(temp, &rect);

    MemPtrFree(temp);

    curLine = (curLine+1) % numLines;
    string += curLength;
    curLength = FntWordWrap(string, rect.extent.x);

    RctSetRectangle(&rect, bounds.topLeft.x, bounds.topLeft.y+curLine*FntLineHeight(),
                           bounds.extent.x, bounds.extent.y-curLine*FntLineHeight());
  }
  // Last chars
  TNDrawCharsToFitWidth(string, &rect);

  MemHandleUnlock(gGadgetHandle);
}


/*****************************************************************************
* Function: GadgetHandler
*
* Description: We use extended Gadgeting => We need an event handler for it
*****************************************************************************/
Boolean GadgetHandler(FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP) {
  Boolean handled = false; 
  EventType *event = (EventType *)paramP;
  
  switch (cmd) { 
    case formGadgetDrawCmd: 
      //Sent to active gadgets any time form is  
      //drawn or redrawn. 
      GadgetRedraw();
      gadgetP->attr.visible = true; 
      handled = true; 
      break; 
       
    case formGadgetHandleEventCmd: 
      //Sent when form receives a gadget event.  
      //paramP points to EventType structure.

      if (event->eType == frmGadgetEnterEvent) { 
        // penDown in gadget's bounds.
        GadgetTap((FormGadgetType *)gadgetP, event);
        handled = true;
      }
      if (event->eType == frmGadgetMiscEvent) {
        //This event is sent by your application
        //when it needs to send info to the gadget
      }
      break;

    case formGadgetDeleteCmd:
      //Perform any cleanup prior to deletion.
      break;

    case formGadgetEraseCmd:
      //FrmHideObject takes care of this if you 
      //return false.
       handled = false;
       break;

    default:
      break;

  }
  return handled;
}



/*****************************************************************************
* Function: GadgetTap
*
* Description: Handles penDown events (taps) on the gadget
*****************************************************************************/
void GadgetTap(FormGadgetType *pGadget, EventType *event) {
  //you may find it useful to track if they
  //lift the pen still within the boundaries of the gadget
  Boolean isPenDown = true;
  Int16 newPointX, newPointY;
  UInt16 index;
  RectangleType bounds;

  // This is just needed since we do not want to access internal structure
  // data directly in FormGadgetType (need rect field below)
  index = TNGetObjectIndexFromPtr(FrmGetActiveForm(), pGadget);
  FrmGetObjectBounds(FrmGetActiveForm(), index, &bounds);

  //track the pen down event
  while (isPenDown){
    EvtGetPen(&newPointX, &newPointY, &isPenDown);
  }

  if (RctPtInRectangle(newPointX, newPointY, &bounds)) {
    // the pen up was also in the gadget

    } // else outside gadget bounds -> do nothing

}


/*****************************************************************************
* Function: GadgetSet
*
* Description: Must be called before draw to set the form the gadget is in
*              and the IDs of the Gadget and the hint gadget
*****************************************************************************/
void GadgetSet(FormPtr frm, UInt16 gadgetID) {
  gForm=frm;
  gGadgetID=gadgetID;
}


void GadgetInit(void) {
  Char *tmp;
  gGadgetHandle = MemHandleNew(StrLen(GADGET_GREETING)+1);
  tmp = MemHandleLock(gGadgetHandle);
  MemSet(tmp, MemHandleSize(gGadgetHandle), 0);
  StrCopy(tmp, GADGET_GREETING);
  MemHandleUnlock(gGadgetHandle);
}


void GadgetFree(void) {
  MemHandleFree(gGadgetHandle);
}

MemHandle GadgetGetHandle(void) {
  return gGadgetHandle;
}
